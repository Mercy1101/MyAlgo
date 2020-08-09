#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>  // include after defining CATCH_CONFIG_RUNNER
#include <iostream>

#include "utility/utility.h"

/// int main(int argc, char** argv) {
///   PrintVersion();
///   CheckConfigFolder();
///   Catch::Session().run(argc, argv);
/// 
///   return 0;
/// }
#include <benchmark/benchmark.h>

static void BM_StringCreation(benchmark::State& state) {
  for (auto _ : state)
    std::string empty_string;
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
  std::string x = "hello";
  for (auto _ : state)
    std::string copy(x);
}
BENCHMARK(BM_StringCopy);

BENCHMARK_MAIN();
