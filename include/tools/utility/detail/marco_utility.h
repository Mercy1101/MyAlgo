/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.

///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
///
/// @file   marco_utility.h
/// @brief  放置一些关于宏定义的小工具
///
/// @author lijiancong, pipinstall@163.com
/// @date   2019-12-01 17:15:24
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_MARCO_UTILITY_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_MARCO_UTILITY_H_

#include <string>
#include <vector>

#include "concurrentqueue/concurrentqueue.h"

// A macro to disallow operator=
// This should be used in the private: declarations for a class.
#define MYALGO_DISALLOW_ASSIGN_(type) void operator=(type const &) = delete

// A macro to disallow copy constructor and operator=
// This should be used in the private: declarations for a class.
#define MYALGO_DISALLOW_COPY_AND_ASSIGN_(type) \
  type(type const &) = delete;                 \
  MYALGO_DISALLOW_ASSIGN_(type)

/// 定义std::lock_guard的简写
#define LOCK_GUARD(type) std::lock_guard<std::mutex> LoCk__(type)

/// 定义一个布尔值转字符串的宏
#define BOOL_TO_STRING(type) ((type) ? "true" : "false")

#define MYALGO_CATCH_AND_WRAP              \
  catch (const std::exception &e) {        \
    LOG(LOG_ERROR, "Error: %s", e.what()); \
  }

namespace Lee {
inline namespace utility {
inline namespace marco {
/// 长度单位定义
using KiloMeter = int;   ///< 千米
using Meter = int;       ///< 米
using Millimeter = int;  ///< 毫米
/// 重量单位定义
using Kilogram = int;  ///< 千克
using Gram = int;      ///< 克
/// 时间单位定义
using Hour = int;               ///< 时
using Minute = int;             ///< 分
using Second = int;             ///< 秒
using MilliSecond = long long;  ///< 毫秒

/// 定义无锁多生产者多消费者线程安全的队列的别名
template <typename T>
using CQueue = moodycamel::ConcurrentQueue<T>;

/** 默认的配置文件放置的路径（根目录下的文件夹名） */
const std::string DEFAULT_CONFIG_FOLDER_NAME = "config";
/** 默认的配置文件名 */
const std::string DEFAULT_CONFIG_FILE_NAME = "conf.ini";
/** 默认日志根文件夹 */
const std::string DEFAULT_LOG_FILE_FOLDER = "log";

}  // namespace marco
}  // namespace utility
}  // namespace Lee

#endif  // end of MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_MARCO_UTILITY_H_
