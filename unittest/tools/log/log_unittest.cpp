#include "log/log.h"

#include <catch2/catch.hpp>
#include <iostream>

/**
 * @brief 对log模块进行简单的单元测试
 */
TEST_CASE("简单的打印测试", "[log]") {
  #if 0
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
  #endif
  /** 不支持变参，只支持string */
  // LOG_NO_FUNCTIONNAME(SPD_INFO, "%s,%d,%x", "This is Unittest!", 1024, 256);
}
