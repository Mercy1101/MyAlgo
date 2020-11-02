#include "catch/catch.hpp"
#include "my_algo/my_algo.hpp"

TEST_CASE("skip_list 简单测试", "[lee][skip_list]") {
  using namespace lee;

  skip_list skiplist;
  skiplist.add(1);
  skiplist.add(2);
  skiplist.add(3);
  REQUIRE(!skiplist.search(0));
  skiplist.add(4);
  REQUIRE(skiplist.search(1));
  REQUIRE(!skiplist.erase(0));   // 返回 false，0 不在跳表中
  REQUIRE(skiplist.erase(1));    // 返回 true
  REQUIRE(!skiplist.search(1));  // 返回 false，1 已被擦除
}
