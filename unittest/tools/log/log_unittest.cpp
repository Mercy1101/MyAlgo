#include "log/log.h"

#include <atomic>
#include <catch2/catch.hpp>
#include <iostream>

#include "utility/utility.h"

// #define LOG_UNITTEST 日志测试的开关
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
  for (int i = 0; i < 100; ++i) {
    std::thread ls([i]() {
      for (int xx = 0; xx < 100; ++xx)
        LOG(LOG_INFO, "this is count(%d) thread(%d).", ++count, i);
      /// Lee::SleepForRandomMilliSecond(2);
    });
    ls.detach();
  }
}

TEST_CASE("log 模块的速度测试", "[log]") {
  BENCHMARK(" All Log 1000") {
    for (int i = 0; i < 1; ++i) {
      LOG(LOG_TRACE,
          "DialResult is failed, loop decrease and dial again. (ScheduleID: "
          "%d, DialNumber: %s, DialResult: %f "
          "is_audit_dial: false)",
          i, "01010086", i + 40.0005);
      LOG(LOG_DEBUG,
          "DialResult is failed, loop decrease and dial again. (ScheduleID: "
          "%d, DialNumber: %s, DialResult: %f "
          "is_audit_dial: false)",
          i, "01010086", i + 40.0005);
      LOG(LOG_INFO,
          "DialResult is failed, loop decrease and dial again. (ScheduleID: "
          "%d, DialNumber: %s, DialResult: %f "
          "is_audit_dial: false)",
          i, "01010086", i + 40.0005);
      LOG(LOG_WARN,
          "DialResult is failed, loop decrease and dial again. (ScheduleID: "
          "%d, DialNumber: %s, DialResult: %f "
          "is_audit_dial: false)",
          i, "01010086", i + 40.0005);
      LOG(LOG_ERROR,
          "DialResult is failed, loop decrease and dial again. (ScheduleID: "
          "%d, DialNumber: %s, DialResult: %f "
          "is_audit_dial: false)",
          i, "01010086", i + 40.0005);
      LOG(LOG_CRITICAL,
          "DialResult is failed, loop decrease and dial again. (ScheduleID: "
          "%d, DialNumber: %s, DialResult: %f "
          "is_audit_dial: false)",
          i, "01010086", i + 40.0005);
      LOG(LOG_OFF,
          "DialResult is failed, loop decrease and dial again. (ScheduleID: "
          "%d, DialNumber: %s, DialResult: %f "
          "is_audit_dial: false)",
          i, "01010086", i + 40.0005);
      LOG(LOG_INFO,
          "DialResult is failed, loop decrease and dial again. (ScheduleID: "
          "%d, DialNumber: %s, DialResult: %f "
          "is_audit_dial: false)",
          i, "01010086", i + 40.0005);
    }
    return 0;
  };
}
#endif  // end of LOG_UNITTEST
