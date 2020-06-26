///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   async_log_utility.h
/// @brief  异步log模块的实现
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-06-26 13:51:57
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_ASYNC_LOG_UTILITY_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_ASYNC_LOG_UTILITY_H_

#include <filesystem>
#include <mutex>
#include <queue>
#include <string>

namespace lee {
inline namespace utility {
inline namespace async_log {

/// @name     日志的基类
/// @brief    定义一个基类方便工厂模式
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-06-26 14:16:16
/// @warning  线程不安全
class base_log {
  public： virtual void write_log() = 0;
  virtual ~base_log() = default;
};

/// @name     async_log
/// @brief    实现一个异步的日志模块
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-06-26 13:53:11
/// @warning  线程不安全
class async_log : public base_log {
 public:
  async_log() { log_queue_.reseve(2000); }

  virtual ~async_log() = default;

  /// @name     write_log
  /// @brief    用于创建写日志
  ///
  /// @param    log_text  [in] 需要些的内容
  ///
  /// @return   NONE
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2020-06-26 14:10:57
  /// @warning  线程不安全
  virtual void write_log(const LEVEL& level, std::string& log_text) override {
    static std::mutex mux;
    std::lock_guard<std::mutex> lock(mux);
    log_queue_.second.push_back(log_text);
    output_command_line(level, log_text);

    write_file(level, log_text);
  }

 private:
  std::pair<std::mutex, std::queue<std::string>> log_text_;  ///< 日志队列
  const std::string current_log_name_ = "detail_1.log";  ///< 当前日志文件名称
  const std::string default_log_root_path_ =
      "/log/detail/";  ///< 默认日志文件夹
  const LEVEL default_command_line_level_ = LEVEL::INFO;  ///< 命令行打印等级
  const LEVEL default_write_file_level_ = LEVEL::WARN;  ///< 写入文件的刷新等级

  void write_file(const LEVEL& level) {
    if (is_flush_file_now(level) || log_text_.second.size() >= 100) {
      if(!log_text_.first.is_lock(){
        std::lock_guard<std::mutex> lock(log_text_.first);
        std::vector<std::string> vec;
        vec.reserve(100);
        while (!log_text_.second.empty()) {
          vec.emplace_back(log_text_.second.front());
          log_text_.second.pop();
        }
        std::async(std::launch::async, write_file_imp, std::move(vec));
      }
    } else {
      return;
    }
  }

  void write_file_imp(std::vector<std::string>&& vec) {
    std::ofstream file(std::filesystem::current_path().generic_string() +
                       default_log_root_path_ + current_log_name_);
    if (!file) {
      throw "open file failed!";
    }

    for(const auto& it : vec){
      file << it << '\n';
    }

    return;
  }

  /// @brief    output_command_line
  ///
  /// @param    level [in]
  ///
  /// @return
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2020-06-26 14:58:08
  /// @warning  线程不安全
  void output_command_line(const LEVEL& level) {
    if (is_output_command_line(level)) {
      std::cout << log_queue_.back() << '\n';
    } else {
      return;
    }
  }

  /// @name     is_print_command_line
  /// @brief    判断是否打印输出至命令行
  ///
  /// @param    level [in]
  ///
  /// @return   true or false
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2020-06-26 15:00:59
  /// @warning  线程不安全
  bool is_output_command_line(const LEVEL& level) {
    return level >= default_command_line_level_;
  }

  /// @name     is_flush_file_now
  /// @brief    判断是否需要马上进行写文件
  ///
  /// @param    level [in]
  ///
  /// @return   true or false
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2020-06-26 15:02:12
  /// @warning  线程不安全
  bool is_flush_file_now(const LEVEL& level) {
    return level >= default_write_file_level_;
  }

 private:
  /// @name     日志等级枚举体
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2020-06-26 14:13:52
  /// @warning  线程不安全
  enum class LEVEL : int {
    TRACE = 3,
    DEBUG = 8,
    INFO = 13,
    WARN = 18,
    ERROR = 23,
    CRITICAL = 28,
  }
};
}  // namespace async_log
}  // namespace utility
}  // namespace lee
#endif