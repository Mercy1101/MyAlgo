#include <iostream>
#include "gtest/gtest.h"
#include "easylogging++.h"
#include "profiler.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv)
{
    TinySTL::profiler::ProfilerInstance::start();
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    LOG(INFO) << "Test once!";
    TinySTL::profiler::ProfilerInstance::finish();
    TinySTL::profiler::ProfilerInstance::dumpDuringTime();
    TinySTL::profiler::ProfilerInstance::memory();
    return 0;
}