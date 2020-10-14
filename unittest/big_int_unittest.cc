#include <iostream>

#include "catch/catch.hpp"
#include "my_algo/my_algo.hpp"

TEST_CASE("big_int() 默认构造函数测试", "[lee][big_int]") {
  using namespace lee;
  big_int temp;
  REQUIRE(temp.to_string() == "0");
}

TEST_CASE("big_int(long long) 构造函数测试, 正数入参", "[lee][big_int]") {
  using namespace lee;
  long long n = static_cast<long long>(1e9);
  big_int num(n);
  REQUIRE(num.to_string() == "1000000000");
  big_int num1(n + 1);
  REQUIRE(num1.to_string() == "1000000001");
  big_int num2(0);
  REQUIRE(num2.to_string() == "0");
  auto max = std::numeric_limits<long long>::max();
  big_int num3(max);
  REQUIRE(num3.to_string() == std::to_string(max));
}

TEST_CASE("big_int(long long) 构造函数测试, 负数入参", "[lee][big_int]") {
  using namespace lee;
  long long n = static_cast<long long>(-1e9);
  big_int num(n);
  REQUIRE(num.to_string() == "-1000000000");
  big_int num1(n - 1);
  REQUIRE(num1.to_string() == "-1000000001");
  big_int num2(n + 1);
  REQUIRE(num2.to_string() == "-999999999");
  big_int num3(-0);
  REQUIRE(num3.to_string() == "0");
  auto min = std::numeric_limits<long long>::lowest();
  big_int num4(min);
  REQUIRE(num4.to_string() == std::to_string(min));
}

TEST_CASE("big_int(const big_int&) 拷贝构造函数测试", "[lee][big_int]") {
  using namespace lee;
  long long n = static_cast<long long>(1e9);
  big_int num(n);
  big_int copy(num);
  REQUIRE(num == copy);
  REQUIRE(num.to_string() == "1000000000");
  REQUIRE(copy.to_string() == "1000000000");
  num += 1;
  REQUIRE(num != copy);
  REQUIRE(num.to_string() == "1000000001");
  REQUIRE(copy.to_string() == "1000000000");
}

TEST_CASE("big_int operator=(const big_int&) 拷贝构造符测试",
          "[lee][big_int]") {
  using namespace lee;
  long long n = static_cast<long long>(1e9);
  big_int num1(n);
  big_int copy1 = num1;
  REQUIRE(num1 == copy1);
  REQUIRE(num1.to_string() == "1000000000");
  REQUIRE(copy1.to_string() == "1000000000");
  num1 += 1;
  REQUIRE(num1 != copy1);
  REQUIRE(num1.to_string() == "1000000001");
  REQUIRE(copy1.to_string() == "1000000000");
}

TEST_CASE("big_int(const std::string&) 构造函数测试", "[lee][big_int]") {
  using namespace lee;
  std::string str("111111111111111");
  big_int bt(str);
  REQUIRE(bt.to_string() == str);

  std::string str2("11111111111111111111111111111111111");
  big_int bt2(str2);
  REQUIRE(bt2.to_string() == str2);

  std::string str3("0");
  big_int bt3(str3);
  REQUIRE(bt3.to_string() == str3);

  std::string str4("-0");
  big_int bt4(str4);
  REQUIRE(bt4.to_string() == "0");

  std::string str5("-1000000000");
  big_int bt5(str5);
  REQUIRE(bt5.to_string() == str5);

  std::string str6("-10000000000");
  big_int bt6(str6);
  REQUIRE(bt6.to_string() == str6);
}

