///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   utility.h
/// @brief  工具集
///
/// @author lijiancong, pipinstall@163.com
/// @date   2019-12-01 17:06:41
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_UTILITY_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_UTILITY_H_

#include "utility/detail/algorithm_utility.h"  /// 包含algorthm相关的小工具
#include "utility/detail/concurrency_utility.h"  /// 包含有关线程的小工具
#include "utility/detail/detail_utility.h"  /// 包含杂项相关的小工具
/// #include "utility/detail/file_line_reader_utility.h"  /// 包含按行读取的文件
#include "utility/detail/marco_utility.h"  /// 包含相关宏定义的小工具
#include "utility/detail/prime_utility.h"  /// 包含关于素数的函数
#include "utility/detail/random_utility.h"  /// 包含与随机数相关的小工具
#ifdef _WIN32
#include "utility/detail/system_utility.h"  /// 包含系统相关的小工具
#else
/// 这里应该添加linux版本的系统函数头文件
#endif
#include "utility/detail/time_utility.h"   /// 包含time相关的小工具
#include "utility/thirdparty/cxxopts.hpp"  /// 包含命令行解析的小工具
#include "utility/thirdparty/debug_assert.hpp"  /// 包含assert小工具

#endif  // end of MYALGO_INCLUDE_TOOLS_UTILITY_UTILITY_H_
