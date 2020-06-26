
#pragma once

#include <atomic>
#include <cassert>
#include <chrono>
#include <functional>
#include <initializer_list>
#include <memory>
#include <mutex>
#include <string_view>
#include <thread>
#include <vector>

namespace spdlog {
/// 异步溢出策略 - 默认为阻塞
enum class async_overflow_policy {
  block,          ///< 阻塞至知道消息可以被入列
  overrun_oldest  ///< 在尝试添加新的东西时如果队列满了就丢弃队列中老的消息
};

#define SPDLOG_LEVEL_TRACE 0
#define SPDLOG_LEVEL_DEBUG 1
#define SPDLOG_LEVEL_INFO 2
#define SPDLOG_LEVEL_WARN 3
#define SPDLOG_LEVEL_ERROR 4
#define SPDLOG_LEVEL_CRITICAL 5
#define SPDLOG_LEVEL_OFF 6

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

//
// Pattern time - specific time getting to use for pattern_formatter.
// local time by default
//
enum class pattern_time_type {
  local,  // log localtime
  utc     // log utc
};

class async_logger;
using async_logger_ptr = std::shared_ptr<spdlog::async_logger>;

enum class async_msg_type { log, flush, terminate };

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

struct source_loc {
  constexpr source_loc() = default;
  constexpr source_loc(const char *filename_in, int line_in,
                       const char *funcname_in)
      : filename{filename_in}, line{line_in}, funcname{funcname_in} {}

  constexpr bool empty() const noexcept { return line == 0; }
  const char *filename{nullptr};
  int line{0};
  const char *funcname{nullptr};
};

class formatter {
 public:
  virtual ~formatter() = default;
  virtual void format(const log_msg &msg, std::string &dest) = 0;
  virtual std::unique_ptr<formatter> clone() const = 0;
};

struct null_mutex {
  void lock() const {}
  void unlock() const {}
  bool try_lock() const { return true; }
};

struct null_atomic_int {
  int value;
  null_atomic_int() = default;

  explicit null_atomic_int(int new_value) : value(new_value) {}

  int load(std::memory_order = std::memory_order_relaxed) const {
    return value;
  }

  void store(int new_value, std::memory_order = std::memory_order_relaxed) {
    value = new_value;
  }

  int exchange(int new_value, std::memory_order = std::memory_order_relaxed) {
    std::swap(new_value, value);
    return new_value;  // return value before the call
  }
};

class sink {
 public:
  virtual ~sink() = default;
  virtual void log(const log_msg &msg) = 0;
  virtual void flush() = 0;
  virtual void set_pattern(const std::string &pattern) = 0;
  virtual void set_formatter(
      std::unique_ptr<spdlog::formatter> sink_formatter) = 0;

  level_enum level() const;
  inline bool should_log(level_enum msg_level) const {
    return static_cast<int>(msg_level) >=
           level_.load(std::memory_order_relaxed);
  }

  inline void set_level(level_enum log_level) {
    level_.store(static_cast<int>(log_level), std::memory_order_relaxed);
  }

  inline level_enum level() const {
    return static_cast<level_enum>(level_.load(std::memory_order_relaxed));
  }

 protected:
  // sink log level - default is all
  std::atomic<int> level_{static_cast<int>(level_enum::trace)};
};

template <typename T>
class circular_q {
  size_t max_items_ = 0;
  typename std::vector<T>::size_type head_ = 0;
  typename std::vector<T>::size_type tail_ = 0;
  size_t overrun_counter_ = 0;
  std::vector<T> v_;

 public:
  using value_type = T;

  // empty ctor - create a disabled queue with no elements allocated at all
  circular_q() = default;

  explicit circular_q(size_t max_items)
      : max_items_(max_items + 1)  // one item is reserved as marker for full q
        ,
        v_(max_items_) {}

  circular_q(const circular_q &) = default;
  circular_q &operator=(const circular_q &) = default;

  // move cannot be default,
  // since we need to reset head_, tail_, etc to zero in the moved object
  circular_q(circular_q &&other) noexcept { copy_moveable(std::move(other)); }

  circular_q &operator=(circular_q &&other) noexcept {
    copy_moveable(std::move(other));
    return *this;
  }

  // push back, overrun (oldest) item if no room left
  void push_back(T &&item) {
    if (max_items_ > 0) {
      v_[tail_] = std::move(item);
      tail_ = (tail_ + 1) % max_items_;

      if (tail_ == head_)  // overrun last item if full
      {
        head_ = (head_ + 1) % max_items_;
        ++overrun_counter_;
      }
    }
  }

