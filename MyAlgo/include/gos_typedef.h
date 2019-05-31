#ifndef MYALGO_INCLUDE_GOS_TYPEDEF_H
#define MYALGO_INCLUDE_GOS_TYPEDEF_H

#include <stdio.h>
#include <vector>
#include <map>
#include "profiler.h"

#define PROFILER_MODE
#define _OSWIN32_

#define THOU_TESTS_TIMES

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

#define VECTOR      std::vector
#define MAP         std::map
#endif

#define MOD(a, b)       ((a) % (b))

#define GET_INT(ptr)            (*((UINT32*)(ptr)))
#define SET_INT(ptr, value)     (*((UINT32*)(ptr))) = (UINT32)(value)

#define GET_INT64(ptr)          (*((INT64*)(ptr)))
#define SET_INT64(ptr, value)   (*((INT64*)(ptr))) = (INT64)(value)

#define GET_SHORT(ptr)          (*((UINT16*)(ptr)))
#define SET_SHORT(ptr, value)   (*((UINT16*)(ptr))) = (UINT16)(value)

#define IS_NULL_STRING(x)   (!(x) || *((CHAR*)(x)) == '\0')

#define DIR_SEPARATOR    '\\'
#define IS_DIR_SPLIT(c) ((c) == '/' || (c) == '\\')

namespace Lee {
    namespace GosLog {

/* 打印级别 */
#define LOG_NULL            0
#define LOG_DETAIL          1
#define LOG_INFO            2
#define LOG_WARN            3
#define LOG_ERROR           4
#define LOG_FAIL            5
#define LOG_FATAL           6
#define LOG_PROFILER        7

#define LOG_ALL             0xf0
#define LOG_DEFAULT         0xff

#define MODULE_PLAT         0xff00
#define MODULE_DTP          0xff01
#define MODULE_SYS_LOG      0xff02

#define MODULE_PLAT_NAME    "PLAT"
#define MODULE_DTP_NAME     "DTP"
#define MODULE_SYSLOG_NAME  "SYSLOG"

// #define DEFAULT_REMOTE_LOGGER_PORT      27300

    };  // end of namespace GosLog
};  // end of namespace Lee


namespace Lee {
    namespace Profiler {

/* 打印运行时长和当前程序使用多少内存等信息 
   使用说明： 只有在定义了PROFILER_MODE宏的情况下才会显示性能信息。否则只是正常的运行该函数。
   x为函数名， 逗号后面是函数x所需要的入参
   例如： 1. itoa(a, b); 改写为宏应为 PROFILER_F(itoa, a, b)
         2. find();     改写为宏应为 PROFILER_F(find);
*/
#ifdef PROFILER_MODE
#define PROFILER_F(x, ...)  Lee::Profiler::ProfilerInstance::start();\
                            x##(__VA_ARGS__);\
                            Lee::Profiler::ProfilerInstance::finish();\
                            std::cout << "Function: " << #x << "() \n"; \
                            std::cout << "In File: " << __FILE__ << " Line: " << __LINE__ << "\n";\
                            std::cout << "Date: " << __DATE__ << " " << __TIME__<< "\n";\
                            std::cout << "Spand Time: " << Lee::Profiler::ProfilerInstance::millisecond() << "ms\n" ;\
                            std::cout << "Use Memory: " << Lee::Profiler::ProfilerInstance::memory() << "KB"; \
                            std::cout << "(" << Lee::Profiler::ProfilerInstance::memory(Lee::Profiler::ProfilerInstance::MemoryUnit::MB_) \
                                      << " MB)\n"<< std::endl;
#else
#define PROFILER_F(x, ...) x##(__VA_ARGS__);
#endif

        };// end of namespace Profiler 
};// end of namespace Lee 



#endif