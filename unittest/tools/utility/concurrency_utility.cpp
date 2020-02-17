
#include "utility/detail/concurrency_utility.h"

#include <catch2/catch.hpp>

TEST_CASE("层级锁用法示例", "[utility][concurrency]") {
  Lee::hierarchical_mutex mutex_high(500);
  Lee::hierarchical_mutex mutex_mid(600);
  Lee::hierarchical_mutex mutex_low(700);
  try {
    std::lock_guard<Lee::hierarchical_mutex> lock0(mutex_high);
    std::lock_guard<Lee::hierarchical_mutex> lock1(mutex_mid);
    std::lock_guard<Lee::hierarchical_mutex> lock2(mutex_low);
  } catch (const std::exception& e) {
    REQUIRE(false);
  }
  try {
    std::lock_guard<Lee::hierarchical_mutex> lock0(mutex_low);
    std::lock_guard<Lee::hierarchical_mutex> lock1(mutex_mid);
    std::lock_guard<Lee::hierarchical_mutex> lock2(mutex_high);
  } catch (const std::exception& e) {
    REQUIRE(true);
  }
}
