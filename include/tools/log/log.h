/**
 * Copyright (c) 2019 Lijiancong. All rights reserved.
 *
 * Use of this source code is governed by a MIT license
 * that can be found in the License file.
 */

///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/**
* @file                 log.h
* @brief                这个文件主要定义了重写的新log模块的接口，
                        会使用C++11以后的关键字，c++Builder不能使用

* @author               Lijiancong, pipinstall@163.com
* @date                 2019-09-17 10:37:35

* @note
*/
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
#ifndef TOOLS_LOG_LOG_H
#define TOOLS_LOG_LOG_H

#include <stdarg.h>  // for va_list

#include <cassert>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

#include "log/spdlog/spdlog.h"
#include "simpleini/SimpleIni.h"
#include "spdlog/sinks/rotating_file_sink.h"
//#include "utility/utility.h"  // for GetRootPath
#include "utility/detail/marco_utility.h"
#include "utility/detail/system_utility.h"

/** 所有日志模块相对于程序根路径下的默认文件夹名称 */
const std::string DEFAULT_LOG_ROOT_PATH = "log";

/** detail日志模块相对于log根文件夹下的默认文件夹名称 */
const std::string DEFAULT_DETAIL_LOG_FOLDER_NAME = "detail";

/** profiler日志模块相对于log根文件夹下的默认文件夹名称 */
const std::string DEFAULT_PROFILER_LOG_FOLDER_NAME = "profiler";

/** 默认的log模块配置文件，相对于程序根路径的相对路径和文件名 */
// const std::string DEFAULT_LOG_CONFIG_FILE_NAME = "config\\conf.ini";
const std::string DEFAULT_LOG_CONFIG_FILE_NAME =
    Lee::DEFAULT_CONFIG_FOLDER_NAME + "\\" + Lee::DEFAULT_CONFIG_FILE_NAME;

/**
 * @brief 定义了日志模块的打印等级。
 */
enum SPD_LOG_LEVEL : int {
  LOG_TRACE = SPDLOG_LEVEL_TRACE,        ///< 0
  LOG_DEBUG = SPDLOG_LEVEL_DEBUG,        ///< 1
  LOG_INFO = SPDLOG_LEVEL_INFO,          ///< 2
  LOG_WARN = SPDLOG_LEVEL_WARN,          ///< 3
  LOG_ERROR = SPDLOG_LEVEL_ERROR,        ///< 4
  LOG_CRITICAL = SPDLOG_LEVEL_CRITICAL,  ///< 5
  LOG_OFF = SPDLOG_LEVEL_OFF,            ///< 6  全部不打印
};

namespace Lee {
namespace log {
#undef SWITCH_LOG_NO_FUNCNAME_LINENUMBER  // 定义这个宏来删除
                                          // 在日志后面添加
                                          // <InFunction: Line:>
                                          // 这些信息
///< 默认log打印等级为info(2)
constexpr int DEFAULT_LOG_LEVEL_CONF = 2;
///< 默认log刷新文件等级为warn(3)
constexpr int DEFAULT_FLUSH_LEVEL_CONF = 3;

/**
 * @brief               使用spdlog开源库来封装一个新的打印模块
 * @author              Lijiancong, pipinstall@163.com
 * @date                2019-09-17 13:54:01
 */
class SpdLogInstance {
 public:
  /**
  * @name                WriteSpdLog
  * @brief 主要进行C语言字符串整合为string型，传给日志打印函数，进行打印

  * @param               strFuncName  [in]    调用该函数的函数名称
  * @param               iLineNumber  [in]    在第几行调用该函数
  * @param               eLogLevel    [in]    打印等级
  * @param               szFormat     [in]    日志信息，C型字符串

  * @return              NONE
  * @author              Lijiancong, pipinstall@163.com
  * @date                2019-09-17 11:03:46
  * @warning             线程安全
  * @note
  */
  static void WriteSpdLog(const std::string &strFuncName, const int iLineNumber,
                          const enum SPD_LOG_LEVEL &eLogLevel,
                          const char *szFormat, ...) {
    va_list valog;
    va_start(valog, szFormat);
    char acLog[1024 * 2] = {0};
    vsprintf_s(acLog, sizeof(acLog), szFormat, valog);
    // std::cout << "in>>" << acLog << std::endl;
    std::string strLog(acLog);

#ifndef SWITCH_LOG_NO_FUNCNAME_LINENUMBER
    strLog = strLog + " <In Function: " + strFuncName +
             ", Line: " + std::to_string(iLineNumber) + ">";
#endif

    BaseLog(eLogLevel, strLog);
    // std::cout << "out>>" << acLog << std::endl;
    va_end(valog);
  }
  static void WriteSpdLog(const enum SPD_LOG_LEVEL &eLogLevel,
                          const std::string &strLogText) {
    BaseLog(eLogLevel, strLogText);
  }

