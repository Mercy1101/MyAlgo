﻿#include "utility/utility.h"

#include <catch2/catch.hpp>
#include <cstdlib>  // for RAND_MAX
#include <limits>
#include <string>

TEST_CASE("KMP简单测试", "[utility][KMP]") {
  using namespace lee;
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

TEST_CASE("IsMultiOverFlow在int类型下的简单测试",
          "[utility][IsMultiOverFlow]") {
  int x = 0;
  int y = 0;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));
  x = 0;
  y = INT_MAX;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));
  x = INT_MAX;
  y = 0;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));
  x = 1;
  y = INT_MAX;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));
  x = INT_MAX;
  y = 1;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  x = INT_MIN;
  y = 0;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));
  x = INT_MIN;
  y = 1;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  x = INT_MIN;
  y = -1;
  REQUIRE(lee::IsMultiOverFlow(x, y));
  x = INT_MIN;
  y = -2;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  x = INT_MAX / 5;
  y = 5;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));
  x = 1 + (INT_MAX / 5);
  y = 5;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  x = INT_MIN / 5;
  y = 5;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));
  x = -1 + (INT_MIN / 5);
  y = 5;
  REQUIRE(lee::IsMultiOverFlow(x, y));
  x = INT_MIN / 5;
  y = -5;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));
  x = -1 + (INT_MIN / 5);
  y = -5;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  x = INT_MAX / 2;
  y = 2;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));
  x = 1 + (INT_MAX / 2);
  y = 2;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  x = INT_MIN / 2;
  y = 2;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));
  x = -1 + (INT_MIN / 2);
  y = 2;
  REQUIRE(lee::IsMultiOverFlow(x, y));
  x = INT_MIN / 2;
  y = -2;
  REQUIRE(lee::IsMultiOverFlow(x, y));
  x = -1 + (INT_MIN / 2);
  y = -2;
  REQUIRE(lee::IsMultiOverFlow(x, y));
}

TEST_CASE("IsMultiOverFlow在long long类型下的溢出测试",
          "[utility][IsMultiOverFlow]") {
  long long x = 0;
  long long y = 0;

  /** LLONG_MIN * -1 溢出了 */
  x = LLONG_MIN;
  y = -1;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  /** LLONG_MIN * -2 溢出了 */
  x = LLONG_MIN;
  y = -2;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  /** （LLONG_MAX / 5 + 1） * 5 溢出了 */
  x = 1 + (LLONG_MAX / 5);
  y = 5;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  /** (LLONG_MIN / 5 - 1) * 5 溢出了 */
  x = -1 + (LLONG_MIN / 5);
  y = 5;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  /** (LLONG_MIN / 5 - 2) * 5 溢出了 */
  x = -2 + (LLONG_MIN / 5);
  y = 5;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  /**(LLONG_MIN / 5 - 1) * -5 溢出了 */
  x = -1 + (LLONG_MIN / 5);
  y = -5;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  /** (LLONG_MAX / 2 + 1) * 2 溢出了 */
  x = 1 + (LLONG_MAX / 2);
  y = 2;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  /**  (LLONG_MIN / 2 - 1) * 2 溢出了 */
  x = -1 + (LLONG_MIN / 2);
  y = 2;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  /**  LLONG_MIN / 2 * -2 溢出了 */
  x = LLONG_MIN / 2;
  y = -2;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  /**  (LLONG_MIN / 2 - 1) * -2 溢出了 */
  x = -1 + (LLONG_MIN / 2);
  y = -2;
  REQUIRE(lee::IsMultiOverFlow(x, y));
}

