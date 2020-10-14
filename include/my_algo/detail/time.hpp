///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   time.h
/// @brief  关于时间的函数
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-10-02 14:16:40
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_TIME_HPP_
#define INCLUDE_DETAIL_TIME_HPP_

#include <chrono>
#include <ctime>

namespace lee {
inline namespace time {

/// @name     get_time_stamp
/// @brief    获取当前电脑桌面时间的时间戳(距离1970年元月1日零点到现在的秒数)
///
/// @param    NONE
///
/// @return   当前时间的时间戳
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-11-05 09:44:32
/// @warning  线程安全
inline time_t get_time_stamp() noexcept {
  auto current_time_s = std::chrono::duration_cast<std::chrono::seconds>(
                            std::chrono::system_clock::now().time_since_epoch())
                            .count();
  return current_time_s;
}

/// @name     get_milli_time_stamp
/// @brief    获取当前电脑桌面时间的时间戳(距离1970年1月1日零点到现在的毫秒数)
///
/// @param    NONE
///
/// @return   当前的时间戳
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-06 10:49:06
/// @warning  线程不安全
inline time_t get_milli_time_stamp() noexcept {
  auto current_time_s = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::system_clock::now().time_since_epoch())
                            .count();
  return current_time_s;
}

/// @name     get_cpu_milli_second
/// @brief    获取距离当前程序启动的毫秒数
///
/// @param    NONE
///
/// @return   毫秒数(clock_t类型是足以表示进程所用的处理器时间的实数类型)
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-01 20:15:41
/// @warning  线程不安全
inline clock_t get_cpu_milli_second() noexcept {
  return (std::clock() * 1000 / CLOCKS_PER_SEC);
}

/// @name     get_time_string
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
///     std::cout << lee::get_time_string(1571290440) << std::endl;
///     输出：2019-10-17 13:34:00
inline std::string get_time_string(time_t Time, const char *Format) {
  if (Time < 0) {
    throw std::logic_error("invalid time param!");
    Time = 0;
  }

  time_t t = Time;
  tm buf;
#ifdef _WIN32
  localtime_s(&buf, &t);
#else
  localtime_r(&t, &buf);
#endif
  char p[32] = {0};
  strftime(p, sizeof(p), Format, &buf);
  return std::string(p);
}
inline std::string get_time_string(time_t Time) {
  return get_time_string(Time, "%F %T");
}
inline std::string get_time_string(const char *Format) {
  return get_time_string(lee::get_time_stamp(), Format);
}
inline std::string get_time_string() {
  return get_time_string(lee::get_time_stamp(), "%F %T");
}

/// @name     get_compile_time
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
inline time_t get_compile_time() {
  static std::once_flag InstanceFlag;
  static time_t compile_time_s = 0;  ///< 单位为秒
  std::call_once(InstanceFlag, []() {
    tm tm = {0};
    char Mmm[4] = {0};
    strncpy(Mmm, "Jan", sizeof(Mmm));
    sscanf(__DATE__, "%3s %d %d", Mmm, &tm.tm_mday, &tm.tm_year);

    std::string Month(Mmm);
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

    sscanf(__TIME__, "%d:%d:%d", &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
    /** 修正参数 */
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    compile_time_s = mktime(&tm);
    if (compile_time_s <= 0 || compile_time_s > lee::get_time_stamp()) {
      throw std::logic_error("error time param!");
      compile_time_s = -1;
    }
  });  /// std::call_once()
  return compile_time_s;
}

/// @name     get_today_specific_time_stamp
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
inline time_t get_today_specific_time_stamp(const int iHour, const int iMin,
                                            const int iSec) {
  if (iHour < 0 || iHour > 23 || iMin < 0 || iMin > 59 || iSec < 0 ||
      iSec > 59) {
    throw std::logic_error("invalid time param!");
  }

  time_t t = ::time(nullptr);
  tm stTimer = {0};
#ifdef _WIN32
  localtime_s(&stTimer, &t);
#else
  localtime_r(&t, &stTimer);
#endif
  stTimer.tm_hour = iHour;
  stTimer.tm_min = iMin;
  stTimer.tm_sec = iSec;
  return mktime(&stTimer);
}

/// @name     get_specific_timestamp
/// @brief    获取特定日期的时间戳
///
/// @param    year  [in]
/// @param    mouth [in]
/// @param    day   [in]
/// @param    hour  [in]
/// @param    min   [in]
/// @param    sec   [in]
///
/// @return   特定日期时间戳
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-08-02 15:47:05
/// @warning  线程不安全
inline time_t get_specific_timestamp(const int year, const int mouth,
                                     const int day, const int hour,
                                     const int min, const int sec) {
  tm time = {0};
  time.tm_year = year - 1900;
  time.tm_mon = mouth - 1;
  time.tm_mday = day;
  time.tm_hour = hour;
  time.tm_min = min;
  time.tm_sec = sec;

  return mktime(&time);
}

/// @name     get_weekday
/// @brief    获取某特定日期是周几
///
/// @param    year  [in]
/// @param    mouth [in]
/// @param    day   [in]
///
/// @return   周几
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2020-06-25 08:55:20
/// @warning  线程不安全
inline std::optional<int> get_weedday(const int year, const int mouth,
                                      const int day) {
  if (mouth < 1 || mouth > 12 || day < 1 || day > 31) {
    return {};
  }

  int c = year / 100;
  int y = year % 100;
  int m = mouth;
  if (year <= 1582 && mouth <= 10 && day < 4) {
    return y + y / 4 + c / 4 - 2 * c + (13 * (m + 1)) / 5 + day + 2;
  } else {
    return (c / 4 - 2 * c + y + y / 4 + (13 * (m + 1)) / 5 + day - 1) % 7;
  }
}
}  // namespace time
}  // namespace lee
#endif
