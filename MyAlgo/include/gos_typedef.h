#ifndef MYALGO_INCLUDE_GOS_TYPEDEF_H
#define MYALGO_INCLUDE_GOS_TYPEDEF_H

#include "profiler.h"

#define PROFILER_MODE
#define _OSWIN32_

#define THOU_TESTS_TIMES

namespace LEE_DEBUG
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


#define BOOL    int
#define TRUE    1
#define FALSE   0

#define UINT64          unsigned long long
#define INT64           long long

#define UINT32  unsigned int
#define INT32   int

#define UINT16  short
#define UINT8   unsigned char

#define CHAR    char
#define VOID    void


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)   sizeof(x)/sizeof((x)[0])
#endif

#define MOD(a, b)       ((a) % (b))

#define GET_INT(ptr)            (*((UINT32*)(ptr)))
#define SET_INT(ptr, value)     (*((UINT32*)(ptr))) = (UINT32)(value)

#define GET_INT64(ptr)          (*((INT64*)(ptr)))
#define SET_INT64(ptr, value)   (*((INT64*)(ptr))) = (INT64)(value)

#define GET_SHORT(ptr)          (*((UINT16*)(ptr)))
#define SET_SHORT(ptr, value)   (*((UINT16*)(ptr))) = (UINT16)(value)


/* 打印运行时长和当前程序使用多少内存等信息 
   使用说明： 只有在定义了PROFILER_MODE宏的情况下才会显示性能信息。否则只是正常的运行该函数。
   x为函数名， 逗号后面是函数x所需要的入参
   例如： 1. itoa(a, b); 改写为宏应为 PROFILER_F(itoa, a, b)
         2. find();     改写为宏应为 PROFILER_F(find);
*/
#ifdef PROFILER_MODE
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
#define PROFILER_F(x, ...) x##(__VA_ARGS__);
#endif


#endif