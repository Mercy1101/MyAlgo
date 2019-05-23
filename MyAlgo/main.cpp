#include <iostream>
#include "gtest/gtest.h"
#include "easylogging++.h"
#include "gos_typedef.h"
#include "profiler.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    PROFILER_F(RUN_ALL_TESTS);
    PROFILER_F(printf, "It's Test. %d%u\n", "12s12", -34, 69);
    return 0;
}