#include "profiler/Profiler.h"

#include <catch2/catch.hpp>

TEST_CASE("PROFILER_F 的基准测试", "[profiler]") {
  BENCHMARK("简单测试") {
    for (int i = 0; i < 10; ++i) {
      PROFILER_F();
    }
  };
}