  // Return reference to the front item.
  // If there are no elements in the container, the behavior is undefined.
  const T &front() const { return v_[head_]; }

  T &front() { return v_[head_]; }

  // Return number of elements actually stored
  size_t size() const {
    if (tail_ >= head_) {
      return tail_ - head_;
    } else {
      return max_items_ - (head_ - tail_);
    }
  }

  // Return const reference to item by index.
  // If index is out of range 0��size()-1, the behavior is undefined.
  const T &at(size_t i) const {
    assert(i < size());
    return v_[(head_ + i) % max_items_];
  }

  // Pop item from front.
  // If there are no elements in the container, the behavior is undefined.
  void pop_front() { head_ = (head_ + 1) % max_items_; }

  bool empty() const { return tail_ == head_; }

  bool full() const {
    // head is ahead of the tail by 1
    if (max_items_ > 0) {
      return ((tail_ + 1) % max_items_) == head_;
    }
    return false;
  }

  size_t overrun_counter() const { return overrun_counter_; }

 private:
  // copy from other&& and reset it to disabled state
  void copy_moveable(circular_q &&other) noexcept {
    max_items_ = other.max_items_;
    head_ = other.head_;
    tail_ = other.tail_;
    overrun_counter_ = other.overrun_counter_;
    v_ = std::move(other.v_);

    // put &&other in disabled, but valid state
    other.max_items_ = 0;
    other.head_ = other.tail_ = 0;
    other.overrun_counter_ = 0;
  }
};
template <typename T>
class mpmc_blocking_queue {
 public:
  using item_type = T;
  explicit mpmc_blocking_queue(size_t max_items) : q_(max_items) {}

  // try to enqueue and block if no room left
  void enqueue(T &&item) {
    {
      std::unique_lock<std::mutex> lock(queue_mutex_);
      pop_cv_.wait(lock, [this] { return !this->q_.full(); });
      q_.push_back(std::move(item));
    }
    push_cv_.notify_one();
  }

  // enqueue immediately. overrun oldest message in the queue if no room left.
  void enqueue_nowait(T &&item) {
    {
      std::unique_lock<std::mutex> lock(queue_mutex_);
      q_.push_back(std::move(item));
    }
    push_cv_.notify_one();
  }

  // try to dequeue item. if no item found. wait upto timeout and try again
  // Return true, if succeeded dequeue item, false otherwise
  bool dequeue_for(T &popped_item, std::chrono::milliseconds wait_duration) {
    {
      std::unique_lock<std::mutex> lock(queue_mutex_);
      if (!push_cv_.wait_for(lock, wait_duration,
                             [this] { return !this->q_.empty(); })) {
        return false;
      }
      popped_item = std::move(q_.front());
      q_.pop_front();
    }
    pop_cv_.notify_one();
    return true;
  }

  size_t overrun_counter() {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    return q_.overrun_counter();
  }

 private:
  std::mutex queue_mutex_;
  std::condition_variable push_cv_;
  std::condition_variable pop_cv_;
  circular_q<T> q_;
};

// Extend log_msg with internal buffer to store its payload.
// This is needed since log_msg holds string_views that points to stack data.

class log_msg_buffer : public log_msg {
  /// fmt::basic_memory_buffer<char, 250> buffer;
  std::string buffer;
  void update_string_views() {
    logger_name =
        std::basic_string_view<char>{buffer.data(), logger_name.size()};
    payload = {buffer.data() + logger_name.size(), payload.size()};
  }

 public:
  log_msg_buffer() = default;
  explicit log_msg_buffer(const log_msg &orig_msg) : log_msg{orig_msg} {
    buffer.reserve(250);
    buffer.append(logger_name.begin(), logger_name.end());
    buffer.append(payload.begin(), payload.end());
    update_string_views();
  }

  log_msg_buffer(const log_msg_buffer &other) : log_msg{other} {
    buffer.reserve(250);
    buffer.append(logger_name.begin(), logger_name.end());
    buffer.append(payload.begin(), payload.end());
    update_string_views();
  }

  log_msg_buffer(log_msg_buffer &&other) noexcept
      : log_msg{other}, buffer{std::move(other.buffer)} {
    buffer.reserve(250);
    update_string_views();
  }

  log_msg_buffer &operator=(const log_msg_buffer &other) {
    log_msg::operator=(other);
    buffer.reserve(250);
    buffer.clear();
    buffer.append(other.buffer.data(),
                  other.buffer.data() + other.buffer.size());
    update_string_views();
    return *this;
  }

