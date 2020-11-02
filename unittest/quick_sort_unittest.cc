#include "catch/catch.hpp"
#include "my_algo/my_algo.hpp"
#include <algorithm>
#include <random>

TEST_CASE("quick_sort 简单测试", "[lee][quick_sort]") {
  using namespace lee;
  std::vector<int> vec(10000, 0);
  std::iota(vec.begin(), vec.end(), 0);
  std::random_device d;
  std::mt19937 g(d());
  std::shuffle(vec.begin(), vec.end(), g);
  
  quick_sort(vec.begin(), vec.end());
  REQUIRE(std::is_sorted(vec.begin(), vec.end()));

  std::vector<int> same(10000, 1);
  quick_sort(same.begin(), same.end());
  REQUIRE(std::is_sorted(same.begin(), same.end()));
}