TEST_CASE("IsMultiOverFlow在long long类型下没有溢出的测试",
          "[utility][IsMultiOverFlow]") {
  long long x = 0;
  long long y = 0;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  /** LLONG_MAX * 0 没有溢出*/
  x = 0;
  y = LLONG_MAX;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  /** LLONG_MAX * 0 没有溢出*/
  x = LLONG_MAX;
  y = 0;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  /** LLONG_MAX * 1 没有溢出*/
  x = 1;
  y = LLONG_MAX;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  /** LLONG_MAX * 1 没有溢出*/
  x = LLONG_MAX;
  y = 1;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  /** LLONG_MIN * 0 没有溢出*/
  x = LLONG_MIN;
  y = 0;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  /** LLONG_MIN * 1 没有溢出*/
  x = LLONG_MIN;
  y = 1;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  /** LLONG_MAX / 5 * 5 没有溢出*/
  x = LLONG_MAX / 5;
  y = 5;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  /** LLONG_MIN / 5 * 5 没有溢出*/
  x = LLONG_MIN / 5;
  y = 5;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  /** LLONG_MIN / 5 * 5 没有溢出*/
  /** LLONG_MIN / 5由于余数被略去了所以LLONG_MIN / 5 * 5 没有溢出 */
  x = LLONG_MIN / 5;
  y = -5;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  /** LLONG_MIN / 2 * 2 没有溢出*/
  x = LLONG_MIN / 2;
  y = 2;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  /** LLONG_MAX / 2 * 2 没有溢出*/
  x = LLONG_MAX / 2;
  y = 2;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));
}

TEST_CASE("IsMultiOverFlow在long long类型下溢出的测试",
          "[utility][IsMultiOverFlow]") {
  /** LLONG_MAX * LLONG_MAX 溢出了 */
  long long x = LLONG_MAX;
  long long y = LLONG_MAX;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  /** LLONG_MAX * LLONG_MIN 溢出了 */
  x = LLONG_MAX;
  y = LLONG_MIN;
  REQUIRE(lee::IsMultiOverFlow(x, y));
  REQUIRE(lee::IsMultiOverFlow(y, x));

  /** LLONG_MIN * LLONG_MIN 溢出了 */
  x = LLONG_MIN;
  y = LLONG_MIN;
  REQUIRE(lee::IsMultiOverFlow(x, y));
}

TEST_CASE("IsMultiOverFlow在unsigned int类型下的测试",
          "[utility][IsMultiOverFlow]") {
  unsigned x = UINT_MAX;
  unsigned y = UINT_MAX;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  x = 1;
  y = UINT_MAX;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  x = 2;
  y = UINT_MAX;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  x = UINT_MAX / 2;
  y = 2;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  x = UINT_MAX / 2 + 1;
  y = 2;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  x = UINT_MAX / 2;
  y = 2;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  x = UINT_MAX / 2 + 1;
  y = 2;
  REQUIRE(lee::IsMultiOverFlow(x, y));
}

TEST_CASE("IsMultiOverFlow在unsigned long long 类型下的测试",
          "[utility][IsMultiOverFlow]") {
  unsigned long long x = ULLONG_MAX;
  unsigned long long y = ULLONG_MAX;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  x = 1;
  y = ULLONG_MAX;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  x = 2;
  y = ULLONG_MAX;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  x = ULLONG_MAX / 2;
  y = 2;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  x = (ULLONG_MAX / 2) + 1;
  y = 2;
  REQUIRE(lee::IsMultiOverFlow(x, y));

  x = ULLONG_MAX / 5;
  y = 5;
  REQUIRE_FALSE(lee::IsMultiOverFlow(x, y));

  x = (ULLONG_MAX / 5) + 1;
  y = 5;
  REQUIRE(lee::IsMultiOverFlow(x, y));
}

TEST_CASE("IsMultiOverFlow在简单测试", "[utility][power]") {
  REQUIRE(lee::power(0, 2) == 0);

  REQUIRE(lee::power(2, 0) == 1);
  REQUIRE(lee::power(2, 5) == 32);
  REQUIRE(lee::power(2, 7) == 128);
  REQUIRE(lee::power(2, 10) == 1024);
  REQUIRE(lee::power(2, 16) == 65536);

  REQUIRE(lee::power(7, 0) == 1);
  REQUIRE(lee::power(7, 5) == 16807);
  REQUIRE(lee::power(7, 7) == 823543);

  /** 零不能乘幂0 */
  // REQUIRE(lee::power(0, 0), 1);
  /** int 溢出为0 */
  // REQUIRE(lee::power(2, 32), 0);
  /** 不支持负数次幂 */
  // REQUIRE(lee::power(2, -1), 1);
}

