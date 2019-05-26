#include <iostream>
#include "gtest/gtest.h"
#include "easylogging++.h"
#include "gos_typedef.h"
#include "profiler.h"
#include <easy/profiler.h>

INITIALIZE_EASYLOGGINGPP

/// @brief Not a very fast log builder but good enough for sample
class MyLogBuilder : public el::LogBuilder
{
public:
    std::string build(const el::LogMessage* logMessage, bool appendNewLine) const
    {
        std::stringstream str;
        str << logMessage->message();
        if (appendNewLine) str << "\n";
        return str.str().c_str();
    }
};

int main(int argc, char **argv)
{
    el::LogBuilderPtr myLogBuilder = el::LogBuilderPtr(new MyLogBuilder());
    el::Loggers::getLogger("default")->setLogBuilder(myLogBuilder);
    LOG(INFO) << "message from default logger";

    el::Loggers::getLogger("new_logger");
    CLOG(INFO, "new_logger") << "message from new_logger logger";

    ::testing::InitGoogleTest(&argc, argv);
    PROFILER_F(RUN_ALL_TESTS);
    PROFILER_F(printf, "It's Test. %d%u\n", -34, 69);
    return 0;
}