  log_msg_buffer &operator=(log_msg_buffer &&other) noexcept {
    log_msg::operator=(other);
    buffer.reserve(250);
    buffer = std::move(other.buffer);
    update_string_views();
    return *this;
  }
};

class backtracer {
  mutable std::mutex mutex_;
  std::atomic<bool> enabled_{false};
  circular_q<log_msg_buffer> messages_;

 public:
  backtracer() = default;
  inline backtracer(const backtracer &other) {
    std::lock_guard<std::mutex> lock(other.mutex_);
    enabled_ = other.enabled();
    messages_ = other.messages_;
  }

  inline backtracer(backtracer &&other) noexcept {
    std::lock_guard<std::mutex> lock(other.mutex_);
    enabled_ = other.enabled();
    messages_ = std::move(other.messages_);
  }

  inline backtracer &backtracer::operator=(backtracer other) {
    std::lock_guard<std::mutex> lock(mutex_);
    enabled_ = other.enabled();
    messages_ = std::move(other.messages_);
    return *this;
  }

  inline void enable(size_t size) {
    std::lock_guard<std::mutex> lock{mutex_};
    enabled_.store(true, std::memory_order_relaxed);
    messages_ = circular_q<log_msg_buffer>{size};
  }

  inline void disable() {
    std::lock_guard<std::mutex> lock{mutex_};
    enabled_.store(false, std::memory_order_relaxed);
  }

  inline bool enabled() const {
    return enabled_.load(std::memory_order_relaxed);
  }

  inline void push_back(const log_msg &msg) {
    std::lock_guard<std::mutex> lock{mutex_};
    messages_.push_back(log_msg_buffer{msg});
  }

  // pop all items in the q and apply the given fun on each of them.
  inline void foreach_pop(std::function<void(const log_msg &)> fun) {
    std::lock_guard<std::mutex> lock{mutex_};
    while (!messages_.empty()) {
      auto &front_msg = messages_.front();
      fun(front_msg);
      messages_.pop_front();
    }
  }
};

class pattern_formatter final : public formatter
{
public:
    using custom_flags = std::unordered_map<char, std::unique_ptr<custom_flag_formatter>>;

    explicit pattern_formatter(std::string pattern, pattern_time_type time_type = pattern_time_type::local,
        std::string eol = spdlog::details::os::default_eol, custom_flags custom_user_flags = custom_flags());

    // use default pattern is not given
    explicit pattern_formatter(pattern_time_type time_type = pattern_time_type::local, std::string eol = spdlog::details::os::default_eol);

    pattern_formatter(const pattern_formatter &other) = delete;
    pattern_formatter &operator=(const pattern_formatter &other) = delete;

    std::unique_ptr<formatter> clone() const override;
    void format(const log_msg &msg, memory_buf_t &dest) override;

    template<typename T, typename... Args>
    pattern_formatter &add_flag(char flag, const Args &... args)
    {
        custom_handlers_[flag] = details::make_unique<T>(args...);
        return *this;
    }
    void set_pattern(std::string pattern);

private:
    std::string pattern_;
    std::string eol_;
    pattern_time_type pattern_time_type_;
    std::tm cached_tm_;
    std::chrono::seconds last_log_secs_;
    std::vector<std::unique_ptr<details::flag_formatter>> formatters_;
    custom_flags custom_handlers_;

    std::tm get_time_(const details::log_msg &msg);
    template<typename Padder>
    void handle_flag_(char flag, details::padding_info padding);

    // Extract given pad spec (e.g. %8X)
    // Advance the given it pass the end of the padding spec found (if any)
    // Return padding.
    static details::padding_info handle_padspec_(std::string::const_iterator &it, std::string::const_iterator end);

    void compile_pattern_(const std::string &pattern);
};

class logger {
 public:
  // Empty logger
  explicit logger(std::string name) : name_(std::move(name)), sinks_() {}

  // Logger with range on sinks
  template <typename It>
  logger(std::string name, It begin, It end)
      : name_(std::move(name)), sinks_(begin, end) {}

  // Logger with single sink
  logger(std::string name, std::shared_ptr<sink> single_sink)
      : logger(std::move(name), {std::move(single_sink)}) {}

  // Logger with sinks init list
  logger(std::string name, std::initializer_list<std::shared_ptr<sink>> sinks)
      : logger(std::move(name), sinks.begin(), sinks.end()) {}

