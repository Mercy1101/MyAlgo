#include "log/log.h"

#include <atomic>
#include <catch2/catch.hpp>
#include <iostream>
#include "utility/utility.h"

// #define LOG_UNITTEST
#undef LOG_UNITTEST
#ifdef LOG_UNITTEST
/**
 * @brief 对log模块进行简单的单元测试
 */
TEST_CASE("简单的打印测试", "[log]") {

  LOG(LOG_TRACE, "This is Unittest!");
  LOG(LOG_DEBUG, "This is Unittest!");
  LOG(LOG_INFO, "This is Unittest!");
  LOG(LOG_WARN, "This is Unittest!");
  LOG(LOG_ERROR, "This is Unittest!");
  LOG(LOG_CRITICAL, "This is Unittest!");
  LOG(LOG_OFF, "This is Unittest!");
  LOG(LOG_INFO, "%s,%d,%x", "This is Unittest!", 1024, 256);

  LOG_NO_FUNCTIONNAME(LOG_TRACE, "This is Unittest!");
  LOG_NO_FUNCTIONNAME(LOG_DEBUG, "This is Unittest!");
  LOG_NO_FUNCTIONNAME(LOG_INFO, "This is Unittest!");
  LOG_NO_FUNCTIONNAME(LOG_WARN, "This is Unittest!");
  LOG_NO_FUNCTIONNAME(LOG_ERROR, "This is Unittest!");
  LOG_NO_FUNCTIONNAME(LOG_CRITICAL, "This is Unittest!");
  LOG_NO_FUNCTIONNAME(LOG_OFF, "This is Unittest!");

  /** 不支持变参，只支持string */
  // LOG_NO_FUNCTIONNAME(SPD_INFO, "%s,%d,%x", "This is Unittest!", 1024, 256);
}

std::atomic<int> count = 0;
TEST_CASE("测试多线程下异步日志的效果", "[log]") {
  for (int i = 0; i < 100; ++i)
  {
    std::thread ls([i]() {
      for (int xx = 0; xx < 100; ++xx)
        LOG(LOG_INFO, "this is count(%d) thread(%d).", ++count, i);
      Lee::SleepForRandomMilliSecond(2);
                   });
    ls.detach();
  }
}
#endif // end of LOG_UNITTEST
