// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <algorithm>
#include <chrono>
#include <mutex>
#include <string>

// Return file size according to open FILE* object
inline size_t filesize(FILE *f) {
  if (f == nullptr) {
    throw("Failed getting file size. fd is null");
  }
#if defined(_WIN32) && !defined(__CYGWIN__)
  int fd = ::_fileno(f);
#if _WIN64  // 64 bits
  __int64 ret = ::_filelengthi64(fd);
  if (ret >= 0) {
    return static_cast<size_t>(ret);
  }

#else  // windows 32 bits
  long ret = ::_filelength(fd);
  if (ret >= 0) {
    return static_cast<size_t>(ret);
  }
#endif

#else  // unix
// OpenBSD doesn't compile with :: before the fileno(..)
#if defined(__OpenBSD__)
  int fd = fileno(f);
#else
  int fd = ::fileno(f);
#endif
// 64 bits(but not in osx or cygwin, where fstat64 is deprecated)
#if (defined(__linux__) || defined(__sun) || defined(_AIX)) && \
    (defined(__LP64__) || defined(_LP64))
  struct stat64 st;
  if (::fstat64(fd, &st) == 0) {
    return static_cast<size_t>(st.st_size);
  }
#else  // other unix or linux 32 bits or cygwin
  struct stat st;
  if (::fstat(fd, &st) == 0) {
    return static_cast<size_t>(st.st_size);
  }
#endif
#endif
  throw("Failed getting file size from fd", errno);
  return 0;  // will not be reached.
}
// folder separator
#ifdef _WIN32
static const char folder_sep = '\\';
#else
SPDLOG_CONSTEXPR static const char folder_sep = '/';
#endif
// Return true if path exists (file or directory)
inline bool path_exists(const std::string &filename) noexcept {
#ifdef _WIN32
#ifdef SPDLOG_WCHAR_FILENAMES
  auto attribs = ::GetFileAttributesW(filename.c_str());
#else
  auto attribs = ::GetFileAttributesA(filename.c_str());
#endif
  return attribs != INVALID_FILE_ATTRIBUTES;
#else  // common linux/unix all have the stat system call
  struct stat buffer;
  return (::stat(filename.c_str(), &buffer) == 0);
#endif
}

// return true on success
static SPDLOG_INLINE bool mkdir_(const std::string &path) {
#ifdef _WIN32
#ifdef SPDLOG_WCHAR_FILENAMES
  return ::_wmkdir(path.c_str()) == 0;
#else
  return ::_mkdir(path.c_str()) == 0;
#endif
#else
  return ::mkdir(path.c_str(), mode_t(0755)) == 0;
#endif
}

// create the given directory - and all directories leading to it
// return true on success or if the directory already exists
inline bool create_dir(std::string path) {
  if (path_exists(path)) {
    return true;
  }

  if (path.empty()) {
    return false;
  }

#ifdef _WIN32
  // support forward slash in windows
  std::replace(path.begin(), path.end(), '/', folder_sep);
#endif

  size_t search_offset = 0;
  do {
    auto token_pos = path.find(folder_sep, search_offset);
    // treat the entire path as a folder if no folder separator not found
    if (token_pos == std::string::npos) {
      token_pos = path.size();
    }

    auto subdir = path.substr(0, token_pos);

    if (!subdir.empty() && !path_exists(subdir) && !mkdir_(subdir)) {
      return false;  // return error if failed creating dir
    }
    search_offset = token_pos + 1;
  } while (search_offset < path.size());

  return true;
}

inline int remove(const std::string& filename) noexception
{
#if defined(_WIN32) && defined(SPDLOG_WCHAR_FILENAMES)
    return ::_wremove(filename.c_str());
#else
    return std::remove(filename.c_str());
#endif
}

