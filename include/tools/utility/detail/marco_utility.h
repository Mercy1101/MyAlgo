/// Copyright (c) 2019 Lijiancong. All rights reserved.
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

// A macro to disallow operator=
// This should be used in the private: declarations for a class.
#define MYALGO_DISALLOW_ASSIGN_(type) void operator=(type const &) = delete

// A macro to disallow copy constructor and operator=
// This should be used in the private: declarations for a class.
#define MYALGO_DISALLOW_COPY_AND_ASSIGN_(type) \
  type(type const &) = delete;                 \
  MYALGO_DISALLOW_ASSIGN_(type)

namespace Lee {
inline namespace marco_utility {
/// 长度单位定义
using KiloMeter = int;   ///< 千米
using Meter = int;       ///< 米
using Millimeter = int;  ///< 毫米
/// 重量单位定义
using Kilogram = int;  ///< 千克
using Gram = int;      ///< 克
/// 时间单位定义
using Hour = int; ///< 时
using Minute = int; ///< 分
using Second = int;             ///< 秒
using MilliSecond = long long;  ///< 毫秒

/// lock_guard的缩写定义
using scoped_lock = std::lock_guard<std::mutex>;

/** 默认的配置文件放置的路径（根目录下的文件夹名） */
const std::string DEFAULT_CONFIG_FOLDER_NAME = "config";
/** 默认的配置文件名 */
const std::string DEFAULT_CONFIG_FILE_NAME = "conf.ini";
/** 默认日志根文件夹 */
const std::string DEFAULT_LOG_FILE_FOLDER = "log";


}  // namespace marco_utility
}  // end of namespace Lee

#endif  // end of MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_MARCO_UTILITY_H_
