
#include "utility/detail/concurrency_utility.h"

#include <catch2/catch.hpp>
#include <iostream>

TEST_CASE("层级锁用法示例", "[utility][concurrency]") {
  lee::hierarchical_mutex mutex_high(500);
  lee::hierarchical_mutex mutex_mid(600);
  lee::hierarchical_mutex mutex_low(700);
  try {
    std::lock_guard<lee::hierarchical_mutex> lock0(mutex_high);
    std::lock_guard<lee::hierarchical_mutex> lock1(mutex_mid);
    std::lock_guard<lee::hierarchical_mutex> lock2(mutex_low);
  } catch (const std::exception& e) {
    (void)e;
    /// std::cout << e.what() << std::endl;
    REQUIRE(false);
  }
  try {
    std::lock_guard<lee::hierarchical_mutex> lock0(mutex_low);
    std::lock_guard<lee::hierarchical_mutex> lock1(mutex_mid);
    std::lock_guard<lee::hierarchical_mutex> lock2(mutex_high);
  } catch (const std::exception& e) {
    (void)e;
    /// std::cout << e.what() << std::endl;
    REQUIRE(true);
  }
}
