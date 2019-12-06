#include "utility/utility.h"
#include <catch2/catch.hpp>
#include <cstdlib>  // for RAND_MAX
#include <limits>
#include <string>

TEST_CASE("KMP简单测试", "[utility][KMP]") {
  using namespace Lee;
  std::string test1("10110100100011111010101");
  size_t pos = 0;

  REQUIRE(KMP::Search(test1, "1011", pos));
  REQUIRE(pos == 0);

  REQUIRE(KMP::Search(test1, "1010", pos));
  REQUIRE(pos == 3);

  REQUIRE(KMP::Search(test1, "10110100100011111010101", pos));
  REQUIRE(pos == 0);

  REQUIRE(KMP::Search(test1, "0011", pos));
  REQUIRE(pos == 10);

  REQUIRE(KMP::Search(test1, "10101", pos));
  REQUIRE(pos == 16);
}

TEST_CASE("IsFileExist简单测试", "[utility][IsFileExist]") {
  std::string Path;
  /** 存在的情况 */
  Path = "C:\\Program Files";
  REQUIRE(Lee::IsFileExist(Path));
  Path = "C:\\Program Files (x86)";
  REQUIRE(Lee::IsFileExist(Path));
  Path = "C:\\Windows";
  REQUIRE(Lee::IsFileExist(Path));

  /** 不存在的情况 */
  Path = "C:\\Wws";
  REQUIRE_FALSE(Lee::IsFileExist(Path));
  Path = "C:\\sProgram Files (x86)s";
  REQUIRE_FALSE(Lee::IsFileExist(Path));
  Path = "C:\\sProram Fils";
  REQUIRE_FALSE(Lee::IsFileExist(Path));
}

TEST_CASE("IsMultiOverFlow在int类型下的简单测试",
          "[utility][IsMultiOverFlow]") {
  int x = 0;
  int y = 0;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));
  x = 0;
  y = INT_MAX;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));
  x = INT_MAX;
  y = 0;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));
  x = 1;
  y = INT_MAX;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));
  x = INT_MAX;
  y = 1;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  x = INT_MIN;
  y = 0;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));
  x = INT_MIN;
  y = 1;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  x = INT_MIN;
  y = -1;
  REQUIRE(Lee::IsMultiOverFlow(x, y));
  x = INT_MIN;
  y = -2;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  x = INT_MAX / 5;
  y = 5;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));
  x = 1 + (INT_MAX / 5);
  y = 5;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  x = INT_MIN / 5;
  y = 5;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));
  x = -1 + (INT_MIN / 5);
  y = 5;
  REQUIRE(Lee::IsMultiOverFlow(x, y));
  x = INT_MIN / 5;
  y = -5;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));
  x = -1 + (INT_MIN / 5);
  y = -5;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  x = INT_MAX / 2;
  y = 2;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));
  x = 1 + (INT_MAX / 2);
  y = 2;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  x = INT_MIN / 2;
  y = 2;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));
  x = -1 + (INT_MIN / 2);
  y = 2;
  REQUIRE(Lee::IsMultiOverFlow(x, y));
  x = INT_MIN / 2;
  y = -2;
  REQUIRE(Lee::IsMultiOverFlow(x, y));
  x = -1 + (INT_MIN / 2);
  y = -2;
  REQUIRE(Lee::IsMultiOverFlow(x, y));
}

TEST_CASE("IsMultiOverFlow在long long类型下的溢出测试",
          "[utility][IsMultiOverFlow]") {
  long long x = 0;
  long long y = 0;

  /** LLONG_MIN * -1 溢出了 */
  x = LLONG_MIN;
  y = -1;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  /** LLONG_MIN * -2 溢出了 */
  x = LLONG_MIN;
  y = -2;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  /** （LLONG_MAX / 5 + 1） * 5 溢出了 */
  x = 1 + (LLONG_MAX / 5);
  y = 5;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  /** (LLONG_MIN / 5 - 1) * 5 溢出了 */
  x = -1 + (LLONG_MIN / 5);
  y = 5;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  /** (LLONG_MIN / 5 - 2) * 5 溢出了 */
  x = -2 + (LLONG_MIN / 5);
  y = 5;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  /**(LLONG_MIN / 5 - 1) * -5 溢出了 */
  x = -1 + (LLONG_MIN / 5);
  y = -5;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  /** (LLONG_MAX / 2 + 1) * 2 溢出了 */
  x = 1 + (LLONG_MAX / 2);
  y = 2;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  /**  (LLONG_MIN / 2 - 1) * 2 溢出了 */
  x = -1 + (LLONG_MIN / 2);
  y = 2;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  /**  LLONG_MIN / 2 * -2 溢出了 */
  x = LLONG_MIN / 2;
  y = -2;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  /**  (LLONG_MIN / 2 - 1) * -2 溢出了 */
  x = -1 + (LLONG_MIN / 2);
  y = -2;
  REQUIRE(Lee::IsMultiOverFlow(x, y));
}

