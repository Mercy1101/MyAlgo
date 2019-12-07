#define CATCH_CONFIG_RUNNER
#include <iostream>

#include "Version/Version.h"  // for print PrintVersion()
#include <catch2/catch.hpp>   // include after defining CATCH_CONFIG_RUNNER
#include "init/init.h"
#include "utility/utility.h"

int main(int argc, char** argv) {
  PrintVersion();
  CheckConfigFolder();
  Catch::Session().run(argc, argv);
  system("pause");
  return 0;
}
