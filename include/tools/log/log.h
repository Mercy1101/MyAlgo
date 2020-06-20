///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
///
/// @file       log.h
/// @brief      这个文件主要定义了重写的新log模块的接口，
///             会使用C++11以后的关键字，c++Builder不能使用
///
/// @author     Lijiancong, pipinstall@163.com
/// @date       2019-09-17 10:37:35
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
#ifndef MCT_UTILITY_DETAIL_LOG_LOG_H_
#define MCT_UTILITY_DETAIL_LOG_LOG_H_

#pragma warning(disable:4819)
#include <stdarg.h>  // for va_list

#include <cassert>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <thread>

#include "log/spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "utility/detail/system_utility.h"

/** 所有日志模块相对于程序根路径下的默认文件夹名称 */
const std::string DEFAULT_LOG_ROOT_PATH = "log";

/** detail日志模块相对于log根文件夹下的默认文件夹名称 */
const std::string DEFAULT_DETAIL_LOG_FOLDER_NAME = "detail";

/** profiler日志模块相对于log根文件夹下的默认文件夹名称 */
const std::string DEFAULT_PROFILER_LOG_FOLDER_NAME = "profiler";

namespace lee {
inline namespace utility {
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
 * @brief 定义了日志模块的打印等级。
 */
enum class SPD_LOG_LEVEL : int {
  LOG_TRACE = SPDLOG_LEVEL_TRACE,        ///< 0
  LOG_DEBUG = SPDLOG_LEVEL_DEBUG,        ///< 1
  LOG_INFO = SPDLOG_LEVEL_INFO,          ///< 2
  LOG_WARN = SPDLOG_LEVEL_WARN,          ///< 3
  LOG_ERROR = SPDLOG_LEVEL_ERROR,        ///< 4
  LOG_CRITICAL = SPDLOG_LEVEL_CRITICAL,  ///< 5
  LOG_OFF = SPDLOG_LEVEL_OFF,            ///< 6  全部不打印
};

/**
 * @brief               使用spdlog开源库来封装一个新的打印模块
 * @author              Lijiancong, pipinstall@163.com
 * @date                2019-09-17 13:54:01
 */
class SpdLogInstance {
 public:
  /**
 * @name     WriteSpdLog
 * @brief    主要进行C语言字符串整合为string型，
 *           传给日志打印函数，进行打印

 * @param    thread_id    [in]    线程ID
 * @param    strFuncName  [in]    调用该函数的函数名称
 * @param    iLineNumber  [in]    在第几行调用该函数
 * @param    eLogLevel    [in]    打印等级
 * @param    log          [in]    日志信息

 * @return   NONE
 * @author   Lijiancong, pipinstall@163.com
 * @date     2019-09-17 11:03:46
 * @warning  线程安全
 * @note
 */
  static void WriteSpdLog(const std::thread::id thread_id,
                          const std::string &strFuncName, const int iLineNumber,
                          const SPD_LOG_LEVEL &eLogLevel,
                          const std::string &log) {
    if (log.size() > 1024 * 2) {
      BaseLog(SPD_LOG_LEVEL::LOG_ERROR, "LOG text is too large!");
    }
    std::ostringstream oss;
    oss << thread_id;
    std::string stid = oss.str();

    std::string strLog;
#ifndef SWITCH_LOG_NO_FUNCNAME_LINENUMBER
    strLog = log + " <In Function: " + strFuncName +
             ", Line: " + std::to_string(iLineNumber) + ", PID: " + stid + ">";
#endif

    BaseLog(eLogLevel, strLog);
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
    static int g_iLogLevelConf = static_cast<int>(SPD_LOG_LEVEL::LOG_INFO);
    std::call_once(flag, InitSpdLogger, g_sptrDetailLogger, g_iLogLevelConf);
    int iLogLevel = static_cast<int>(eLogLevel);
    /** 无论怎么样都会写文件日志 */
    switch (iLogLevel) {
      case static_cast<int>(SPD_LOG_LEVEL::LOG_TRACE):
        g_sptrDetailLogger->trace(strLogText);
        break;

      case static_cast<int>(SPD_LOG_LEVEL::LOG_DEBUG):
        g_sptrDetailLogger->debug(strLogText);
        break;

      case static_cast<int>(SPD_LOG_LEVEL::LOG_INFO):
        g_sptrDetailLogger->info(strLogText);
        break;

      case static_cast<int>(SPD_LOG_LEVEL::LOG_WARN):
        g_sptrDetailLogger->warn(strLogText);
        break;

      case static_cast<int>(SPD_LOG_LEVEL::LOG_ERROR):
        g_sptrDetailLogger->error(strLogText);
        break;

      case static_cast<int>(SPD_LOG_LEVEL::LOG_CRITICAL):
        g_sptrDetailLogger->critical(strLogText);
        break;

      case static_cast<int>(SPD_LOG_LEVEL::LOG_OFF):
        /// Do Nothing
        break;

      default:
        g_sptrDetailLogger->info(strLogText);
        break;
    }

    /** 这里的判断条件来判定是否打印命令行窗口的日志 */
    if (iLogLevel >= g_iLogLevelConf) {
      switch (iLogLevel) {
        case static_cast<int>(SPD_LOG_LEVEL::LOG_TRACE):
          spdlog::trace(strLogText);
          break;

        case static_cast<int>(SPD_LOG_LEVEL::LOG_DEBUG):
          spdlog::debug(strLogText);
          break;

        case static_cast<int>(SPD_LOG_LEVEL::LOG_INFO):
          spdlog::info(strLogText);
          break;

        case static_cast<int>(SPD_LOG_LEVEL::LOG_WARN):
          spdlog::warn(strLogText);
          break;

        case static_cast<int>(SPD_LOG_LEVEL::LOG_ERROR):
          spdlog::error(strLogText);
          break;

        case static_cast<int>(SPD_LOG_LEVEL::LOG_CRITICAL):
          spdlog::critical(strLogText);
          break;

        case static_cast<int>(SPD_LOG_LEVEL::LOG_OFF):
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
    (void)iLogLevelConf;
    /** 检查路径有没有创建 */
    const auto strPath = ::lee::get_root_path();
    const std::string strLogRootFolder = strPath + "/" + DEFAULT_LOG_ROOT_PATH;
    if (!::lee::is_file_exist(strLogRootFolder)) {
      if (!::lee::create_file_folder(strLogRootFolder)) {
        std::cout << "Path " << strLogRootFolder << "is not exist!"
                  << std::endl;
        std::cout << "can't create log root floder" << std::endl;
      }
    }

    std::string strDetailLogPath = strPath + "/" + DEFAULT_LOG_ROOT_PATH + "/" +
                                   DEFAULT_DETAIL_LOG_FOLDER_NAME;
    if (!::lee::is_file_exist(strDetailLogPath)) {
      if (!::lee::create_file_folder(strDetailLogPath)) {
        std::cout << "CreateFileFolder: " << strDetailLogPath << " failed!"
                  << std::endl;
        std::cout << "CreateFileFolder failed in log/log.h!" << std::endl;
      }
    }
    strDetailLogPath += "/detail.log";
    sptrDetailLogger = spdlog::rotating_logger_mt("detail", strDetailLogPath,
                                                  1048576 * 50, 10);
    // auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>(
    //   "async_file_logger", "logs/async_log.txt");

    enum spdlog::level::level_enum eFlushLevel =
        spdlog::level::level_enum::warn;
    /** 读取配置中的日志等级 */
    /// if (!ReadLogConfig(DEFAULT_LOG_CONFIG_FILE_NAME, iLogLevelConf,
    ///                    eFlushLevel)) {
    ///   std::string LogText =
    ///       "Read config file: " + DEFAULT_LOG_CONFIG_FILE_NAME + " failed!";
    ///   sptrDetailLogger->error(LogText);
    /// }

    sptrDetailLogger->flush_on(eFlushLevel);
    sptrDetailLogger->set_level(spdlog::level::trace);

    spdlog::set_level(spdlog::level::trace);
  }
};
}  // namespace log
}  // namespace utility
}  // namespace lee

#define LOG_TRACE(x)                                  \
  ::lee::log::SpdLogInstance::WriteSpdLog(            \
      std::this_thread::get_id(), __func__, __LINE__, \
      ::lee::log::SPD_LOG_LEVEL::LOG_TRACE, std::string(x))
#define LOG_DEBUG(x)                                  \
  ::lee::log::SpdLogInstance::WriteSpdLog(            \
      std::this_thread::get_id(), __func__, __LINE__, \
      ::lee::log::SPD_LOG_LEVEL::LOG_DEBUG, std::string(x))
#define LOG_INFO(x)                                   \
  ::lee::log::SpdLogInstance::WriteSpdLog(            \
      std::this_thread::get_id(), __func__, __LINE__, \
      ::lee::log::SPD_LOG_LEVEL::LOG_INFO, std::string(x))
#define LOG_WARN(x)                                   \
  ::lee::log::SpdLogInstance::WriteSpdLog(            \
      std::this_thread::get_id(), __func__, __LINE__, \
      ::lee::log::SPD_LOG_LEVEL::LOG_WARN, std::string(x))
#define LOG_ERROR(x)                                  \
  ::lee::log::SpdLogInstance::WriteSpdLog(            \
      std::this_thread::get_id(), __func__, __LINE__, \
      ::lee::log::SPD_LOG_LEVEL::LOG_ERROR, std::string(x))
#define LOG_CRITICAL(x)                               \
  ::lee::log::SpdLogInstance::WriteSpdLog(            \
      std::this_thread::get_id(), __func__, __LINE__, \
      ::lee::log::SPD_LOG_LEVEL::LOG_CRITICAL, std::string(x))

#endif  // end of TOOLS_LOG_LOG_H
