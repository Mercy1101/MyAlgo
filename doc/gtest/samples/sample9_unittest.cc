// Copyright 2009 Google Inc. All Rights Reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


// This sample shows how to use Google Test listener API to implement
// an alternative console output and how to use the UnitTest reflection API
// to enumerate test cases and tests and to inspect their results.

#include <stdio.h>

#include "gtest/gtest.h"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
namespace {
// Provides alternative output mode which produces minimal amount of
// information about tests.
class TersePrinter : public EmptyTestEventListener {
 private:
  // Called before any test activity starts.
  void OnTestProgramStart(const UnitTest& /* unit_test */) override {}

  // Called after all test activities have ended.
  void OnTestProgramEnd(const UnitTest& unit_test) override {
    fprintf(stdout, "TEST %s\n", unit_test.Passed() ? "PASSED" : "FAILED");
    fflush(stdout);
  }

  // Called before a test starts.
  void OnTestStart(const TestInfo& test_info) override {
    fprintf(stdout,
            "*** Test %s.%s starting.\n",
            test_info.test_case_name(),
            test_info.name());
    fflush(stdout);
  }

  // Called after a failed assertion or a SUCCEED() invocation.
  void OnTestPartResult(const TestPartResult& test_part_result) override {
    fprintf(stdout,
            "%s in %s:%d\n%s\n",
            test_part_result.failed() ? "*** Failure" : "Success",
            test_part_result.file_name(),
            test_part_result.line_number(),
            test_part_result.summary());
    fflush(stdout);
  }

  // Called after a test ends.
  void OnTestEnd(const TestInfo& test_info) override {
    fprintf(stdout,
            "*** Test %s.%s ending.\n",
            test_info.test_case_name(),
            test_info.name());
    fflush(stdout);
  }
};  // class TersePrinter

TEST(CustomOutputTest, PrintsMessage) {
  printf("Printing something from the test body...\n");
}

TEST(CustomOutputTest, Succeeds) {
  SUCCEED() << "SUCCEED() has been invoked from here";
}

TEST(CustomOutputTest, Fails) {
  EXPECT_EQ(1, 2)
      << "This test fails in order to demonstrate alternative failure messages";
}
}  // namespace

int main(int argc, char **argv) {
  InitGoogleTest(&argc, argv);

  bool terse_output = false;
  if (argc > 1 && strcmp(argv[1], "--terse_output") == 0 )
    terse_output = true;
  else
    printf("%s\n", "Run this program with --terse_output to change the way "
           "it prints its output.");

  UnitTest& unit_test = *UnitTest::GetInstance();

  // 如果给出--terse_output命令行标志，则禁止
  // 标准输出并附加自己的结果打印机。
  if (terse_output) {
    TestEventListeners& listeners = unit_test.listeners();

    //从列表中删除默认的控制台输出侦听器，这样就可以了
    //不接收来自Google Test的活动，也不会打印任何输出。 以来
    //此操作将监听器的所有权转移给我们的调用者
    //也必须删除它。
    delete listeners.Release(listeners.default_result_printer());

    // 将自定义输出侦听器添加到列表中。 它现在会收到
    // 来自Google Test的事件并打印替代输出。 我们没有
    // 由于Google Test承担所有权，因此必须担心删除它
    // 在将其添加到列表后将其覆盖。
    listeners.Append(new TersePrinter);
  }
  int ret_val = RUN_ALL_TESTS();

  // 这是使用UnitTest反射API检查测试的示例
  // 结果 在这里，我们从我们预期失败的测试中打折失败。
  int unexpectedly_failed_tests = 0;
  for (int i = 0; i < unit_test.total_test_case_count(); ++i) {
    const TestCase& test_case = *unit_test.GetTestCase(i);
    for (int j = 0; j < test_case.total_test_count(); ++j) {
      const TestInfo& test_info = *test_case.GetTestInfo(j);
      // Counts failed tests that were not meant to fail (those without
      // 'Fails' in the name).
      if (test_info.result()->Failed() &&
          strcmp(test_info.name(), "Fails") != 0) {
        unexpectedly_failed_tests++;
      }
    }
  }

  // Test that were meant to fail should not affect the test program outcome.
  if (unexpectedly_failed_tests == 0)
    ret_val = 0;

  return ret_val;
}
