/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.

///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
///
/// @file   time_utility.h
/// @brief  时间相关的工具集
///
/// @author lijiancong, pipinstall@163.com
/// @date   2019-11-06 10:12:52
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_TIME_UTILITY_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_TIME_UTILITY_H_

#include <stdio.h>
#include <time.h>

#include <cassert>
#include <chrono>
#include <ctime>
#include <limits>     // for std::numeric_limits()
#include <mutex>      // for std::call_once()
#include <typeindex>  // for std::typeindex()
#include <typeinfo>   // for typeid

#include "utility/detail/marco_utility.h"  // for Lee::Second


namespace Lee {
inline namespace utility {
inline namespace time {
/// @name     GetCurrentTimeStamp
/// @brief    获取当前电脑桌面时间的时间戳(距离1970年元月1日零点到现在的秒数)
///
/// @param    NONE
///
/// @return   当前时间的时间戳
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-11-05 09:44:32
/// @warning  线程安全
inline Lee::Second GetCurrentTimeStamp() noexcept {
  auto current_time_s = std::chrono::duration_cast<std::chrono::seconds>(
                            std::chrono::system_clock::now().time_since_epoch())
                            .count();
#undef max  /// 这句话视为了取消window自带的max宏定义,让下面这句话能编译
  if (current_time_s >
      std::numeric_limits<Lee::Second>::max() -
          static_cast<decltype(current_time_s)>(30 * 24 * 60 * 60)) {
    //    assert(false &&
    //           "Excute GetCurrentTimeStamp() failed!(current_time_s
    //           overflow!)");
    std::cout
        << "Excute GetCurrentTimeStamp() failed!(current_time_s overflow!)"
        << std::endl;
    return -1;
  }
  return static_cast<Lee::Second>(current_time_s);
}

/// @name     GetCurrentMilliSecondStamp
/// @brief    获取当前电脑桌面时间的时间戳(距离1970年1月1日零点到现在的毫秒数)
///
/// @param    NONE
///
/// @return   当前的时间戳
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-06 10:49:06
/// @warning  线程不安全
inline Lee::MilliSecond GetCurrentMilliSecondStamp() noexcept {
  auto current_time_s = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::system_clock::now().time_since_epoch())
                            .count();
#undef max  /// 这句话视为了取消window自带的max宏定义,让下面这句话能编译
  if (current_time_s >
      std::numeric_limits<Lee::MilliSecond>::max() -
          static_cast<decltype(current_time_s)>(30 * 24 * 60 * 60)) {
    ///    assert(false &&
    ///           "Excute GetCurrentMilliSecondStamp() failed!(current_time_s "
    ///           "overflow!)");
    std::cout << "Excute GetCurrentMilliSecondStamp() failed!(current_time_s "
                 "overflow!)"
              << std::endl;
    return -1;
  }
  return static_cast<Lee::MilliSecond>(current_time_s);
}

/// @name     GetCPUMilliSecond
/// @brief    获取距离当前程序启动的毫秒数
///
/// @param    NONE
///
/// @return   毫秒数(clock_t类型是足以表示进程所用的处理器时间的实数类型)
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-01 20:15:41
/// @warning  线程不安全
inline clock_t GetCPUMilliSecond() noexcept {
  return (std::clock() * 1000 / CLOCKS_PER_SEC);
}

/// @name     GetTimeString
/// @brief    用于传入时间戳，返回格式化后的时间。
///
/// @param    Time [in]    时间戳
///
/// @return   格式化后的时间
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-11-05 14:05:04
/// @warning  线程不安全
/// @example:
///     std::cout << Lee::GetTimeString(1571290440) << std::endl;
///     输出：2019-10-17 13:34:00
inline std::string GetTimeString(Lee::Second Time, const char *Format) {
  if (Time < 0) {
    /// assert(false && "GetTimeString Param Time is invalid!");
    std::cout << "GetTimeString Param Time is invalid!" << std::endl;
    Time = 0;
  }

  time_t t = Time;
  tm buf;
  localtime_s(&buf, &t);
  char p[32] = {0};
  strftime(p, sizeof(p), Format, &buf);
  return std::string(p);
}
inline std::string GetTimeString(Lee::Second Time) {
  return GetTimeString(Time, "%F %T");
}
inline std::string GetTimeString(const char *Format) {
  return GetTimeString(Lee::GetCurrentTimeStamp(), Format);
}
inline std::string GetTimeString(Lee::MilliSecond Time) {
  return GetTimeString(static_cast<Lee::Second>(Time / 1000), "%F %T");
}
inline std::string GetTimeString(Lee::MilliSecond Time, const char *Format) {
  return GetTimeString(static_cast<Lee::Second>(Time / 1000), Format);
}
inline std::string GetTimeString() {
  return GetTimeString(Lee::GetCurrentTimeStamp(), "%F %T");
}

/// @name     GetCompileTimeStamp
/// @brief    获取编译时刻的时间戳
///           (多用于时间戳大小的有效性的判断,运行时获取的时间戳不会比编译时间戳大)
///
/// @param    NONE
///
/// @return   编译时刻的时间戳
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-11-06 10:07:38
/// @warning  线程安全
inline Lee::Second GetCompileTimeStamp() {
  static std::once_flag InstanceFlag;
  static time_t compile_time_s = 0;  ///< 单位为秒
  std::call_once(InstanceFlag, []() {
    tm tm = {0};
    char Mmm[4] = {0};
    strcpy_s(Mmm, sizeof(Mmm), "Jan");
    sscanf_s(__DATE__, "%3s %d %d", Mmm, static_cast<unsigned>(sizeof(Mmm)),
             &tm.tm_mday, &tm.tm_year);

    std::string Month(Mmm);
    // std::reverse(Month.begin(), Month.end());
    if (Month.find("Jan") != std::string::npos) {
      tm.tm_mon = 1;
    } else if (Month.find("Feb") != std::string::npos) {
      tm.tm_mon = 2;
    } else if (Month.find("Mar") != std::string::npos) {
      tm.tm_mon = 3;
    } else if (Month.find("Apr") != std::string::npos) {
      tm.tm_mon = 4;
    } else if (Month.find("May") != std::string::npos) {
      tm.tm_mon = 5;
    } else if (Month.find("Jun") != std::string::npos) {
      tm.tm_mon = 6;
    } else if (Month.find("Jul") != std::string::npos) {
      tm.tm_mon = 7;
    } else if (Month.find("Aug") != std::string::npos) {
      tm.tm_mon = 8;
    } else if (Month.find("Sep") != std::string::npos) {
      tm.tm_mon = 9;
    } else if (Month.find("Oct") != std::string::npos) {
      tm.tm_mon = 10;
    } else if (Month.find("Nov") != std::string::npos) {
      tm.tm_mon = 11;
    } else if (Month.find("Dec") != std::string::npos) {
      tm.tm_mon = 12;
    } else {
      tm.tm_mon = 0;
    }

    sscanf_s(__TIME__, "%d:%d:%d", &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
    /** 修正参数 */
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    compile_time_s = mktime(&tm);
    if (compile_time_s <= 0 || compile_time_s > Lee::GetCurrentTimeStamp()) {
      /// assert(false && "GetCompileTimeStamp() is failed!");
      std::cout << "GetCompileTimeStamp() is failed!" << std::endl;
      compile_time_s = -1;
    }
  });  /// std::call_once()
  return static_cast<Lee::Second>(compile_time_s);
}

/// @name     GetTodaySpecificTimeStamp_Agent
/// @brief    获取今天特定时间的时间戳
///
/// @param    iHour    [in]
/// @param    iMin     [in]
/// @param    iSec     [in]
///
/// @return   返回的今天特定时间的时间戳
///  @retval  0            非法入参
///  @retval  UnixStamp    今天特定时间的时间戳
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-08-20 16:11:44
/// @warning  线程不安全
inline Lee::Second GetTodaySpecificTimeStamp(const int iHour, const int iMin,
                                             const int iSec) {
  if (iHour < 0 || iHour > 23 || iMin < 0 || iMin > 59 || iSec < 0 ||
      iSec > 59) {
    /// assert(false && "Invalid Param! GetTodaySpecificTimeStamp Execute fail!
    /// ");
    std::cout << "Invalid Param! GetTodaySpecificTimeStamp Execute fail! "
              << std::endl;
    return 0;
  }

  time_t t = ::time(nullptr);
  tm stTimer = {0};
  localtime_s(&stTimer, &t);
  stTimer.tm_hour = iHour;
  stTimer.tm_min = iMin;
  stTimer.tm_sec = iSec;
  return static_cast<int>(mktime(&stTimer));
}

}  // namespace time
}  // namespace utility
}  // namespace Lee

#endif  // end of MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_TIME_UTILITY_H_