  virtual ~logger() = default;
  // public methods
  inline logger::logger(const logger &other)
      : name_(other.name_),
        sinks_(other.sinks_),
        level_(other.level_.load(std::memory_order_relaxed)),
        flush_level_(other.flush_level_.load(std::memory_order_relaxed)),
        custom_err_handler_(other.custom_err_handler_),
        tracer_(other.tracer_) {}

  inline logger(logger &&other) noexcept
      : name_(std::move(other.name_)),
        sinks_(std::move(other.sinks_)),
        level_(other.level_.load(std::memory_order_relaxed)),
        flush_level_(other.flush_level_.load(std::memory_order_relaxed)),
        custom_err_handler_(std::move(other.custom_err_handler_)),
        tracer_(std::move(other.tracer_))

  {}

  inline logger &operator=(logger other) noexcept {
    this->swap(other);
    return *this;
  }

  inline void swap(spdlog::logger &other) noexcept {
    name_.swap(other.name_);
    sinks_.swap(other.sinks_);

    // swap level_
    auto other_level = other.level_.load();
    auto my_level = level_.exchange(other_level);
    other.level_.store(my_level);

    // swap flush level_
    other_level = other.flush_level_.load();
    my_level = flush_level_.exchange(other_level);
    other.flush_level_.store(my_level);

    custom_err_handler_.swap(other.custom_err_handler_);
    std::swap(tracer_, other.tracer_);
  }

  void log(source_loc loc, level_enum lvl, const std::string &fmt) {
    log_(loc, lvl, fmt);
  }

  void log(source_loc loc, level_enum lvl, std::basic_string_view<char> fmt) {
    log_(loc, lvl, fmt);
  }

  template <typename FormatString, typename... Args>
  void log(level_enum lvl, const FormatString &fmt, const Args &... args) {
    log(source_loc{}, lvl, fmt, args...);
  }

  template <typename FormatString, typename... Args>
  void trace(const FormatString &fmt, const Args &... args) {
    log(level::trace, fmt, args...);
  }

  template <typename FormatString, typename... Args>
  void debug(const FormatString &fmt, const Args &... args) {
    log(level::debug, fmt, args...);
  }

  template <typename FormatString, typename... Args>
  void info(const FormatString &fmt, const Args &... args) {
    log(level::info, fmt, args...);
  }

  template <typename FormatString, typename... Args>
  void warn(const FormatString &fmt, const Args &... args) {
    log(level::warn, fmt, args...);
  }

  template <typename FormatString, typename... Args>
  void error(const FormatString &fmt, const Args &... args) {
    log(level::err, fmt, args...);
  }

  template <typename FormatString, typename... Args>
  void critical(const FormatString &fmt, const Args &... args) {
    log(level::critical, fmt, args...);
  }

  template <typename T>
  void log(level_enum lvl, const T &msg) {
    log(source_loc{}, lvl, msg);
  }

  void log(source_loc loc, level_enum lvl, const std::string &msg) {
    log(loc, lvl, msg);
  }

  void log(std::chrono::system_clock::time_point log_time, source_loc loc,
           level_enum lvl, std::basic_string_view<char> msg) {
    bool log_enabled = should_log(lvl);
    bool traceback_enabled = tracer_.enabled();
    if (!log_enabled && !traceback_enabled) {
      return;
    }

    log_msg log_msg(log_time, loc, name_, lvl, msg);
    log_it_(log_msg, log_enabled, traceback_enabled);
  }

  void log(source_loc loc, level_enum lvl, std::basic_string_view<char> msg) {
    bool log_enabled = should_log(lvl);
    bool traceback_enabled = tracer_.enabled();
    if (!log_enabled && !traceback_enabled) {
      return;
    }

    log_msg log_msg(loc, name_, lvl, msg);
    log_it_(log_msg, log_enabled, traceback_enabled);
  }

  void log(level_enum lvl, std::string_view msg) {
    log(source_loc{}, lvl, msg);
  }

  void log(source_loc loc, level_enum lvl, const std::string &msg) {
    log(loc, lvl, msg);
  }

  template <typename T>
  void trace(const T &msg) {
    log(level::trace, msg);
  }

  template <typename T>
  void debug(const T &msg) {
    log(level::debug, msg);
  }

  template <typename T>
  void info(const T &msg) {
    log(level::info, msg);
  }

  template <typename T>
  void warn(const T &msg) {
    log(level::warn, msg);
  }