TEST_CASE("Multiplies_s()例子", "[utility][Multiplies_s]") {
  auto xx0 = lee::Multiplies_s(2, 4);
  REQUIRE(xx0.has_value());
  REQUIRE(xx0.value() == 2 * 4);

  auto xx1 = lee::Multiplies_s(INT_MIN, -1);
  REQUIRE_FALSE(xx1.has_value());

  auto xx2 = lee::Multiplies_s(INT_MIN / 2, 3);
  REQUIRE_FALSE(xx2.has_value());
  REQUIRE(xx2.value_or(1) == 1);
}

TEST_CASE("GetRandom(), 随机函数测试", "[utility][GetRandom]") {
  for (int i = 0; i < 1000; ++i) {
    auto random_number = lee::GetRandom();
    CHECK((random_number >= 0 && random_number <= RAND_MAX));
  }
}

SCENARIO("GetRandomRange(), 随机函数测试", "[utility][GetRandomRange]") {
  GIVEN("一个入参的测试") {
    WHEN("入参为正整数") {
      THEN("生成的随机数应该大于等于零，小于入参的数值") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandom(999);
          REQUIRE((random_number >= 0 && random_number <= 999));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandom(1);
          REQUIRE((random_number >= 0 && random_number <= 1));
        }
      }
    }
    WHEN("入参为零") {
      THEN("只能生成零") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandom(0);
          REQUIRE((random_number >= 0 && random_number <= 0));
        }
      }
    }
    WHEN("入参为负整数") {
      THEN("生成的随机数应该小于等于零，大于入参的数值") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandom(-999);
          REQUIRE((random_number >= -999 && random_number <= 0));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandom(-1);
          REQUIRE((random_number >= -1 && random_number <= 0));
        }
      }
    }
  }
  GIVEN("两个入参的测试") {
    WHEN("入参为正整数") {
      THEN("生成的随机数应该大于等于较小入参的数值，小于较大入参的数值") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandom(999, 888);
          REQUIRE((random_number >= 888 && random_number <= 999));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandom(1, 2);
          REQUIRE((random_number >= 1 && random_number <= 2));
        }
      }
    }
    WHEN("入参相等") {
      THEN("只能生成该入参") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandom(999, 999);
          REQUIRE((random_number >= 999 && random_number <= 999));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandom(0, 0);
          REQUIRE((random_number >= 0 && random_number <= 0));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandom(-1, -1);
          REQUIRE((random_number >= -1 && random_number <= -1));
        }
      }
    }
    WHEN("入参为负数整数混合") {
      THEN("生成[x, y]的数值") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandom(-999, 999);
          REQUIRE((random_number >= -999 && random_number <= 999));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandom(-1, 1);
          REQUIRE((random_number >= -1 && random_number <= 1));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandom(0, -100);
          REQUIRE((random_number >= -100 && random_number <= 0));
        }
      }
    }
  }
}

SCENARIO("GetRangeRandomNumberInt(), 等概率生成整数函数测试",
         "[utility][GetRangeRandomNumberFloat]") {
  GIVEN("一个入参的测试") {
    WHEN("入参为正整数") {
      THEN("生成的随机数应该大于等于零，小于入参的数值") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberInt(999);
          REQUIRE((random_number >= 0 && random_number <= 999));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberInt(1);
          REQUIRE((random_number >= 0 && random_number <= 1));
        }
      }
    }
    WHEN("入参为零") {
      THEN("只能生成零") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberInt(0);
          REQUIRE((random_number >= 0 && random_number <= 0));
        }
      }
    }
    WHEN("入参为负整数") {
      THEN("生成的随机数应该小于等于零，大于入参的数值") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberInt(-999);
          REQUIRE((random_number >= -999 && random_number <= 0));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberInt(-1);
          REQUIRE((random_number >= -1 && random_number <= 0));
        }
      }
    }
  }
  GIVEN("两个入参的测试") {
    WHEN("入参为正整数") {
      THEN("生成的随机数应该大于等于较小入参的数值，小于较大入参的数值") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberInt(999, 888);
          REQUIRE((random_number >= 888 && random_number <= 999));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberInt(1, 2);
          REQUIRE((random_number >= 1 && random_number <= 2));
        }
      }
    }
    WHEN("入参相等") {
      THEN("只能生成该入参") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberInt(999, 999);
          REQUIRE((random_number >= 999 && random_number <= 999));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberInt(0, 0);
          REQUIRE((random_number >= 0 && random_number <= 0));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberInt(-1, -1);
          REQUIRE((random_number >= -1 && random_number <= -1));
        }
      }
    }
    WHEN("入参为负数整数混合") {
      THEN("生成[x, y]的数值") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberInt(-999, 999);
          REQUIRE((random_number >= -999 && random_number <= 999));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberInt(-1, 1);
          REQUIRE((random_number >= -1 && random_number <= 1));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberInt(0, -100);
          REQUIRE((random_number >= -100 && random_number <= 0));
        }
      }
    }
  }
}

