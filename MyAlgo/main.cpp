#include <iostream>
#include "gtest/gtest.h"
#include "easylogging++.h"
#include "profiler.h"
#include <easy/profiler.h>

INITIALIZE_EASYLOGGINGPP

using namespace LEE_DEBUG;

int main(int argc, char **argv)
{
    LOG_IF(true, INFO) << "Logged if condition is true";
    for (int i = 1; i <= 10; ++i)
    {
        // 每运行该语句两次后，才做一次记录
        LOG_EVERY_N(2, INFO) << "Logged every second iter";
        // 5 logs written; 2, 4, 6, 7, 10
    }

    for (int i = 1; i <= 10; ++i)
    {
        // 在运行该语句两次后，每次记录
        LOG_AFTER_N(2, INFO) << "Log after 2 hits; " << i;
        // 8 logs written; 3, 4, 5, 6, 7, 8, 9, 10
    }

    for (int i = 1; i <= 100; ++i)
    {
        // 在运行三次该语句后，不再执行记录
        LOG_N_TIMES(3, INFO) << "Log only 3 times; " << i;
        // 3 logs writter; 1, 2, 3
    }

    ::testing::InitGoogleTest(&argc, argv);
    PROFILER_F(RUN_ALL_TESTS);
    PROFILER_F(printf, "It's Test. %d%u\n", -34, 69);
    return 0;
}