  template <typename T>
  void error(const T &msg) {
    log(level::err, msg);
  }

  template <typename T>
  void critical(const T &msg) {
    log(level::critical, msg);
  }

  // return true logging is enabled for the given level.
  inline bool should_log(level_enum msg_level) const {
    return static_cast<int>(msg_level) >=
           level_.load(std::memory_order_relaxed);
  }

  // return true if backtrace logging is enabled.
  inline bool should_backtrace() const { return tracer_.enabled(); }

  inline void set_level(level_enum log_level)
  {
    level_.store(static_cast<int>(log_level));
  }

  inline level_enum level() const {
    return static_cast<level_enum>(level_.load(std::memory_order_relaxed));
  }

  inline const std::string& name() const { return name_;
  }

// set formatting for the sinks in this logger.
// each sink will get a separate instance of the formatter object.
inline void set_formatter(std::unique_ptr<formatter> f)
{
    for (auto it = sinks_.begin(); it != sinks_.end(); ++it)
    {
        if (std::next(it) == sinks_.end())
        {
            // last element - we can be move it.
            (*it)->set_formatter(std::move(f));
            break; // to prevent clang-tidy warning
        }
        else
        {
            (*it)->set_formatter(f->clone());
        }
    }
}

inline void set_pattern(std::string pattern,
  pattern_time_type time_type = pattern_time_type::local) {
    auto new_formatter = std::make_unique<pattern_formatter>(std::move(pattern), time_type);
    set_formatter(std::move(new_formatter));
  }

  // backtrace support.
  // efficiently store all debug/trace messages in a circular buffer until
  // needed for debugging.
  void enable_backtrace(size_t n_messages);
  void disable_backtrace();
  void dump_backtrace();

  // flush functions
  void flush();
  void flush_on(level_enum log_level);
  level_enum flush_level() const;

  // sinks
  const std::vector<std::shared_ptr<sink>> &sinks() const;

  std::vector<std::shared_ptr<sink>> &sinks();

  // error handler
  void set_error_handler(std::function<void(const std::string &err_msg)>);

  // create new logger with same sinks and configuration.
  virtual std::shared_ptr<logger> clone(std::string logger_name);

 protected:
  std::string name_;
  std::vector<std::shared_ptr<sink>> sinks_;
  std::atomic<int> level_{level_enum::info};
  std::atomic<int> flush_level_{level_enum::off};
  std::function<void(const std::string &err_msg)> custom_err_handler_{nullptr};
  backtracer tracer_;

  // common implementation for after templated public api has been resolved
  template <typename FormatString, typename... Args>
  void log_(source_loc loc, level_enum lvl, const FormatString &fmt,
            const Args &... args) {
    bool log_enabled = should_log(lvl);
    bool traceback_enabled = tracer_.enabled();
    if (!log_enabled && !traceback_enabled) {
      return;
    }
    SPDLOG_TRY {
      memory_buf_t buf;
      fmt::format_to(buf, fmt, args...);
      details::log_msg log_msg(loc, name_, lvl,
                               string_view_t(buf.data(), buf.size()));
      log_it_(log_msg, log_enabled, traceback_enabled);
    }
    SPDLOG_LOGGER_CATCH()
  }

  // log the given message (if the given log level is high enough),
  // and save backtrace (if backtrace is enabled).
  void log_it_(const log_msg &log_msg, bool log_enabled,
               bool traceback_enabled);
  virtual void sink_it_(const log_msg &msg);
  virtual void flush_();
  void dump_backtrace_();
  bool should_flush_(const log_msg &msg);

  // handle errors during logging.
  // default handler prints the error to stderr at max rate of 1 message/sec.
  void err_handler_(const std::string &msg);
};

void swap(logger &a, logger &b);

class async_logger final : public std::enable_shared_from_this<async_logger>,
                           public logger {
  friend class thread_pool;

 public:
  template <typename It>
  async_logger(
      std::string logger_name, It begin, It end, std::weak_ptr<thread_pool> tp,
      async_overflow_policy overflow_policy = async_overflow_policy::block)
      : logger(std::move(logger_name), begin, end),
        thread_pool_(std::move(tp)),
        overflow_policy_(overflow_policy) {}

  async_logger(
      std::string logger_name, std::initializer_list<sink> sinks_list,
      std::weak_ptr<thread_pool> tp,
      async_overflow_policy overflow_policy = async_overflow_policy::block);

  async_logger(
      std::string logger_name, std::shared_ptr<sink> single_sink,
      std::weak_ptr<thread_pool> tp,
      async_overflow_policy overflow_policy = async_overflow_policy::block);

  std::shared_ptr<logger> clone(std::string new_name) override;

 protected:
  void sink_it_(const log_msg &msg) override;
  void flush_() override;
  void backend_sink_it_(const log_msg &incoming_log_msg);
  void backend_flush_();

 private:
  std::weak_ptr<thread_pool> thread_pool_;
  async_overflow_policy overflow_policy_;
};

// Async msg to move to/from the queue
// Movable only. should never be copied
struct async_msg : log_msg_buffer {
  async_msg_type msg_type{async_msg_type::log};
  async_logger_ptr worker_ptr;

