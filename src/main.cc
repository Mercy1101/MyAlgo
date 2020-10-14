#define CATCH_CONFIG_RUNNER

#include <iostream>

#include "catch/catch.hpp"  // include after defining CATCH_CONFIG_RUNNER

int main(int argc, char** argv) {
  Catch::Session().run(argc, argv);
  return 0;
}