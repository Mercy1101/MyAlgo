namespace lee {
namespace log {
enum level {
  TRACE = 1,
  DEBUG = 2,
  INFO = 3,
  WARN = 4,
  ERROR = 5,
  OFF = 6,
};
class file_helper {
 public:
  void log(const std::string& text) {}
  void flush() {}
};
class terminal_helper {
 public:
  void log(const std::string& text) {
    std::lock_guard<std::mutex> lock(mut_);
    printf(text.c_str());
    printf("\n");
  }

 private:
  std::mutex mut_;
};
class format_text_helper {
 public:
  std::string format_text(const level level, const std::string& text) {
    return create_format_log();
  }

 private:
  std::string create_format_log(const level level, const std::string& text) {
    std::string formated_text;
    add_head_text(format_text);
    add_time_text(format_text, time(nullptr));
    add_level_text(format_text, level);
    add_text(format_text, text);
    add_tail_text(format_text);
    return format_text;
  }

  void add_head_text(std::string& text) {}

  void add_time_text(std::string& text, size_t time_stamp) {
    std::string time_text(" [");
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
    time_text += tmp;  /// std::string(tmp);
    time_text += "]";
    text += time_text;
  }
  void add_level_text(std::string& text, level level) {
    std::string str(" [");
    if (level == level::TRACE) {
      str += "TRACE";
    } else if (level == level::DEBUG) {
      str += "DEBUG";
    } else if (level == level::INFO) {
      str += "INFO";
    } else if (level == level::WARN) {
      str += "WARN";
    } else if (level == level::ERROR) {
      str += "ERROR";
    }
    str += "]";
    text += str;
  }

  void add_text(std::string& text, const std::string& text1) {
    text += " ";
    text += text1;
  }
  void add_tail_text(std::string& text) {}
};

class path_helper {
 public:
  void make_path(const std::string& revaltive_path) {
    absolute_path = get_absolute_path();
  }
};
class log_implement {
 public:
  static log_implement& get_instance() {
    static log_implement* logger_ = nullptr;
    static std::once_flag flag;
    std::call_once(flag, [&]() { logger_ = new log_implement(); });
    return *logger_;
  }

 public:
  void log(const level log_level, const std::string& log_text) {
    auto text = format_.format_text(log_level, log_text);
    file_.log(text);
    if (log_level >= file_flush_level_) {
      file_.flush();
    }
    if (log_level >= cout_level_) {
      cout_.log(text);
    }

   private:
    log::file_helper file_;
    log::terminal_helper cout_;
    log::format_text_helper format_;
    log::path_helper path;
    level file_flush_level_ = level::WARN;
    level cout_level_ = level::INFO;
    log_implement() {
      /// 创建日志路径
    }
  };

}  // namespace log
}  // namespace log
