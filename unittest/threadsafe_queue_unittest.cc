#include "catch/catch.hpp"
#include "my_algo/my_algo.hpp"

TEST_CASE("threadsafe_queue 简单测试", "[lee][threadsafe_queue]") {
  using namespace lee;
  threadsafe_queue<int> q;
  REQUIRE(q.empty());
  q.push(1);
  REQUIRE(!q.empty());
  int i = 0;
  REQUIRE(q.try_pop(i));
  REQUIRE(i == 1);

  q.push(2);
  auto s = q.try_pop();
  REQUIRE(*s == 2);

  q.push(3);
  auto s1 = q.wait_and_pop();
  REQUIRE(*s1 == 3);

  REQUIRE(q.empty());
}