SCENARIO("GetRangeRandomNumberFloat(), 等概率生成实数(浮点数)函数测试",
         "[utility][GetRangeRandomNumberFloat]") {
  GIVEN("一个入参的测试") {
    WHEN("入参为正浮点数") {
      THEN("生成的随机数应该大于等于零，小于入参的数值") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberFloat(999.996);
          auto random_number2 = lee::GetRangeRandomNumberFloat(999.996);
          REQUIRE((random_number >= 0.0 && random_number <= 999.996));
          REQUIRE(random_number != random_number2);
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberFloat(1.0);
          auto random_number2 = lee::GetRangeRandomNumberFloat(1.0);
          REQUIRE((random_number >= 0 && random_number <= 1.0));
          REQUIRE(random_number != random_number2);
        }
      }
    }
    WHEN("入参为零") {
      THEN("只能生成零") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberFloat(0);
          REQUIRE(random_number == Approx(0));
        }
      }
    }
    WHEN("入参为负浮点数") {
      THEN("生成的随机数应该小于等于零，大于入参的数值") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberFloat(-999.996);
          auto random_number2 = lee::GetRangeRandomNumberFloat(-999.996);
          REQUIRE((random_number >= -999.996 && random_number <= 0));
          REQUIRE(random_number != random_number2);
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberFloat(-1.0);
          auto random_number2 = lee::GetRangeRandomNumberFloat(-1.0);
          REQUIRE((random_number >= -1.0 && random_number <= 0.0));
          REQUIRE(random_number != random_number2);
        }
      }
    }
  }
  GIVEN("两个入参的测试") {
    WHEN("入参为正浮点数") {
      THEN("生成的随机数应该大于等于较小入参的数值，小于较大入参的数值") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberFloat(999.669, 888.669);
          auto random_number2 =
              lee::GetRangeRandomNumberFloat(999.669, 888.669);
          REQUIRE((random_number >= 888.669 && random_number <= 999.669));
          REQUIRE(random_number != random_number2);
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberFloat(1.0, 2.0);
          auto random_number2 = lee::GetRangeRandomNumberFloat(1.0, 2.0);
          REQUIRE((random_number >= 1.0 && random_number <= 2.0));
          REQUIRE(random_number != random_number2);
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberFloat(1.0, 0.0);
          auto random_number2 = lee::GetRangeRandomNumberFloat(1.0, 0.0);
          REQUIRE((random_number >= 0.0 && random_number <= 1.0));
          REQUIRE(random_number != random_number2);
        }
      }
    }
    WHEN("入参相等") {
      THEN("只能生成该入参") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberFloat(999.997, 999.997);
          REQUIRE(random_number == Approx(999.997));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberFloat(0.0, 0.0);
          REQUIRE(random_number == Approx(0.0));
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberFloat(-1.0, -1.0);
          REQUIRE(random_number == Approx(-1.0));
        }
      }
    }
    WHEN("入参为负数整数混合") {
      THEN("生成[x, y]的数值") {
        for (int i = 0; i < 1000; ++i) {
          auto random_number =
              lee::GetRangeRandomNumberFloat(-999.996, 999.996);
          auto random_number2 =
              lee::GetRangeRandomNumberFloat(-999.996, 999.996);
          REQUIRE((random_number >= -999.996 && random_number <= 999.996));
          REQUIRE(random_number != random_number2);
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberFloat(-1.0, 1.0);
          auto random_number2 = lee::GetRangeRandomNumberFloat(-1.0, 1.0);
          REQUIRE((random_number >= -1.0 && random_number <= 1.0));
          REQUIRE(random_number != random_number2);
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberFloat(0.0, -100.0);
          auto random_number2 = lee::GetRangeRandomNumberFloat(0.0, -100.0);
          REQUIRE((random_number >= -100.0 && random_number <= 0.0));
          REQUIRE(random_number != random_number2);
        }
        for (int i = 0; i < 1000; ++i) {
          auto random_number = lee::GetRangeRandomNumberFloat(-0.001, 0.001);
          auto random_number2 = lee::GetRangeRandomNumberFloat(-0.001, 0.001);
          REQUIRE((random_number >= -0.001 && random_number <= 0.001));
          REQUIRE(random_number != random_number2);
        }
      }
    }
  }
}

