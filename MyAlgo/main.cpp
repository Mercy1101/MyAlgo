#include <iostream>
#include <cstdio>
#include <cassert>
#include <random>
#include <algorithm>
#include <iterator>
#include "gtest/gtest.h"
#include "gos_typedef.h"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    system("pause");

    return 0;
}

