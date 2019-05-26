#ifndef _PROFILER_H_
#define _PROFILER_H_

#include <chrono>
#include <ctime>
#include <exception>
#include <iostream>
#include <memory>
#include <ratio>
#include <utility>
#include "easylogging++.h"

#ifdef WIN32
#include <Windows.h>
#include <Psapi.h>
#pragma comment(lib, "psapi.lib")
#else
#include <sys/resource>
#include <sys/time.h>
#endif

#define PROFILER_MODE

namespace LEE_DEBUG {
    namespace Profiler {
        class ProfilerInstance
        {
        public:
            typedef std::chrono::steady_clock SteadyClock;
            typedef SteadyClock::time_point TimePoint;
            typedef std::chrono::duration<double, std::ratio<1, 1>> DurationTime;//单位秒
            enum class MemoryUnit { KB_, MB_, GB_ };
        private:
#define KB / 1024
#define MB KB / 1024
#define GB MB / 1024
        private:
            static DurationTime duringTime;
            static TimePoint startTime;
            static TimePoint finishTime;
        public:
            static void start();//开始计时
            static void finish();//结束计时
            static void dumpDuringTime(std::ostream& os = std::cout);//打印时间

            static double second();//以秒为单位返回时间
            static double millisecond();//以毫秒为单位返回时间

            static size_t memory(MemoryUnit mu = MemoryUnit::KB_);//查询当前程序的内存使用量
        };
    }

    /* 打印运行时长和当前程序使用多少内存等信息
   使用说明： 只有在定义了PROFILER_MODE宏的情况下才会显示性能信息。否则只是正常的运行该函数。
   x为函数名， 逗号后面是函数x所需要的入参
   例如： 1. itoa(a, b); 改写为宏应为 PROFILER_F(itoa, a, b)
         2. find();     改写为宏应为 PROFILER_F(find);
*/
#ifdef PROFILER_MODE
#if 0
#define PROFILER_F(x, ...)  LEE_DEBUG::Profiler::ProfilerInstance::start();\
                            x##(__VA_ARGS__);\
                            LEE_DEBUG::Profiler::ProfilerInstance::finish();\
                            std::cout << "Function: " << #x << "() \n"; \
                            std::cout << "In File: " << __FILE__ << " Line: " << __LINE__ << "\n";\
                            std::cout << "Date: " << __DATE__ << " " << __TIME__<< "\n";\
                            std::cout << "Spand Time: " << LEE_DEBUG::Profiler::ProfilerInstance::millisecond() << "ms\n" ;\
                            std::cout << "Use Memory: " << LEE_DEBUG::Profiler::ProfilerInstance::memory() << "KB"; \
                            std::cout << "(" << LEE_DEBUG::Profiler::ProfilerInstance::memory(LEE_DEBUG::Profiler::ProfilerInstance::MemoryUnit::MB_) \
                                      << " MB)\n"<< std::endl;
#else
#define PROFILER_F(x, ...)  LEE_DEBUG::Profiler::ProfilerInstance::start();\
                            x##(__VA_ARGS__);\
                            LEE_DEBUG::Profiler::ProfilerInstance::finish();\
                            LOG(INFO) << "\nFunction: " << #x << "() \n" \
                            << "In File: " << __FILE__ << " Line: " << __LINE__ << "\n"\
                            << "Spand Time: " << LEE_DEBUG::Profiler::ProfilerInstance::millisecond() << "ms\n" \
                            << "Use Memory: " << LEE_DEBUG::Profiler::ProfilerInstance::memory() << "KB" \
                            << "(" << LEE_DEBUG::Profiler::ProfilerInstance::memory(LEE_DEBUG::Profiler::ProfilerInstance::MemoryUnit::MB_) \
                            << " MB)\n"<< std::endl;
#endif

#else
#define PROFILER_F(x, ...) x##(__VA_ARGS__);
#endif

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

#endif