// Return directory name from given path or empty string
// "abc/file" => "abc"
// "abc/" => "abc"
// "abc" => ""
// "abc///" => "abc//"
inline std::string dir_name(std::string path) {
#ifdef _WIN32
  // support forward slash in windows
  std::replace(path.begin(), path.end(), '/', folder_sep);
#endif
  auto pos = path.find_last_of(folder_sep);
  return pos != std::string::npos ? path.substr(0, pos) : std::string{};
}

// This is avoid msvc issue in sleep_for that happens if the clock changes.
// See https://github.com/gabime/spdlog/issues/609
inline void sleep_for_millis(int milliseconds) noexcept {
#if defined(_WIN32)
  ::Sleep(milliseconds);
#else
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
#endif
}

// wchar support for windows file names (SPDLOG_WCHAR_FILENAMES must be defined)
#if defined(_WIN32) && defined(SPDLOG_WCHAR_FILENAMES)
SPDLOG_INLINE std::string filename_to_str(const filename_t &filename) {
  memory_buf_t buf;
  wstr_to_utf8buf(filename, buf);
  return fmt::to_string(buf);
}
#else
inline std::string filename_to_str(const std::string &filename) {
  return filename;
}
#endif

// Helper class for file sinks.
// When failing to open a file, retry several times(5) with a delay interval(10
// ms). Throw spdlog_ex exception on errors.

class file_helper {
 public:
  explicit file_helper() = default;

  file_helper(const file_helper &) = delete;
  file_helper &operator=(const file_helper &) = delete;
  inline ~file_helper() { close(); }

  inline void open(const std::string &fname, bool truncate) {
    close();
    filename_ = fname;
    auto *mode = truncate ? ("wb") : ("ab");

    for (int tries = 0; tries < open_tries_; ++tries) {
      // create containing folder if not exists already.
      create_dir(dir_name(fname));
      if (!fopen_s(&fd_, fname.c_str(), mode)) {
        return;
      }

      sleep_for_millis(open_interval_);
    }

    throw("Failed opening file " + filename_to_str(filename_) + " for writing",
          errno);
  }

  inline void reopen(bool truncate) {
    if (filename_.empty()) {
      throw("Failed re opening file - was not opened before");
    }
    this->open(filename_, truncate);
  }

  inline void flush() { std::fflush(fd_); }

  inline void close() {
    if (fd_ != nullptr) {
      std::fclose(fd_);
      fd_ = nullptr;
    }
  }

  inline void write(const std::string &buf) {
    size_t msg_size = buf.size();
    auto data = buf.data();
    if (std::fwrite(data, 1, msg_size, fd_) != msg_size) {
      throw("Failed writing to file " + filename_to_str(filename_), errno);
    }
  }
  inline size_t size() const {
    if (fd_ == nullptr) {
      throw("Cannot use size() on closed file " + filename_to_str(filename_));
    }
    return filesize(fd_);
  }

  const std::string &filename() const { return filename_; }

  //
  // return file path and its extension:
  //
  // "mylog.txt" => ("mylog", ".txt")
  // "mylog" => ("mylog", "")
  // "mylog." => ("mylog.", "")
  // "/dir1/dir2/mylog.txt" => ("/dir1/dir2/mylog", ".txt")
  //
  // the starting dot in filenames is ignored (hidden files):
  //
  // ".mylog" => (".mylog". "")
  // "my_folder/.mylog" => ("my_folder/.mylog", "")
  // "my_folder/.mylog.txt" => ("my_folder/.mylog", ".txt")
  static std::tuple<std::string, std::string> split_by_extension(
      const std::string &fname) {
    auto ext_index = fname.rfind('.');

    // no valid extension found - return whole path and empty string as
    // extension
    if (ext_index == std::string::npos || ext_index == 0 ||
        ext_index == fname.size() - 1) {
      return std::make_tuple(fname, std::string());
    }

    // treat cases like "/etc/rc.d/somelogfile or "/abc/.hiddenfile"
    auto folder_index = fname.rfind(folder_sep);
    if (folder_index != std::string::npos && folder_index >= ext_index - 1) {
      return std::make_tuple(fname, std::string());
    }

    // finally - return a valid base and extension tuple
    return std::make_tuple(fname.substr(0, ext_index), fname.substr(ext_index));
  }

