#define CATCH_CONFIG_RUNNER

#include <catch2/catch.hpp>  // include after defining CATCH_CONFIG_RUNNER
#include <iostream>

#include "ut.hpp"
#include "utility/utility.h"

int main(int argc, char** argv) {
  lee::ignore_unused(argc, argv);
  /// Catch::Session().run(argc, argv);
  boost::ut::expect(true);
  boost::ut::expect(1 == 2);
  using namespace boost::ut;
  expect(1_i == 2);
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;

  expect(1_i == 2);                  // UDL syntax
  expect(1 == 2_i);                  // UDL syntax
  expect(that % 1 == 2);             // Matcher syntax
  expect(eq(1, 2));                  // eq/neq/gt/ge/lt/le
  "hello world"_test = [] {
    int i = 43;
    expect(42_i == i);
  };
  system("pause");
  return 0;
}
/// #include <benchmark/benchmark.h>
///
/// static void BM_StringCreation(benchmark::State& state) {
///   for (auto _ : state)
///     std::string empty_string;
/// }
/// // Register the function as a benchmark
/// BENCHMARK(BM_StringCreation);
///
/// // Define another benchmark
/// static void BM_StringCopy(benchmark::State& state) {
///   std::string x = "hello";
///   for (auto _ : state)
///     std::string copy(x);
/// }
/// BENCHMARK(BM_StringCopy);
///
/// BENCHMARK_MAIN();
