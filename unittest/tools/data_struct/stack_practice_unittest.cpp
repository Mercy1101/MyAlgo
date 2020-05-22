#include "data_struct/stack_practice.h"

#include <catch2/catch.hpp>

SCENARIO("四则运算的简单测试", "[data_struct][stack_practice]") {
  Lee::simple_calculate calculator;
  GIVEN("输入简单的运算符") {
    WHEN("简单加法: 2+2") {
      THEN("结果为4") { REQUIRE(calculator.calculate("2+2");); }  ///< THEN
    }                                                             ///< WHEN

    WHEN("简单减法: 4-2") {
      THEN("结果为2") { REQUIRE(calculator.calculate("4-2");); }
    }
  }  ///< GIVEN

  GIVEN("输入单个字符") {
    WHEN("输入正确的数字: 234") {
      THEN("结果为:234") { REQUIRE(calculator.calculate("234");); }
    }  ///< WHEN

    WHEN("输入四则运算符号: +") {
      THEN("抛出异常") {
        try {
          calculator.calculate("+");
          REQUIRE(false);
        } catch (std::exception e) {
          REQUIRE(true);
        }
      }  ///< THEN
    }    ///< WHEN

    WHEN("输入非法符号: @") {
      THEN("抛出异常") {
        try {
          calculator.calculate("@");
          REQUIRE(false);
        } catch (std::exception e) {
          REQUIRE(true);
        }
      }
    }  ///< WHEN
  }    ///<  GIVEN

}  ///< SCENARIO