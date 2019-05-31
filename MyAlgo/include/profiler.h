#ifndef _PROFILER_H_
#define _PROFILER_H_

#include <chrono>
#include <ctime>
#include <exception>
#include <iostream>
#include <memory>
#include <ratio>
#include <utility>

#ifdef WIN32
#include <Windows.h>
#include <Psapi.h>
#pragma comment(lib, "psapi.lib")
#else
#include <sys/resource>
#include <sys/time.h>
#endif

namespace Lee
{

/// 32位系统int取值范围         ：-2^31 ~ 2^31-1（2147483647~-2147483648, 约21亿）
/// 32位系统unsigned int取值范围：0 ~ 2^23 - 1 (0~4294967296, 约42亿)
enum NUMBER_E
{
    Thousand_ = 1000,               // 千
    Ten_Thousands_ = 10000,         // 万
    Hundred_Thousands_ = 100000,    // 十万
    Million_ = 1000000,             // 百万
    Ten_Million_ = 10000000,        // 千万
    Hundred_Million_ = 10000000,    // 亿
    Billion_ = 100000000,           // 十亿
    INT_MAX_NUM_ = 2147483647,      // 32位系统int上限（约21亿）
    //UINT_MAX_NUM_ = 4294967296,     // 32位系统int上限（约21亿）
};

}

namespace Lee{
    namespace Profiler{

        class ProfilerInstance
        {
        public:
            typedef std::chrono::steady_clock SteadyClock;
            typedef SteadyClock::time_point TimePoint;
            typedef std::chrono::duration<double, std::ratio<1, 1>> DurationTime;  // 单位秒.
            enum class MemoryUnit{KB_, MB_, GB_};
        private:
            #define KB / 1024
            #define MB KB / 1024
            #define GB MB / 1024
        private:
            static DurationTime duringTime;
            static TimePoint startTime;
            static TimePoint finishTime;
        public:
            static void start();  // 开始计时
            static void finish();  // 结束时间
            static void dumpDuringTime(std::ostream& os = std::cout);  // 打印时间

            static double second();  // 以秒为单位返回时间
            static double millisecond();  // 以毫秒为单位返回时间

            static size_t memory(MemoryUnit mu = MemoryUnit::KB_);  // 查询当前程序的内存使用量
        };
    }
}

#endif