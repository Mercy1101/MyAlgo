/** 
 * Copyright (c) 2019 Lijiancong. All rights reserved.
 * 
 * Use of this source code is governed by a MIT license
 * that can be found in the License file.
 */

///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// 
/**
* @file                 time_utility.h
* @brief                时间相关的工具集

* @author               lijiancong
* @date                 2019-11-06 10:12:52

* @note
*/
///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// ///////// 
#pragma once
#ifndef PLAT_TOOLS_UTILITY_TIME_UTILITY
#define PLAT_TOOLS_UTILITY_TIME_UTILITY

#include <ctime>
#include <stdio.h>
#include <time.h>
#include <cassert>
#include <mutex>    // for std::call_once()

namespace Lee { namespace Utility_{ namespace Time_{
    /**
     * @name                GetCurrentTimeStamp
     * @brief               获取当前电脑桌面时间的时间戳(距离1970年元月1日零点到现在的秒数)
     * 
     * @param               NONE
     * 
     * @return              当前时间的时间戳
     * 
     * @author              Lijiancong, pipinstall@163.com
     * @date                2019-11-05 09:44:32
     * @warning             线程安全

     * @note
    */
    inline int GetCurrentTimeStamp() noexcept { return static_cast<int>(time(nullptr)); }

    /**
     * @name                GetCurrentMilliSecond
     * @brief               获取距离当前程序启动的毫秒数
     * 
     * @param               NONE
     * 
     * @return              毫秒数(clock_t类型是足以表示进程所用的处理器时间的实数类型)
     * 
     * @author              Lijiancong, pipinstall@163.com
     * @date                2019-11-05 13:32:02
     * @warning             线程不安全

     * @note
     */
    inline clock_t GetCurrentMilliSecond() noexcept { return (std::clock() * 1000 / CLOCKS_PER_SEC); }

    /**
    * @name                GetTimeString
    * @brief               用于传入时间戳，返回格式化后的时间。
    
    * @param               Time [in]    时间戳
    
    * @return              格式化后的时间
    * 
    * @author              Lijiancong, pipinstall@163.com
    * @date                2019-11-05 14:05:04
    * @warning             线程不安全

    * @note
    * @example:
        std::cout << GetTimeString(1571290440) << std::endl;
        输出：2019-10-17 13:34:00
    */
    inline std::string GetTimeString(time_t Time, const char *Format = "%F %T")
    {
        if (Time < 0)
        {
            assert(false && "GetTimeString Param Time is invalid!");
            Time = 0;
        }

        time_t t = Time;
        tm buf;
        localtime_s(&buf, &t);
        char p[32] = { 0 };
        strftime(p, sizeof(p), Format, &buf);
        return std::string(p);
    }

    /**
     * @name                GetCompileTimeStamp
     * @brief               获取编译时刻的时间戳(多用于时间戳大小的有效性的判断,运行时获取的时间戳不会比编译时间戳大)
     * 
     * @param               NONE
     * 
     * @return              编译时刻的时间戳
     * 
     * @author              Lijiancong, pipinstall@163.com
     * @date                2019-11-06 10:07:38
     * @warning             线程安全

    * @note
    */
    inline time_t GetCompileTimeStamp()
    {
        static std::once_flag InstanceFlag;
        static time_t CompileTime = 0;
        std::call_once(InstanceFlag, [](){
            tm tm = {0};
            char Mmm[4] = "Jan";
            sscanf_s(__DATE__, "%3s %d %d", Mmm, static_cast<unsigned>(sizeof(Mmm)), &tm.tm_mday, &tm.tm_year);
            Mmm[3] = Mmm[2]; Mmm[2] = Mmm[0]; Mmm[0] = Mmm[3]; Mmm[3] = 0;

            switch (*(int*)Mmm)
            {
                case (int)('Jan') : tm.tm_mon = 1; break;
                case (int)('Feb') : tm.tm_mon = 2; break;
                case (int)('Mar') : tm.tm_mon = 3; break;
                case (int)('Apr') : tm.tm_mon = 4; break;
                case (int)('May') : tm.tm_mon = 5; break;
                case (int)('Jun') : tm.tm_mon = 6; break;
                case (int)('Jul') : tm.tm_mon = 7; break;
                case (int)('Aug') : tm.tm_mon = 8; break;
                case (int)('Sep') : tm.tm_mon = 9; break;
                case (int)('Oct') : tm.tm_mon = 10; break;
                case (int)('Nov') : tm.tm_mon = 11; break;
                case (int)('Dec') : tm.tm_mon = 12; break;
                default:
                    tm.tm_mon = 0;
                    break;
            }
            sscanf_s(__TIME__, "%d:%d:%d", &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
            /** 修正参数 */
            tm.tm_year -= 1900;
            tm.tm_mon -= 1;
            CompileTime = mktime(&tm);
        });
        return CompileTime;
    }

    /**
    * @name                GetTodaySpecificTimeStamp_Agent
    * @brief               获取今天特定时间的时间戳

    * @param               iHour    [in]
    * @param               iMin     [in]
    * @param               iSec     [in]

    * @return              返回的今天特定时间的时间戳
    *  @retval             0            非法入参
    *  @retval             UnixStamp    今天特定时间的时间戳

    * @author              Lijiancong, 316, lijiancong@fritt.com.cn
    * @date                2019-08-20 16:11:44

    * @note
    */
    inline time_t GetTodaySpecificTimeStamp(const int iHour, const int iMin, const int iSec)
    {
        if (iHour < 0 || iHour > 23 || iMin < 0 || iMin > 59 || iSec < 0 || iSec > 59)
        {
            assert(false && "Invalid Param! GetTodaySpecificTimeStamp Execute fail! ");
            return 0;
        }

        time_t t = time(nullptr);
        tm     stTimer = { 0 };
        localtime_s(&stTimer, &t);
        stTimer.tm_hour = iHour;
        stTimer.tm_min = iMin;
        stTimer.tm_sec = iSec;
        return mktime(&stTimer);
    }

}   // end of namespace Time_
using namespace Time_;
}   // end of namespace Utility_
using namespace Utility_;
}   // end of namespace Lee

#endif  // End of PLAT_TOOLS_UTILITY_TIME_UTILITY