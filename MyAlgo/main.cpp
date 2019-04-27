#include <iostream>
#include "gtest/gtest.h"
#include "easylogging++.h"
#include "list.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    return 0;
}