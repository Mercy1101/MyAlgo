#define CATCH_CONFIG_RUNNER
#include <iostream>

#include "Version/Version.h"  // for print PrintVersion()
#include "catch2/catch.hpp"   // include after defining CATCH_CONFIG_RUNNER
//#include "gtest/gtest.h"
#include "init/init.h"
#include "utility/utility.h"

int main(int argc, char** argv) {
  Lee::ignore_unused(argc, argv);
  PrintVersion();
  CheckConfigFolder();
  //
  ///** 测试模式下跑单元测试 */
  //#ifdef _DEBUG
  //  testing::InitGoogleTest(&argc, argv);
  //#ifdef TXT_OUTPUT
  //  // Gets hold of the event listener list.
  //  ::testing::TestEventListeners& listeners =
  //      ::testing::UnitTest::GetInstance()->listeners();
  //  delete listeners.Release(listeners.default_result_printer());
  //  // Adds a listener to the end.  googletest takes the ownership.
  //  listeners.Append(new MinimalistPrinter("./" + getCurrentTimeStr() +
  //  ".txt"));
  //#endif
  //  RUN_ALL_TESTS();
  //#endif

  /// catch2的单元测试
  Catch::Session().run(argc, argv);
  system("pause");
  return 0;
}