 private:
  /** 禁止这个类被实例化 */
  SpdLogInstance() = delete;

  /**
  * @name                BaseLog
  * @brief               主要进行日志打印的函数

  * @param               eLogLevel    [in] 日志打印等级，只能是这个枚举里有的值
  * @param               strLogText   [in] 需要打印的text

  * @return              NONE
  * @author              Lijiancong, pipinstall@163.com
  * @date                2019-09-17 11:01:22
  * @warning             线程不安全

  * @note
  */
  static void BaseLog(const enum SPD_LOG_LEVEL &eLogLevel,
                      const std::string &strLogText) {
    // PROFILER_F();
    static std::mutex LoggerMutex;
    std::lock_guard<std::mutex> Lock(LoggerMutex);
    static std::once_flag flag;
    static std::shared_ptr<spdlog::logger> g_sptrDetailLogger;
    static int g_iLogLevelConf = SPD_LOG_LEVEL::LOG_INFO;
    std::call_once(flag, InitSpdLogger, g_sptrDetailLogger, g_iLogLevelConf);
    int iLogLevel = eLogLevel;
    /** 无论怎么样都会写文件日志 */
    switch (iLogLevel) {
      case LOG_TRACE:
        g_sptrDetailLogger->trace(strLogText);
        break;

      case LOG_DEBUG:
        g_sptrDetailLogger->debug(strLogText);
        break;

      case LOG_INFO:
        g_sptrDetailLogger->info(strLogText);
        break;

      case LOG_WARN:
        g_sptrDetailLogger->warn(strLogText);
        break;

      case LOG_ERROR:
        g_sptrDetailLogger->error(strLogText);
        break;

      case LOG_CRITICAL:
        g_sptrDetailLogger->critical(strLogText);
        break;

      case LOG_OFF:
        /// Do Nothing
        break;

      default:
        g_sptrDetailLogger->info(strLogText);
        break;
    }

    /** 这里的判断条件来判定是否打印命令行窗口的日志 */
    if (iLogLevel >= g_iLogLevelConf) {
      switch (iLogLevel) {
        case LOG_TRACE:
          spdlog::trace(strLogText);
          break;

        case LOG_DEBUG:
          spdlog::debug(strLogText);
          break;

        case LOG_INFO:
          spdlog::info(strLogText);
          break;

        case LOG_WARN:
          spdlog::warn(strLogText);
          break;

        case LOG_ERROR:
          spdlog::error(strLogText);
          break;

        case LOG_CRITICAL:
          spdlog::critical(strLogText);
          break;

        case LOG_OFF:
          /// Do Nothing
          break;

        default:
          spdlog::info(strLogText);
          break;
      }
    }
  }

  /**
  * @name                InitSpdLogger
  * @brief               初始化SpdLogger的封装

  * @param               sptrDetailLogger [out]
  * @param               iLogLevelConf    [out]

  * @return              NONE
  * @author              Lijiancong, pipinstall@163.com
  * @date                2019-09-17 13:33:37
  * @warning             线程不安全

  * @note
  */