 private:
  const int open_tries_ = 5;
  const int open_interval_ = 10;
  std::FILE *fd_{nullptr};
  std::string filename_;
};

struct log_msg {
  log_msg() = default;
  inline log_msg(std::chrono::system_clock::time_point log_time, source_loc loc,
                 std::basic_string_view<char> a_logger_name, level_enum lvl,
                 std::basic_string_view<char> msg)
      : logger_name(a_logger_name),
        level(lvl),
        time(log_time),
        source(loc),
        payload(msg) {}

  inline log_msg(source_loc loc, std::basic_string_view<char> a_logger_name,
                 level_enum lvl, std::basic_string_view<char> msg)
      : log_msg(std::chrono::system_clock::now(), loc, a_logger_name, lvl,
                msg) {}
  inline log_msg(std::basic_string_view<char> a_logger_name, level_enum lvl,
                 std::basic_string_view<char> msg)
      : log_msg(std::chrono::system_clock::now(), source_loc{}, a_logger_name,
                lvl, msg) {}
  log_msg(const log_msg &other) = default;

  std::basic_string_view<char> logger_name;
  level_enum level{level_enum::off};
  std::chrono::system_clock::time_point time;
  size_t thread_id{0};

  // wrapping the formatted text with color (updated by pattern_formatter).
  mutable size_t color_range_start{0};
  mutable size_t color_range_end{0};

  source_loc source;
  std::basic_string_view<char> payload;
};

enum class level_enum {
  trace = SPDLOG_LEVEL_TRACE,
  debug = SPDLOG_LEVEL_DEBUG,
  info = SPDLOG_LEVEL_INFO,
  warn = SPDLOG_LEVEL_WARN,
  err = SPDLOG_LEVEL_ERROR,
  critical = SPDLOG_LEVEL_CRITICAL,
  off = SPDLOG_LEVEL_OFF,
  n_levels
};

class formatter {
 public:
  virtual ~formatter() = default;
  virtual void format(const details::log_msg &msg, memory_buf_t &dest) = 0;
  virtual std::unique_ptr<formatter> clone() const = 0;
};
class sink {
 public:
  virtual ~sink() = default;
  virtual void log(const log_msg &msg) = 0;
  virtual void flush() = 0;
  virtual void set_pattern(const std::string &pattern) = 0;
  virtual void set_formatter(std::unique_ptr<formatter> sink_formatter) = 0;

  inline bool should_log(level_enum msg_level) const {
    return msg_level >= level_.load(std::memory_order_relaxed);
  }

  inline void set_level(level_enum log_level) {
    level_.store(log_level, std::memory_order_relaxed);
  }

  inline level_enum level() const {
    return static_cast<level_enum>(level_.load(std::memory_order_relaxed));
  }

 protected:
  // sink log level - default is all
  level_enum level_{level::trace};
};

template <typename Mutex>
class base_sink : public sink {
 public:
  base_sink() : formatter_{details::make_unique<spdlog::pattern_formatter>()} {}
  explicit base_sink(std::unique_ptr<spdlog::formatter> formatter)
      : formatter_{std::move(formatter)} {}

  ~base_sink() override = default;

  base_sink(const base_sink &) = delete;
  base_sink(base_sink &&) = delete;

  base_sink &operator=(const base_sink &) = delete;
  base_sink &operator=(base_sink &&) = delete;

