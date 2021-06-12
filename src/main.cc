#define CATCH_CONFIG_RUNNER

#include <iostream>

#include "my_algo/my_algo.hpp"

int main(int argc, char** argv)
{
    Catch::Session().run(argc, argv);
    return 0;
}