  static void InitSpdLogger(std::shared_ptr<spdlog::logger> &sptrDetailLogger,
                            int &iLogLevelConf) {
    /** 检查路径有没有创建 */
    const auto strPath = Lee::GetRootPath();
    const std::string strLogRootFolder =
        strPath + "\\" + DEFAULT_LOG_FILE_FOLDER;
    if (!Lee::IsFileExist(strLogRootFolder)) {
      if (!Lee::CreateFileFolder(strLogRootFolder)) {
        std::cout << "Path " << strLogRootFolder << "is not exist!"
                  << std::endl;
        assert(false && "can't create log root floder");
        exit(-1);
      }
    }

    std::string strDetailLogPath = strPath + "\\" + DEFAULT_LOG_ROOT_PATH +
                                   "\\" + DEFAULT_DETAIL_LOG_FOLDER_NAME;
    if (!Lee::IsFileExist(strDetailLogPath)) {
      if (!Lee::CreateFileFolder(strDetailLogPath)) {
        std::cout << "CreateFileFolder: " << strDetailLogPath << " failed!"
                  << std::endl;
        assert(false && "CreateFileFolder failed in log/log.h!");
        exit(-1);
      }
    }
    strDetailLogPath += "\\detail.log";
    sptrDetailLogger = spdlog::rotating_logger_mt("detail", strDetailLogPath,
                                                  1048576 * 50, 10);

    enum spdlog::level::level_enum eFlushLevel =
        spdlog::level::level_enum::warn;
    /** 读取配置中的日志等级 */
    if (!ReadLogConfig(DEFAULT_LOG_CONFIG_FILE_NAME, iLogLevelConf,
                       eFlushLevel)) {
      std::string LogText =
          "Read config file: " + DEFAULT_LOG_CONFIG_FILE_NAME + " failed!";
      sptrDetailLogger->error(LogText);
    }

    sptrDetailLogger->flush_on(eFlushLevel);
    sptrDetailLogger->set_level(spdlog::level::trace);

    spdlog::set_level(spdlog::level::trace);
  }

  /**
  * @name                ReadLogConfig
  * @brief               读取配置文件中的日志等级

  * @param               FileName         [in]    日志文件的相对路径和文件名
  * @param               iLogLevelConf    [in]    日志显示等级
  * @param               eFlushLevel      [in]    日志刷新等级

  * @return              读取成功或失败

  * @author              Lijiancong, pipinstall@163.com
  * @date                2019-09-17 13:19:34
  * @warning             线程不安全
  * @note
  */
  static bool ReadLogConfig(
      const std::string &FileName, int &iLogLevelConf,
      enum spdlog::level::level_enum &eFlushLevel) noexcept {
    if (FileName.empty()) return false;

    /** 判断该文件存不存在 */
    char acFile[512] = {0};
    sprintf_s(acFile, sizeof(acFile), "%s\\%s", Lee::GetRootPath().c_str(),
              FileName.c_str());
    if (!Lee::IsFileExist(acFile)) {
      printf("file %s not exist!\n", acFile);
      assert(false && "log config file not exist!\n");
      // exit(-1);
      return false;
    }
    bool IsFileUtf8 = true;
    CSimpleIniA ini(IsFileUtf8, false, false);
    SI_Error rc = ini.LoadFile(acFile);
    if (rc < 0) return false;
    // get the value of a key
    std::string strLogLevel =
        ini.GetValue("SpdLogConfig", "spd_log_level", NULL);
    iLogLevelConf =
        strLogLevel.empty() ? DEFAULT_LOG_LEVEL_CONF : std::stoi(strLogLevel);

    std::string strFlushLevel =
        ini.GetValue("SpdLogConfig", "spd_flush_level", NULL);
    int iLogFlushLevel = strLogLevel.empty() ? DEFAULT_FLUSH_LEVEL_CONF
                                             : std::stoi(strFlushLevel);

    if (LOG_TRACE == iLogFlushLevel)
      eFlushLevel = spdlog::level::level_enum::trace;
    else if (LOG_DEBUG == iLogFlushLevel)
      eFlushLevel = spdlog::level::level_enum::debug;
    else if (LOG_INFO == iLogFlushLevel)
      eFlushLevel = spdlog::level::level_enum::info;
    else if (LOG_WARN == iLogFlushLevel)
      eFlushLevel = spdlog::level::level_enum::warn;
    else if (LOG_ERROR == iLogFlushLevel)
      eFlushLevel = spdlog::level::level_enum::err;
    else if (LOG_CRITICAL == iLogFlushLevel)
      eFlushLevel = spdlog::level::level_enum::critical;
    else
      eFlushLevel = spdlog::level::level_enum::warn;

    return true;
  }
};
}  // end of namespace log
}  // end of namespace Lee

#define LOG(level, text, ...)                                            \
  Lee::log::SpdLogInstance::WriteSpdLog(__func__, __LINE__, level, text, \
                                        __VA_ARGS__)
#define LOG_NO_FUNCTIONNAME(level, string) \
  Lee::log::SpdLogInstance::WriteSpdLog(level, string)

#endif  // end of TOOLS_LOG_LOG_H
