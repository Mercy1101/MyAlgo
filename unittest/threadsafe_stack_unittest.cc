#include "catch/catch.hpp"
#include "my_algo/my_algo.hpp"

TEST_CASE("threadsafe_stack 简单测试", "[lee][threadsafe_stack]") {
  using namespace lee;
  threadsafe_stack<int> s;
  REQUIRE(s.empty());
  s.push(2);
  REQUIRE(!s.empty());
  s.push(1);
  int i = 0;
  s.pop(i);
  REQUIRE(i == 1);
  auto p = s.pop();
  REQUIRE(*p == 2);

  REQUIRE(s.empty());
}