TEST_CASE("lee::GetRandomPassWord 的例子展示", "[utility][GetRandomPassWord]") {
}

TEMPLATE_TEST_CASE("arraysize 的模板测试", "[utility][arraysize]", int,
                   long long, unsigned long, double) {
  TestType array_16[16];
  REQUIRE(arraysize(array_16) == 16);
  TestType array_VLA[] = {0, 1, 2};
  REQUIRE(arraysize(array_VLA) == 3);
}

SCENARIO("is_plus_overflow 简单测试", "[utility][is_plus_overflow]") {
#undef min
  int x;
  int y;
  constexpr int min_int = std::numeric_limits<int>::min();
  constexpr int max_int = std::numeric_limits<int>::max();
  GIVEN("没有溢出的情况: int 类型") {
    WHEN("Input: x = 1, y = 1") {
      THEN("Output: false") {
        x = 1;
        y = 1;
        REQUIRE(!lee::is_plus_overflow(x, y));
      }  ///< THEN
    }    ///< WHEN
    WHEN("Input: x = -1, y = -1") {
      THEN("Output: false") {
        x = -1;
        y = -1;
        REQUIRE(!lee::is_plus_overflow(x, y));
      }  ///< THEN
    }    ///< WHEN
    WHEN("Input: x = max_int, y = 0") {
      THEN("Output: false") {
        x = max_int;
        y = 0;
        REQUIRE(!lee::is_plus_overflow(x, y));
      }  ///< THEN
    }    ///< WHEN
    WHEN("Input: x = max, y = min") {
      THEN("Output: false") {
        x = max_int;
        y = min_int;
        REQUIRE(!lee::is_plus_overflow(x, y));
      }  ///< THEN
    }    ///< WHEN
    WHEN("Input: x = 0, y = max") {
      THEN("Output: false") {
        x = 0;
        y = max_int;
        REQUIRE(!lee::is_plus_overflow(x, y));
      }  ///< THEN
    }    ///< WHEN
    WHEN("Input: x = 0, y = 0") {
      THEN("Output: false") {
        x = 0;
        y = 0;
        REQUIRE(!lee::is_plus_overflow(x, y));
      }  ///< THEN
    }    ///< WHEN
    WHEN("Input: x = 0, y = min") {
      THEN("Output: false") {
        x = 0;
        y = min_int;
        REQUIRE(!lee::is_plus_overflow(x, y));
      }  ///< THEN
    }    ///< WHEN
    WHEN("Input: x = min, y = max") {
      THEN("Output: false") {
        x = min_int;
        y = max_int;
        REQUIRE(!lee::is_plus_overflow(x, y));
      }  ///< THEN
    }    ///< WHEN
    WHEN("Input: x = min, y = 0") {
      THEN("Output: false") {
        x = min_int;
        y = 0;
        REQUIRE(!lee::is_plus_overflow(x, y));
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN

  GIVEN("溢出的情况") {
    WHEN("Input: x = max, y = 1") {
      THEN("Output: true") {
        x = max_int;
        y = 1;
        REQUIRE(lee::is_plus_overflow(x, y));
      }  ///< THEN
    }    ///< WHEN
    WHEN("Input: x = min, y = -1") {
      THEN("Output: true") {
        x = min_int;
        y = -1;
        REQUIRE(lee::is_plus_overflow(x, y));
      }  ///< THEN
    }    ///< WHEN
    WHEN("Input: x = min, y = min") {
      THEN("Output: true") {
        x = min_int;
        y = min_int;
        REQUIRE(lee::is_plus_overflow(x, y));
      }  ///< THEN
    }    ///< WHEN
    WHEN("Input: x = max, y = max") {
      THEN("Output: true") {
        x = max_int;
        y = max_int;
        REQUIRE(lee::is_plus_overflow(x, y));
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN
}  ///< SCENARIO