  async_msg() = default;
  ~async_msg() = default;

  // should only be moved in or out of the queue..
  async_msg(const async_msg &) = delete;

  async_msg(async_msg &&) = default;
  async_msg &operator=(async_msg &&) = default;

  // construct from log_msg with given type
  async_msg(async_logger_ptr &&worker, async_msg_type the_type,
            const log_msg &m)
      : log_msg_buffer{m}, msg_type{the_type}, worker_ptr{std::move(worker)} {}

  async_msg(async_logger_ptr &&worker, async_msg_type the_type)
      : log_msg_buffer{}, msg_type{the_type}, worker_ptr{std::move(worker)} {}

  explicit async_msg(async_msg_type the_type) : async_msg{nullptr, the_type} {}
};

class thread_pool {
 public:
  using item_type = async_msg;
  using q_type = mpmc_blocking_queue<item_type>;

  inline thread_pool(size_t q_max_items, size_t threads_n,
                     std::function<void()> on_thread_start)
      : q_(q_max_items) {
    if (threads_n == 0 || threads_n > 1000) {
      throw(
          "spdlog::thread_pool(): invalid threads_n param (valid "
          "range is 1-1000)");
    }
    for (size_t i = 0; i < threads_n; i++) {
      threads_.emplace_back([this, on_thread_start] {
        on_thread_start();
        this->thread_pool::worker_loop_();
      });
    }
  }

  inline thread_pool::thread_pool(size_t q_max_items, size_t threads_n)
      : thread_pool(q_max_items, threads_n, [] {}) {}

  // message all threads to terminate gracefully join them
  ~thread_pool() {
    try {
      for (size_t i = 0; i < threads_.size(); i++) {
        post_async_msg_(async_msg(async_msg_type::terminate),
                        async_overflow_policy::block);
      }

      for (auto &t : threads_) {
        t.join();
      }
    } catch (...) {
    }
  }

  thread_pool(const thread_pool &) = delete;
  thread_pool &operator=(thread_pool &&) = delete;

  void post_log(async_logger_ptr &&worker_ptr, const log_msg &msg,
                async_overflow_policy overflow_policy) {
    async_msg async_m(std::move(worker_ptr), async_msg_type::log, msg);
    post_async_msg_(std::move(async_m), overflow_policy);
  }

  void post_flush(async_logger_ptr &&worker_ptr,
                  async_overflow_policy overflow_policy) {
    post_async_msg_(async_msg(std::move(worker_ptr), async_msg_type::flush),
                    overflow_policy);
  }

  size_t overrun_counter() { return q_.overrun_counter(); }

 private:
  q_type q_;

  std::vector<std::thread> threads_;

  void post_async_msg_(async_msg &&new_msg,
                       async_overflow_policy overflow_policy) {
    if (overflow_policy == async_overflow_policy::block) {
      q_.enqueue(std::move(new_msg));
    } else {
      q_.enqueue_nowait(std::move(new_msg));
    }
  }

  void worker_loop_() {
    while (process_next_msg_()) {
    }
  }

  // process next message in the queue
  // return true if this thread should still be active (while no terminate msg
  // was received)
  bool process_next_msg_() {
    async_msg incoming_async_msg;
    bool dequeued =
        q_.dequeue_for(incoming_async_msg, std::chrono::seconds(10));
    if (!dequeued) {
      return true;
    }

    switch (incoming_async_msg.msg_type) {
      case async_msg_type::log: {
        incoming_async_msg.worker_ptr->backend_sink_it_(incoming_async_msg);
        return true;
      }
      case async_msg_type::flush: {
        incoming_async_msg.worker_ptr->backend_flush_();
        return true;
      }

      case async_msg_type::terminate: {
        return false;
      }

      default: {
        assert(false);
      }
    }
    return true;
  }
};

}  // namespace spdlog
