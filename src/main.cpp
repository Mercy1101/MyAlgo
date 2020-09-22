#define CATCH_CONFIG_RUNNER

#include <catch2/catch.hpp>  // include after defining CATCH_CONFIG_RUNNER
#include <iostream>

#include "ut.hpp"
#include "utility/utility.h"

int main(int argc, char** argv) {
  Catch::Session().run(argc, argv);
  system("pause");
  return 0;
}
