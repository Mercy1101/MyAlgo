#include "catch/catch.hpp"
#include "my_algo/my_algo.hpp"
#include <climits>

TEST_CASE("is_multi_overflow() 简单测试", "[lee][is_multi_overflow]") {
  using namespace lee;
  int x = 0;
  int y = 0;
  int max_num = std::numeric_limits<int>::max();
  int min_num = std::numeric_limits<int>::min();

  /// case 1 #1
  x = 7;
  y = 1 + max_num / x;
  REQUIRE(is_multi_overflow(x, y));
  x = max_num - 1;
  y = 1 + max_num / x;
  REQUIRE(is_multi_overflow(x, y));

  /// case 1 #2
  x = 7;
  y = max_num / x;
  REQUIRE(!is_multi_overflow(x, y));
  x = max_num - 1;
  y = max_num / x;
  REQUIRE(!is_multi_overflow(x, y));

  /// case 2 #1
  x = -7;
  y = -1 + min_num / -x;
  REQUIRE(is_multi_overflow(x, y));
  x = min_num + 1;
  y = -1 + min_num / -x;
  REQUIRE(is_multi_overflow(x, y));

  /// case 2 #2
  x = -7;
  y = min_num / -x;
  REQUIRE(!is_multi_overflow(x, y));
  x = min_num + 1;
  y = min_num / -x;
  REQUIRE(!is_multi_overflow(x, y));

  /// case 3
  x = 0;
  y = max_num;
  REQUIRE(!is_multi_overflow(x, y));
  x = min_num;
  y = 0;
  REQUIRE(!is_multi_overflow(x, y));
  x = 0;
  y = 0;
  REQUIRE(!is_multi_overflow(x, y));

  /// case 4
  x = 1;
  y = max_num;
  REQUIRE(!is_multi_overflow(x, y));
  x = INT_MIN;
  y = 1;
  REQUIRE(!is_multi_overflow(x, y));

  /// case 5
  x = -1;
  y = max_num;
  REQUIRE(!is_multi_overflow(x, y));
  x = -1;
  y = min_num;
  REQUIRE(is_multi_overflow(x, y));

  /// case 6
  x = 2;
  y = min_num / 2;
  REQUIRE(!is_multi_overflow(x, y));
  REQUIRE(!is_multi_overflow(y, x));
  x = 2;
  y = -1 + min_num / 2;
  REQUIRE(is_multi_overflow(x, y));
  REQUIRE(is_multi_overflow(y, x));
}