  void log(const details::log_msg &msg) final {
    std::lock_guard<Mutex> lock(mutex_);
    sink_it_(msg);
  }
  void flush() final {
    std::lock_guard<Mutex> lock(mutex_);
    flush_();
  }
  void set_pattern(const std::string &pattern) final {
    std::lock_guard<Mutex> lock(mutex_);
    set_pattern_(pattern);
  }
  void set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) final {
    std::lock_guard<Mutex> lock(mutex_);
    set_pattern_(pattern);
  }

 protected:
  // sink formatter
  std::unique_ptr<spdlog::formatter> formatter_;
  Mutex mutex_;

  virtual void sink_it_(const details::log_msg &msg) = 0;
  virtual void flush_() = 0;
  virtual void set_pattern_(const std::string &pattern) {
    set_formatter_(details::make_unique<spdlog::pattern_formatter>(pattern));
  }
  virtual void set_formatter_(
      std::unique_ptr<spdlog::formatter> sink_formatter) {
    formatter_ = std::move(sink_formatter);
  }
};
//
// Rotating file sink based on size
//
template <typename Mutex>
class rotating_file_sink final : public base_sink<Mutex> {
 public:
  rotating_file_sink(std::string base_filename, std::size_t max_size,
                     std::size_t max_files, bool rotate_on_open = false)
      : base_filename_(std::move(base_filename)),
        max_size_(max_size),
        max_files_(max_files) {
    file_helper_.open(calc_filename(base_filename_, 0));
    current_size_ = file_helper_.size();  // expensive. called only once
    if (rotate_on_open && current_size_ > 0) {
      rotate_();
    }
  }

  static inline std::string calc_filename(const std::string &filename,
                                          std::size_t index) {
    if (index == 0u) {
      return filename;
    }

    std::string basename, ext;
    std::tie(basename, ext) = file_helper::split_by_extension(filename);
    return fmt::format(("{}.{}{}"), basename, index, ext);
  }
  template <typename Mutex>
  inline std::string filename() {
    std::lock_guard<Mutex> lock(base_sink<Mutex>::mutex_);
    return file_helper_.filename();
  }

 protected:
  void sink_it_(const details::log_msg &msg) override {
    std::string formatted;
    base_sink<Mutex>::formatter_->format(msg, formatted);
    current_size_ += formatted.size();
    if (current_size_ > max_size_) {
      rotate_();
      current_size_ = formatted.size();
    }
    file_helper_.write(formatted);
  }
  void flush_() override { file_helper_.flush(); }

 private:
  // Rotate files:
  // log.txt -> log.1.txt
  // log.1.txt -> log.2.txt
  // log.2.txt -> log.3.txt
  // log.3.txt -> delete
  inline void rotate_() {
    /// using filename_to_str;
    /// using path_exists;
    file_helper_.close();
    for (auto i = max_files_; i > 0; --i) {
      std::string src = calc_filename(base_filename_, i - 1);
      if (!path_exists(src)) {
        continue;
      }
      std::string target = calc_filename(base_filename_, i);

      if (!rename_file_(src, target)) {
        // if failed try again after a small delay.
        // this is a workaround to a windows issue, where very high rotation
        // rates can cause the rename to fail with permission denied (because of
        // antivirus?).
        sleep_for_millis(100);
        if (!rename_file_(src, target)) {
          file_helper_.reopen(true);  // truncate the log file anyway to prevent
                                      // it to grow beyond its limit!
          current_size_ = 0;
          throw("rotating_file_sink: failed renaming " +
                              filename_to_str(src) + " to " +
                              filename_to_str(target),
                          errno);
        }
      }
    }
    file_helper_.reopen(true);
  }

  // delete the target if exists, and rename the src file  to target
  // return true on success, false otherwise.
  template <typename Mutex>
  inline bool rename_file_(const filename_t &src_filename,
                           const filename_t &target_filename) {
    // try to delete the target file in case it already exists.
    (void)details::os::remove(target_filename);
    return details::os::rename(src_filename, target_filename) == 0;
  }

  std::string base_filename_;
  std::size_t max_size_;
  std::size_t max_files_;
  std::size_t current_size_;
  file_helper file_helper_;  /// 用于打开、写文件的对象
};
