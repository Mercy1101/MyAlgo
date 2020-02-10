#include <catch2/catch.hpp>
#include <iostream>

#include "utility/utility.h"

template <typename CharT>
class MyString : public Lee::ObjectCounter<MyString<CharT>> {};

TEST_CASE("object_counter简单测试", "[utility][object_counter]") {
  MyString<char> s1;
  MyString<char> s2;
  MyString<wchar_t> ws;

  REQUIRE(MyString<char>::live() == 2);
  REQUIRE(MyString<wchar_t>::live() == 1);
}