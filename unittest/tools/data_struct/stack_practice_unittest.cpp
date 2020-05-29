#include "data_struct/stack_practice.h"

#include <catch2/catch.hpp>

SCENARIO("四则运算的简单测试", "[data_struct][stack_practice]") {
  Lee::simple_calculate calculator;
  GIVEN("输入简单的运算符") {
    WHEN("简单加法: 2+4") {
      THEN("结果为6") {
        auto temp = calculator.calculate("2+4");
        REQUIRE(temp.has_value());
        REQUIRE(6 == temp.value());
      }  ///< THEN
    }   ///< WHEN

    WHEN("简单加法: 22+44") {
      THEN("结果为66") {
        auto temp = calculator.calculate("22+44");
        REQUIRE(temp.has_value());
        REQUIRE(66 == temp.value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("简单减法: 4-2") {
      THEN("结果为2") {
        auto temp = calculator.calculate("4-2");
        REQUIRE(temp.has_value());
        REQUIRE(2 == temp.value());
      }  ///< THEN
    }    ///< WHEH

    WHEN("简单减法: 44-23") {
      THEN("结果为21") {
        auto temp = calculator.calculate("44-23");
        REQUIRE(temp.has_value());
        REQUIRE(21 == temp.value());
      }  ///< THEN
    }    ///< WHEH
  }      ///< GIVEN

  GIVEN("连续运算") {
    WHEN("连续加法: 4+5+6") {
      THEN("结果为: 15") {
        auto temp = calculator.calculate("4+5+6");
        REQUIRE(temp.has_value());
        REQUIRE(15 == temp.value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("连续减法: 15-7-2") {
      THEN("结果为: 5") {
        auto temp = calculator.calculate("15-7-2");
        REQUIRE(temp.has_value());
        REQUIRE(5 == temp.value());
      } ///< THEN
    }  ///< WHEN
  }    ///< GIVEN

  GIVEN("输入单个字符") {
    WHEN("输入正确的数字: 234") {
      THEN("结果为:234") { REQUIRE(234 == calculator.calculate("234")); }
    }  ///< WHEN

    WHEN("输入四则运算符号: +") {
      THEN("返回空值") {
        REQUIRE(!calculator.calculate("+").has_value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("输入非法符号: @") {
      THEN("返回空值") { REQUIRE(!calculator.calculate("@").has_value()); }
    }  ///< WHEN
  }    ///<  GIVEN

}  ///< SCENARIO