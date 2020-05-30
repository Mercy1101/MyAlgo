#include "data_struct/stack_practice.h"

#include <catch2/catch.hpp>

SCENARIO("四则运算的简单测试", "[data_struct][stack_practice]") {
  GIVEN("输入简单的运算符") {
    WHEN("简单加法: 2+4") {
      THEN("结果为6") {
        auto temp = Lee::simple_calculate::cal("2+4");
        REQUIRE(temp.has_value());
        REQUIRE(6 == temp.value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("简单加法: 22+44") {
      THEN("结果为66") {
        auto temp = Lee::simple_calculate::cal("22+44");
        REQUIRE(temp.has_value());
        REQUIRE(66 == temp.value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("简单减法: 4-2") {
      THEN("结果为2") {
        auto temp = Lee::simple_calculate::cal("4-2");
        REQUIRE(temp.has_value());
        REQUIRE(2 == temp.value());
      }  ///< THEN
    }    ///< WHEH

    WHEN("简单减法: 44-23") {
      THEN("结果为21") {
        auto temp = Lee::simple_calculate::cal("44-23");
        REQUIRE(temp.has_value());
        REQUIRE(21 == temp.value());
      }  ///< THEN
    }    ///< WHEH
  }      ///< GIVEN

  GIVEN("连续运算") {
    WHEN("连续加法: 4+5+6") {
      THEN("结果为: 15") {
        auto temp = Lee::simple_calculate::cal("4+5+6");
        REQUIRE(temp.has_value());
        REQUIRE(15 == temp.value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("连续加法: 4+5+6+9+10") {
      THEN("结果为: 34") {
        auto temp = Lee::simple_calculate::cal("4+5+6+9+10");
        REQUIRE(temp.has_value());
        REQUIRE(34 == temp.value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("连续加法: 4+5+6+99+9999+66767+87987") {
      THEN("结果为: 164867") {
        auto temp = Lee::simple_calculate::cal("4+5+6+99+9999+66767+87987");
        REQUIRE(temp.has_value());
        REQUIRE(164867 == temp.value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("连续减法: 15-7-2") {
      THEN("结果为: 6") {
        auto temp = Lee::simple_calculate::cal("15-7-2");
        REQUIRE(temp.has_value());
        REQUIRE(6 == temp.value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("连续减法: 15-7-2-6-7-8") {
      THEN("结果为: -15") {
        auto temp = Lee::simple_calculate::cal("15-7-2-6-7-8");
        REQUIRE(temp.has_value());
        REQUIRE(-15 == temp.value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("连续减法: 15666669-2397-345534-89834-2893-28938") {
      THEN("结果为: 15197073") {
        auto temp =
            Lee::simple_calculate::cal("15666669-2397-345534-89834-2893-28938");
        REQUIRE(temp.has_value());
        REQUIRE(15197073 == temp.value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("连续加减法: 66+10-12") {
      THEN("结果为: 64") {
        auto temp = Lee::simple_calculate::cal("66+10-12");
        REQUIRE(temp.has_value());
        REQUIRE(64 == temp.value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("连续加减法: 15666669+2397-345534-89834+2893-28938") {
      THEN("结果为: 15207653") {
        auto temp =
            Lee::simple_calculate::cal("15666669+2397-345534-89834+2893-28938");
        REQUIRE(temp.has_value());
        REQUIRE(15207653 == temp.value());
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN

  GIVEN("简单乘除运算") {
    WHEN("简单乘法： 5*8") {
      THEN("结果为： 40") {
        auto temp = Lee::simple_calculate::cal("5*8");
        REQUIRE(temp.has_value());
        REQUIRE(40 == temp.value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("简单乘法： 50*99") {
      THEN("结果为：4950") {
        auto temp = Lee::simple_calculate::cal("50*99");
        REQUIRE(temp.has_value());
        REQUIRE(4950 == temp.value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("简单除法： 50/2") {
      THEN("结果为：25") {
        auto temp = Lee::simple_calculate::cal("50/2");
        REQUIRE(temp.has_value());
        REQUIRE(25 == temp.value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("简单除法： 5/2") {
      THEN("结果为：2") {
        auto temp = Lee::simple_calculate::cal("5/2");
        REQUIRE(temp.has_value());
        REQUIRE(2 == temp.value());
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN

  GIVEN("简单四则运算") {
    WHEN("简单四则运算: 3*4/2+2-4") {
      THEN("结果为： 4") {
        auto temp = Lee::simple_calculate::cal("3*4/2+2-4");
        REQUIRE(temp.has_value());
        REQUIRE(4 == temp.value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("简单四则运算: 30*40/20+20-40") {
      THEN("结果为： 40") {
        auto temp = Lee::simple_calculate::cal("30*40/20+20-40");
        REQUIRE(temp.has_value());
        REQUIRE(40 == temp.value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("简单四则运算: 12323+23*2-81/9") {
      THEN("结果为： 12360") {
        auto temp = Lee::simple_calculate::cal("12323+23*2-81/9");
        REQUIRE(temp.has_value());
        REQUIRE(12360 == temp.value());
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN

  GIVEN("输入单个字符") {
    WHEN("输入正确的数字: 234") {
      THEN("结果为:234") { REQUIRE(234 == Lee::simple_calculate::cal("234")); }
    }  ///< WHEN

    WHEN("输入四则运算符号: +") {
      THEN("返回空值") {
        REQUIRE(!Lee::simple_calculate::cal("+").has_value());
      }  ///< THEN
    }    ///< WHEN

    WHEN("输入非法符号: @") {
      THEN("返回空值") {
        REQUIRE(!Lee::simple_calculate::cal("@").has_value());
      }
    }  ///< WHEN
  }    ///<  GIVEN

  GIVEN("除零") {
    WHEN("简单除零：4554/0") {
      THEN("返回空值") {
        REQUIRE(!Lee::simple_calculate::cal("4554/0").has_value());
      }  ///< THEN
    }    ///< WHEN
    WHEN("复杂除零：444+999-888-4554/0+9090") {
      THEN("返回空值") {
        REQUIRE(
            !Lee::simple_calculate::cal("444+999-888-4554/0+9090").has_value());
      }  ///< THEN
    }    ///< WHEN
    WHEN("复杂除零：2323*2/9+2323+4554/0*99-99+8") {
      THEN("返回空值") {
        REQUIRE(!Lee::simple_calculate::cal("2323*2/9+2323+4554/0*99-99+8")
                     .has_value());
      }  ///< THEN
    }    ///< WHEN
  }      ///< GIVEN

}  ///< SCENARIO