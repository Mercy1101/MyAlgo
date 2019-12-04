/// Copyright (c) 2019 Lijiancong. All rights reserved.
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
#include <ctime>
#include <mutex>  // for std::call_once()

namespace Lee {
inline namespace Utility_ {
inline namespace Time_ {

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
inline int GetCurrentTimeStamp() noexcept {
  return static_cast<int>(time(nullptr));
}

/// @name     GetCurrentMilliSecond
/// @brief    获取距离当前程序启动的毫秒数
///
/// @param    NONE
///
/// @return   毫秒数(clock_t类型是足以表示进程所用的处理器时间的实数类型)
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-01 20:15:41
/// @warning  线程不安全
inline clock_t GetCurrentMilliSecond() noexcept {
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
///     std::cout << GetTimeString(1571290440) << std::endl;
///     输出：2019-10-17 13:34:00
inline std::string GetTimeString(time_t Time, const char *Format = "%F %T") {
  if (Time < 0) {
    assert(false && "GetTimeString Param Time is invalid!");
    Time = 0;
  }

  time_t t = Time;
  tm buf;
  localtime_s(&buf, &t);
  char p[32] = {0};
  strftime(p, sizeof(p), Format, &buf);
  return std::string(p);
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
inline time_t GetCompileTimeStamp() {
  static std::once_flag InstanceFlag;
  static time_t CompileTime = 0;
  std::call_once(InstanceFlag, []() {
    tm tm = {0};
    char Mmm[4] = "Jan";
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
    CompileTime = mktime(&tm);
  });
  return CompileTime;
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
inline int GetTodaySpecificTimeStamp(const int iHour, const int iMin,
                                     const int iSec) {
  if (iHour < 0 || iHour > 23 || iMin < 0 || iMin > 59 || iSec < 0 ||
      iSec > 59) {
    assert(false && "Invalid Param! GetTodaySpecificTimeStamp Execute fail! ");
    return 0;
  }

  time_t t = time(nullptr);
  tm stTimer = {0};
  localtime_s(&stTimer, &t);
  stTimer.tm_hour = iHour;
  stTimer.tm_min = iMin;
  stTimer.tm_sec = iSec;
  return static_cast<int>(mktime(&stTimer));
}

}  // end of namespace Time_
}  // end of namespace Utility_
}  // end of namespace Lee

#endif  // end of MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_TIME_UTILITY_H_