TEST_CASE("IsMultiOverFlow在long long类型下没有溢出的测试",
          "[utility][IsMultiOverFlow]") {
  long long x = 0;
  long long y = 0;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  /** LLONG_MAX * 0 没有溢出*/
  x = 0;
  y = LLONG_MAX;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));
  /** LLONG_MAX * 0 没有溢出*/

  x = LLONG_MAX;
  y = 0;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  /** LLONG_MAX * 1 没有溢出*/
  x = 1;
  y = LLONG_MAX;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  /** LLONG_MAX * 1 没有溢出*/
  x = LLONG_MAX;
  y = 1;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  /** LLONG_MIN * 0 没有溢出*/
  x = LLONG_MIN;
  y = 0;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  /** LLONG_MIN * 1 没有溢出*/
  x = LLONG_MIN;
  y = 1;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  /** LLONG_MAX / 5 * 5 没有溢出*/
  x = LLONG_MAX / 5;
  y = 5;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  /** LLONG_MIN / 5 * 5 没有溢出*/
  x = LLONG_MIN / 5;
  y = 5;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  /** LLONG_MIN / 5 * 5 没有溢出*/
  /** LLONG_MIN / 5由于余数被略去了所以LLONG_MIN / 5 * 5 没有溢出 */
  x = LLONG_MIN / 5;
  y = -5;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  /** LLONG_MIN / 2 * 2 没有溢出*/
  x = LLONG_MIN / 2;
  y = 2;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  /** LLONG_MAX / 2 * 2 没有溢出*/
  x = LLONG_MAX / 2;
  y = 2;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));
}

TEST_CASE("IsMultiOverFlow在long long类型下溢出的测试",
          "[utility][IsMultiOverFlow]") {
  /** LLONG_MAX * LLONG_MAX 溢出了 */
  long long x = LLONG_MAX;
  long long y = LLONG_MAX;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  /** LLONG_MAX * LLONG_MIN 溢出了 */
  x = LLONG_MAX;
  y = LLONG_MIN;
  REQUIRE(Lee::IsMultiOverFlow(x, y));
  REQUIRE(Lee::IsMultiOverFlow(y, x));

  /** LLONG_MIN * LLONG_MIN 溢出了 */
  x = LLONG_MIN;
  y = LLONG_MIN;
  REQUIRE(Lee::IsMultiOverFlow(x, y));
}

TEST_CASE("IsMultiOverFlow在unsigned int类型下的测试",
          "[utility][IsMultiOverFlow]") {
  unsigned x = UINT_MAX;
  unsigned y = UINT_MAX;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  x = 1;
  y = UINT_MAX;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  x = 2;
  y = UINT_MAX;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  x = UINT_MAX / 2;
  y = 2;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  x = UINT_MAX / 2 + 1;
  y = 2;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  x = UINT_MAX / 2;
  y = 2;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  x = UINT_MAX / 2 + 1;
  y = 2;
  REQUIRE(Lee::IsMultiOverFlow(x, y));
}

TEST_CASE("IsMultiOverFlow在unsigned long long 类型下的测试",
          "[utility][IsMultiOverFlow]") {
  unsigned long long x = ULLONG_MAX;
  unsigned long long y = ULLONG_MAX;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  x = 1;
  y = ULLONG_MAX;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  x = 2;
  y = ULLONG_MAX;
  REQUIRE(Lee::IsMultiOverFlow(x, y));

  x = ULLONG_MAX / 2;
  y = 2;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  // x = (ULLONG_MAX / 2) + 1; 为什么这句话警告,算数溢出?
  ///                         2019-12-05 13:39:24
  // y = 2;
  // REQUIRE(Lee::IsMultiOverFlow(x, y));

  x = ULLONG_MAX / 5;
  y = 5;
  REQUIRE_FALSE(Lee::IsMultiOverFlow(x, y));

  x = (ULLONG_MAX / 5) + 1;
  y = 5;
  REQUIRE(Lee::IsMultiOverFlow(x, y));
}

