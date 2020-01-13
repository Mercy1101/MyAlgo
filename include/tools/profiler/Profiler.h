/**
 * Copyright (c) 2019 Lijiancong. All rights reserved.
 * Use of this source code is governed by a MIT license
 * that can be found in the License file.
 */

///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/**
* @file                 Profiler.h
* @brief                这个文件主要写了Profiler的内容，
                        包括了函数时间的检测和内存使用情况的检测

* @details              这个文件里定义了作用域，Lee::Profiler,
                        这个作用域里是Profiler的主要实现

* @author               lijiancong
* @date                 2019-06-21 09:50:54

* @note
1. 文件中包含头文件windows.h(#include <windows.h>),
这个头文件有版本问题.包含的前面需要先定义一个宏(WIN32_LEAN_AND_MEAN)

2. 如果想不适用PROFILER_F这个宏，可以定义这个类ProfilerInstance。
然后调用调用下面的start()和finish()函数，
然后通过调用second()或者millisecond()就可以得到从调用start()到finish()过去了多少时间。
调用memory()就可以获得当前程序占用多少内存。

3. 设计思路：

* @warning
1. 注意GosLog配置的打印等级，如果打印等级没有设置为PROFILER及以上则不会出现打印.

2. 由于PROFILER_F宏使用GosLog函数来输出和打印，
所以该函数应该在GosLog初始化后被调用，否则没有打印。

3. 为了不让PROFILER不影响Release版本的运行，
下方定义了LEE_PROFILER_MODE宏来作为Profiler的开关。
_DEBUG这个宏在VS2010中是在DEBUG模式下被自动定义的，C++Builder中则需要手动设置。

* @example
1.想要知道某个函数运行时间
原语句：
void SomeFunc(int i)
{
    printf("%d", i);
}
现在写为：
void SomeFunc(int i)
{
    PROFILER_F();   //
写在函数定义的第一行，当代码运行至函数定义结束时，打印运行时间 printf("%d", i);
}

2.想要知道某段代码运行时间
原语句:
void SomeFunc(int i)
{
    i++;    // 想要知道这三行代码运行时间
    i++;    // 想要知道这三行代码运行时间
    i++;    // 想要知道这三行代码运行时间

    printf("%d", i);    // 不想计算该行的时间
}
现写为:
void SomeFunc(int i)
{
    {
        PROFILER_F();   //
这个宏与下面三行代码写在一个花括号内，并写在花括号开始第一行 i++; i++; i++;
    }

    printf("%d", i);
}
*/
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef TOOLS_PROFILER_PROFILER_H_
#define TOOLS_PROFILER_PROFILER_H_

#include <chrono>
#include <ctime>
#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <ratio>
#include <utility>
#include "log/log.h"  // for DEFAULT_PROFILER_LOG_FOLDER_NAME
#include "log/spdlog/sinks/rotating_file_sink.h"  // for spdlog
#include "log/spdlog/spdlog.h"
#include "utility/utility.h"  // for Lee::GetCurrentPath()

#include <Windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")

/** 假设定义了_DEBUG这个宏则开启性能宏定义，否则PROFILER_F()是一个空函数 */
#ifdef _DEBUG
#undef LEE_C11_PROFILER_MODE
#define LEE_C11_PROFILER_MODE
#else
#undef LEE_C11_PROFILER_MODE
#endif

namespace Lee {
namespace Profiler_ {

/**
* @name                SpdLogProfiler
* @brief 给spdlog打印日志加一个中间层，如果不在debug模式下该函数定义为空
* @author              Lijiancong, pipinstall@163.com
* @date                2019-08-29 16:07:42
* @warning             线程安全

* @note
*/
static inline void SpdLogProfiler(const char* LogText, ...) {
  static std::once_flag flag;
  static std::shared_ptr<spdlog::logger> rotating_logger;
  std::call_once(flag, [&]() {
    /** 检查路径有没有创建 */
    auto strPath = Lee::GetRootPath();
    const std::string strLogRootFolder =
        strPath + "\\" + Lee::DEFAULT_LOG_FILE_FOLDER;
    if (!Lee::IsFileExist(strLogRootFolder)) {
      if (!Lee::CreateFileFolder(strLogRootFolder)) {
        std::cout << "Path " << strLogRootFolder << "is not exist!"
                  << std::endl;
        /// assert(false && "can't create log root floder");
        Lee::quick_exit(-1, "can't create log root floder");
      }
    }

    strPath +=
        "\\" + DEFAULT_LOG_ROOT_PATH + "\\" + DEFAULT_PROFILER_LOG_FOLDER_NAME;
    if (!Lee::IsFileExist(strPath)) {
      if (!Lee::CreateFileFolder(strPath)) {
        std::cout << "CreateFileFolder: " << strPath << " failed!" << std::endl;
        /// assert(false && "CreateFileFolder failed in Lee::profiler!");
        Lee::quick_exit(-1, "CreateFileFolder failed in Lee::profiler!");
      }
    }
    strPath += "\\profiler.log";
    rotating_logger =
        spdlog::rotating_logger_mt("profiler", strPath, 1048576 * 50, 10);
    rotating_logger->flush_on(spdlog::level::info);
  });

  char acLog[2048] = {0};
  va_list vaLog;
  va_start(vaLog, LogText);
  vsnprintf_s(acLog, sizeof(acLog) - 1, LogText, vaLog);
  acLog[2048 - 1] = '\0';
  va_end(vaLog);

  std::string sLog(acLog);
  rotating_logger->info(sLog);
  return;
}

class ProfilerInstance {
 public:
  typedef std::chrono::steady_clock SteadyClock;
  typedef SteadyClock::time_point TimePoint;
  typedef std::chrono::duration<double, std::ratio<1, 1>>
      DurationTime;  //单位秒
  enum class MemoryUnit { KB_, MB_, GB_ };

  ProfilerInstance(const std::string& sFunc, const std::string& sFile,
                   const int iLine)
      : m_Func(sFunc),
        m_File(sFile),
        m_Line(iLine),
        duringTime(0),
        startTime(SteadyClock::now()),
        finishTime(SteadyClock::now()) {
    SpdLogProfiler(
        "Profiler is Running in %s Line: %d Memory: %u KB(%u MB), "
        "this:%#x\n{",
        sFunc.c_str(), iLine, memory(), memory(MemoryUnit::MB_), this);
    start();
  }

  ~ProfilerInstance() {
    finish();
    SpdLogProfiler(
        "\n1Function: %s \nSpand Time: %.3fms( %.3fs) \nMemory: %u KB(%u MB) \n"
        "In File: %s  LINE: %d\nEnd of Function: %s, this: %#x\n}\n",
        m_Func.c_str(), millisecond(), second(), memory(),
        memory(MemoryUnit::MB_), m_File.c_str(), m_Line, m_Func.c_str(), this);
  }

 private:
#define KB / 1024
#define MB KB / 1024
#define GB MB / 1024
 private:
  DurationTime duringTime;
  TimePoint startTime;
  TimePoint finishTime;

  std::string m_Func;  ///< 传入的需要分析的函数名
  std::string m_File;  ///< 传入的当前分析在那个文件
  int m_Line;          ///< 传入的当前分析在哪一行

 public:
  void start()  // 开始计时
  {
    startTime = SteadyClock::now();
  }

  void finish()  // 结束计时
  {
    finishTime = SteadyClock::now();
    duringTime =
        std::chrono::duration_cast<DurationTime>(finishTime - startTime);
  }

  void dumpDuringTime(std::ostream& os = std::cout)  // 打印时间
  {
    os << "total " << duringTime.count() * 1000 << " milliseconds" << std::endl;
  }

  double second()  // 以秒为单位返回时间
  {
    return duringTime.count();
  }
  double millisecond()  // 以毫秒为单位返回时间
  {
    return duringTime.count() * 1000;
  }

  size_t memory(MemoryUnit mu = MemoryUnit::KB_)  // 查询当前程序的内存使用量
  {
    size_t memory = 0;

    PROCESS_MEMORY_COUNTERS pmc;
    HANDLE hProcess = GetCurrentProcess();
    if (!GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
      throw std::runtime_error("GetProcessMemoryInfo failed");
    CloseHandle(hProcess);

    switch (mu) {
      case MemoryUnit::KB_:
        memory = pmc.WorkingSetSize KB;
        break;
      case MemoryUnit::MB_:
        memory = pmc.WorkingSetSize MB;
        break;
      case MemoryUnit::GB_:
        memory = pmc.WorkingSetSize GB;
        break;
    }
    return memory;
  }
};  // end of class ProfilerInstance

}  // end of namespace Profiler_

/**
* @name                PROFILER_F
* @brief               这个宏封装了实例化ProfilerInstance的过程。
                       保证这个宏的生存周期与某函数生存周期一致
                       便可以得到这个宏的生存周期的时间。
* @param
* @return
* @author              Lijiancong, pipinstall@163.com
* @date                2019-07-11 09:00:58

* @note

* @example
1.想要知道某个函数运行时间
原语句：
void SomeFunc(int i)
{
    printf("%d", i);
}
现在写为：
void SomeFunc(int i)
{
    PROFILER_F();   //
写在函数定义的第一行，当代码运行至函数定义结束时，打印运行时间 printf("%d", i);
}

2.想要知道某段代码运行时间
原语句:
void SomeFunc(int i)
{
    i++;    // 想要知道这三行代码运行时间
    i++;    // 想要知道这三行代码运行时间
    i++;    // 想要知道这三行代码运行时间

    printf("%d", i);    // 不想计算该行的时间
}
现写为:
void SomeFunc(int i)
{
    {
        PROFILER_F();   //
这个宏与下面三行代码写在一个花括号内，并写在花括号开始第一行 i++; i++; i++;
    }

    printf("%d", i);
}
*/
#ifdef LEE_C11_PROFILER_MODE
#define PROFILER_F() \
  Lee::Profiler_::ProfilerInstance pRoFiLeR__(__func__, __FILE__, __LINE__)
#else
#define PROFILER_F() nullptr
#endif  // end of LEE_C11_PROFILER_MODE

}  // end of namespace Lee

#endif  // end of TOOLS_PROFILER_PROFILER_H_