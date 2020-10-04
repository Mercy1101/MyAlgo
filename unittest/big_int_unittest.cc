#include <iostream>

#include "catch/catch.hpp"
#include "my_algo.hpp"

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

#if false
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