TEST_CASE("IsMultiOverFlow在简单测试", "[utility][power]") {
  REQUIRE(Lee::power(0, 2) == 0);

  REQUIRE(Lee::power(2, 0) == 1);
  REQUIRE(Lee::power(2, 5) == 32);
  REQUIRE(Lee::power(2, 7) == 128);
  REQUIRE(Lee::power(2, 10) == 1024);
  REQUIRE(Lee::power(2, 16) == 65536);

  REQUIRE(Lee::power(7, 0) == 1);
  REQUIRE(Lee::power(7, 5) == 16807);
  REQUIRE(Lee::power(7, 7) == 823543);

  /** 零不能乘幂0 */
  // REQUIRE(Lee::power(0, 0), 1);
  /** int 溢出为0 */
  // REQUIRE(Lee::power(2, 32), 0);
  /** 不支持负数次幂 */
  // REQUIRE(Lee::power(2, -1), 1);
}

TEST_CASE("Multiplies_s()例子", "[utility][Multiplies_s]") {
  auto xx0 = Lee::Multiplies_s(2, 4);
  REQUIRE(xx0.has_value());
  REQUIRE(xx0.value() == 2 * 4);

  auto xx1 = Lee::Multiplies_s(INT_MIN, -1);
  REQUIRE_FALSE(xx1.has_value());

  auto xx2 = Lee::Multiplies_s(INT_MIN / 2, 3);
  REQUIRE_FALSE(xx2.has_value());
  REQUIRE(xx2.value_or(1) == 1);
}

TEST_CASE("IsLittleEndian()例子", "[utility][IsLittleEndian]") {
  Lee::IsLittleEndian() ? dbg("电脑是小端的") : dbg("电脑是大端的");
}

TEST_CASE("GetRandom(), 随机函数测试", "[utility][GetRandom]") {
  for (int i = 0; i < 1000; ++i) {
    auto random_number = Lee::GetRandom();
    CHECK((random_number >= 0 && random_number <= RAND_MAX));
  }
}

SCENARIO("GetRandomRange(), 随机函数测试", "[utility][GetRandomRange]") {
  GIVEN("一个入参的测试") {
    WHEN("入参为正整数") {
      THEN("生成的随机数应该大于等于零，小于入参的数值") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = Lee::GetRandomRange(999);
          REQUIRE((random_number >= 0 && random_number <= 999));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = Lee::GetRandomRange(1);
          REQUIRE((random_number >= 0 && random_number <= 1));
        }
      }
    }
    WHEN("入参为零") {
      THEN("只能生成零") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = Lee::GetRandomRange(0);
          REQUIRE((random_number >= 0 && random_number <= 0));
        }
      }
    }
    WHEN("入参为负整数") {
      THEN("生成的随机数应该小于等于零，大于入参的数值") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = Lee::GetRandomRange(-999);
          REQUIRE((random_number >= -999 && random_number <= 0));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = Lee::GetRandomRange(-1);
          REQUIRE((random_number >= -1 && random_number <= 0));
        }
      }
    }
  }
  GIVEN("两个入参的测试") {
    WHEN("入参为正整数") {
      THEN("生成的随机数应该大于等于较小入参的数值，小于较大入参的数值") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = Lee::GetRandomRange(999, 888);
          REQUIRE((random_number >= 888 && random_number <= 999));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = Lee::GetRandomRange(1, 2);
          REQUIRE((random_number >= 1 && random_number <= 2));
        }
      }
    }
    WHEN("入参相等") {
      THEN("只能生成该入参") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = Lee::GetRandomRange(999, 999);
          REQUIRE((random_number >= 999 && random_number <= 999));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = Lee::GetRandomRange(0, 0);
          REQUIRE((random_number >= 0 && random_number <= 0));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = Lee::GetRandomRange(-1, -1);
          REQUIRE((random_number >= -1 && random_number <= -1));
        }
      }
    }
    WHEN("入参为负数整数混合") {
      THEN("生成[x, y]的数值") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = Lee::GetRandomRange(-999, 999);
          REQUIRE((random_number >= -999 && random_number <= 999));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = Lee::GetRandomRange(-1, 1);
          REQUIRE((random_number >= -1 && random_number <= 1));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = Lee::GetRandomRange(0, -100);
          REQUIRE((random_number >= -100 && random_number <= 0));
        }
      }
    }
  }
}