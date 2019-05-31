#include <iostream>
#include <cstdio>
#include "gtest/gtest.h"
#include "gos_typedef.h"
#include "GLog.h"

int main(int argc, char **argv)
{
    {
        using namespace Lee::GosLog;
        GosInitLog();
        GosLog(LOG_ERROR, "It's Test %s.","红色");
        GosLog(LOG_DETAIL, "It's Test %s.","灰色");
        GosLog(LOG_INFO, "It's Test %s.","白色");
        GosLog(LOG_WARN, "It's Test %s.","黄色");
        GosLog(LOG_FAIL, "It's Test %s.","红色");
        GosLog(LOG_FATAL, "It's Test %s.","红色");
        GosLog(LOG_PROFILER, "It's Test %s.","青色");
    }

    ::testing::InitGoogleTest(&argc, argv);

    PROFILER_F(RUN_ALL_TESTS);
    PROFILER_F(printf, "It's Test. %d %u\n", "12s12", -34, 69);

    return 0;
}

