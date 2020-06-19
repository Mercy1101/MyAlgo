#include "log/log.h"

#include <atomic>
#include <catch2/catch.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#define LOG_UNITTEST
// #define LOG_UNITTEST 日志测试的开关
#ifdef LOG_UNITTEST
/**
 * @brief 对log模块进行简单的单元测试
 */
TEST_CASE("简单的打印测试", "[log]") {
  lee::log::SpdLogInstance::WriteSpdLog(
      std::this_thread::get_id(), __func__, __LINE__,
      ::lee::log::SPD_LOG_LEVEL::LOG_INFO, "sjfdlkds");
      LOG_TRACE("This is Unittest!");
  LOG_DEBUG("This is Unittest!");
  LOG_INFO("This is Unittest!");
  LOG_WARN("This is Unittest!");
  LOG_ERROR("This is Unittest!");
  LOG_CRITICAL("This is Unittest!");

  std::string temp(" sdfsdf ");
      LOG_TRACE(temp+"This is Unittest!");
  LOG_DEBUG("This is Unittest!"+temp);
 /// LOG_INFO("This is Unittest!" + "");
 /// LOG_WARN(""+"This is Unittest!");
 /// LOG_ERROR("sldkjflksd"+"This is Unittest!");
 /// LOG_CRITICAL("This is Unittest!" + "lskdjflsd");
}

#endif  // end of LOG_UNITTEST