TEST_CASE("big_int::to_string() 函数测试", "[lee][big_int]") {
  using namespace lee;
  big_int bt("");
  REQUIRE(bt.to_string() == "0");

  std::string str0("1");
  big_int bt0(str0);
  REQUIRE(bt0.to_string() == str0);

  std::string str1("00000000000000000000000001");
  big_int bt1(str1);
  REQUIRE(bt1.to_string() == "1");

  std::string str2 = "-10";
  big_int bt2(str2);
  REQUIRE(bt2.to_string() == str2);

  std::string str3(
      "10000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "0000000000");
  big_int bt3(str3);
  REQUIRE(bt3.to_string() == str3);

  std::string str4(
      "-10000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "000000000000000000000000000000000000000000000000000000000000000000000000"
      "0000000000");
  big_int bt4(str4);
  REQUIRE(bt4.to_string() == str4);
}

TEST_CASE("big_int::compare() 函数测试", "[lee][big_int]") {
  using namespace lee;
  auto n = static_cast<long long>(1e9);
  auto min = std::numeric_limits<long long>::lowest();
  auto max = std::numeric_limits<long long>::max();
  big_int zero("0");
  big_int num(n);
  big_int num1(n + 1);
  big_int num2(n - 1);
  big_int long_max(max);
  big_int long_min(min);

  REQUIRE(zero.compare(zero) == 0);
  REQUIRE(zero.compare(num) == -1);
  REQUIRE(zero.compare(num1) == -1);
  REQUIRE(zero.compare(num2) == -1);
  REQUIRE(zero.compare(long_max) == -1);
  REQUIRE(zero.compare(min) == 1);

  REQUIRE(num.compare(num1) == -1);
  REQUIRE(num1.compare(num) == 1);

  REQUIRE(long_max.compare(long_min) == 1);
  REQUIRE(long_min.compare(long_max) == -1);

  big_int num3("100000000000000000000000000000000000000000000");
  big_int num4 = num3 + 1;
  big_int num5 = num3 - 1;
  REQUIRE(num3.compare(num4) == -1);
  REQUIRE(num3.compare(num3) == 0);
  REQUIRE(num3.compare(num5) == 1);
}

TEST_CASE("big_int::operator+(long long) 函数测试", "[lee][big_int]") {
  using namespace lee;
  auto n = static_cast<long long>(1e9);
  big_int zero("0");
  big_int num(n);
  big_int num1(n + 1);
  big_int num2(-1 * n);
  big_int num3("-999999999");

  REQUIRE((zero + zero).to_string() == "0");
  REQUIRE((zero + zero) == zero);
  REQUIRE(zero == 0);

  REQUIRE((num + 1).to_string() == "1000000001");
  REQUIRE((num + 1) == num1);
  REQUIRE(num == big_int(n));

  REQUIRE((num2 + 1).to_string() == "-999999999");
  REQUIRE((num2 + 1) == num3);
  REQUIRE(num2 == big_int(-1 * n));
}

TEST_CASE("big_int::operator+=(long long) 函数测试", "[lee][big_int]") {
  using namespace lee;
  auto n = static_cast<long long>(1e9);
  big_int zero(0);
  big_int num(n);
  big_int num1(n + 1);
  big_int num2(-1 * n);
  big_int num3("-999999999");

  REQUIRE((zero += zero) == zero);
  REQUIRE(zero == big_int(0));

  REQUIRE((num += 1) == num1);
  REQUIRE(num == num1);

  REQUIRE((num2 += 1) == num3);
  REQUIRE(num2 == num3);
}

TEST_CASE("big_int::operator+(const big_int&) 函数测试", "[lee][big_int]") {
  using namespace lee;
  auto n = static_cast<long long>(1e9);
  big_int zero("0");
  big_int num(n);
  big_int num1(n + 1);
  big_int num2(-1 * n);
  big_int num3("-999999999");
  big_int one(1);

  REQUIRE((zero + zero).to_string() == "0");
  REQUIRE((zero + zero) == zero);
  REQUIRE(zero == 0);

  REQUIRE((num + one).to_string() == "1000000001");
  REQUIRE((num + one) == num1);
  REQUIRE(num == big_int(n));

  REQUIRE((num2 + one).to_string() == "-999999999");
  REQUIRE((num2 + one) == num3);
  REQUIRE(num2 == big_int(-1 * n));
}

TEST_CASE("big_int::operator+=(const big_int&) 函数测试", "[lee][big_int]") {
  using namespace lee;
  auto n = static_cast<long long>(1e9);
  big_int zero(0);
  big_int num(n);
  big_int num1(n + 1);
  big_int num2(-1 * n);
  big_int num3("-999999999");
  big_int one(1);

  REQUIRE((zero += zero) == zero);
  REQUIRE(zero == big_int(0));

  REQUIRE((num += one) == num1);
  REQUIRE(num == num1);

  REQUIRE((num2 += one) == num3);
  REQUIRE(num2 == num3);
}

TEST_CASE("big_int::operator-(long long) 函数测试", "[lee][big_int]") {
  using namespace lee;
  auto n = static_cast<long long>(1e9);
  big_int zero("0");
  big_int num(n);
  big_int num1(n - 1);
  big_int num2(-1 * n);
  big_int num3(-1 * n - 1);

  REQUIRE((zero - zero).to_string() == "0");
  REQUIRE((zero - zero) == zero);
  REQUIRE(zero == 0);

  REQUIRE((num - 1).to_string() == "999999999");
  REQUIRE((num - 1) == num1);
  REQUIRE(num == big_int(n));

  REQUIRE((num2 - 1).to_string() == "-1000000001");
  REQUIRE((num2 - 1) == num3);
  REQUIRE(num2 == big_int("-1000000000"));
}

TEST_CASE("big_int::operator-=(long long) 函数测试", "[lee][big_int]") {
  using namespace lee;
  auto n = static_cast<long long>(1e9);
  big_int zero(0);
  big_int num(n);
  big_int num1(n - 1);
  big_int num2(-1 * n);
  big_int num3("-1000000001");

  REQUIRE((zero -= zero) == zero);
  REQUIRE(zero == big_int(0));

  REQUIRE((num -= 1) == num1);
  REQUIRE(num == num1);

  REQUIRE((num2 -= 1) == num3);
  REQUIRE(num2 == num3);
}

TEST_CASE("big_int::operator-(const big_int&) 函数测试", "[lee][big_int]") {
  using namespace lee;
  auto n = static_cast<long long>(1e9);
  big_int zero("0");
  big_int num(n);
  big_int num1(n - 1);
  big_int num2(-1 * n);
  big_int num3("-1000000001");
  big_int one(1);

  REQUIRE((zero - zero).to_string() == "0");
  REQUIRE((zero - zero) == zero);
  REQUIRE(zero == 0);

  REQUIRE((num - one).to_string() == "999999999");
  REQUIRE((num - one) == num1);
  REQUIRE(num == big_int(n));

  REQUIRE((num2 - one).to_string() == "-1000000001");
  REQUIRE((num2 - one) == num3);
  REQUIRE(num2 == big_int(-1 * n));
}

TEST_CASE("big_int::operator-=(const big_int&) 函数测试", "[lee][big_int]") {
  using namespace lee;
  auto n = static_cast<long long>(1e9);
  big_int zero(0);
  big_int num(n);
  big_int num1(n - 1);
  big_int num2(-1 * n);
  big_int num3("-1000000001");
  big_int one(1);

  REQUIRE((zero -= zero) == zero);
  REQUIRE(zero == big_int(0));

  REQUIRE((num -= one) == num1);
  REQUIRE(num == num1);

  REQUIRE((num2 -= one) == num3);
  REQUIRE(num2 == num3);
}

#if false
TEST_CASE("big_int::operator*() 函数测试", "[lee][big_int]") {
  using namespace lee;
  big_int zero(0);
  big_int one(1);
  big_int two(2);

  auto n = static_cast<long long>(1e9);
  big_int base0(n);
  big_int base1(n - 1);
  big_int base2(n + 1);
  auto max = std::numeric_limits<long long>::max();
  auto min = std::numeric_limits<long long>::min();

  /// 乘以0
  REQUIRE((zero * zero) == zero);
  REQUIRE((zero * zero).to_string() == "0");
  REQUIRE((zero * one) == zero);
  REQUIRE((zero * one).to_string() == "0");
  REQUIRE((zero * base0) == zero);
  REQUIRE((zero * base0).to_string() == "0");
  REQUIRE((zero * base1) == zero);
  REQUIRE((zero * base1).to_string() == "0");
  REQUIRE((zero * base2) == zero);
  REQUIRE((zero * base2).to_string() == "0");
  REQUIRE((zero * max) == zero);
  REQUIRE((zero * max).to_string() == "0");
  REQUIRE((zero * min) == zero);
  REQUIRE((zero * min).to_string() == "0");

  /// 乘以1
  REQUIRE((one * one) == one);
  REQUIRE((one * one).to_string() == "1");
  REQUIRE((one * base0) == base1);
  REQUIRE((one * base0).to_string() == "1000000000");
  REQUIRE((one * base1) == base1);
  REQUIRE((one * base1).to_string() == "999999999");
  REQUIRE((one * base2) == base2);
  REQUIRE((one * base2).to_string() == "1000000001");
  REQUIRE((one * max) == max);
  REQUIRE((one * min) == min);

  /// 乘以2
  REQUIRE((two * 2) == 4);
  REQUIRE((two * two) == 4);
  REQUIRE((base2 * 2) == 2000000002);
  REQUIRE((base2 * two) == 2000000002);
}

TEST_CASE("big_int::pow() 函数测试", "[lee][big_int]") {
  using namespace lee;
  big_int x(2);
  x.pow(0);
  REQUIRE(x.pow(0).to_string() == "1");
  REQUIRE(x.to_string() == "1");

  big_int x1(2);
  REQUIRE(x1.pow(1).to_string() == "2");
  REQUIRE(x1.to_string() == "2");

  big_int x2(2);
  REQUIRE(x2.pow(4).to_string() == "16");
  REQUIRE(x2.to_string() == "16");

  big_int x3("1000000000");
  REQUIRE(x3.pow(2).to_string() == "1000000000000000000");
  REQUIRE(x3.to_string() == "1000000000000000000");

  /// 不支持负指数幂
  /// x.pow(-1);
}

TEST_CASE("big_int::operator+(big_int&) 函数测试", "[lee][big_int]") {
  using namespace lee;
  big_int bt0("1234567812345678");
  big_int bt1("8765432187654321");
  REQUIRE((bt0 + bt1).to_string() == "9999999999999999");

  big_int bt2("-1234567812345678");
  big_int bt3("8765432187654321");
  REQUIRE((bt2 + bt3).to_string() == "7530864375308643");

  big_int bt4("1234567812345678");
  big_int bt5("-8765432187654321");
  REQUIRE((bt4 + bt5).to_string() == "-7530864375308643");

  big_int bt6("1");
  big_int bt7("-1");
  REQUIRE((bt6 + bt7).to_string() == "0");

  big_int bt8("0");
  big_int bt9("0");
  REQUIRE((bt8 + bt9).to_string() == "0");
}
#endif