#include <iostream>
#include <gtest/gtest.h>
#include "log/log.h"

/** 
 * @brief 对log模块进行简单的单元测试
 */
TEST(Log, Simple)
{
    LOG(SPD_TRACE, "This is Unittest!");
    LOG(SPD_DEBUG, "This is Unittest!");
    LOG(SPD_INFO, "This is Unittest!");
    LOG(SPD_WARN, "This is Unittest!");
    LOG(SPD_ERROR, "This is Unittest!");
    LOG(SPD_CRITICAL, "This is Unittest!");
    LOG(SPD_OFF, "This is Unittest!");
    LOG(SPD_INFO, "%s,%d,%x", "This is Unittest!", 1024, 256);

    LOG_NO_FUNCTIONNAME(SPD_TRACE, "This is Unittest!");
    LOG_NO_FUNCTIONNAME(SPD_DEBUG, "This is Unittest!");
    LOG_NO_FUNCTIONNAME(SPD_INFO, "This is Unittest!");
    LOG_NO_FUNCTIONNAME(SPD_WARN, "This is Unittest!");
    LOG_NO_FUNCTIONNAME(SPD_ERROR, "This is Unittest!");
    LOG_NO_FUNCTIONNAME(SPD_CRITICAL, "This is Unittest!");
    LOG_NO_FUNCTIONNAME(SPD_OFF, "This is Unittest!");
    /** 不支持变参，只支持string */
    // LOG_NO_FUNCTIONNAME(SPD_INFO, "%s,%d,%x", "This is Unittest!", 1024, 256);
}