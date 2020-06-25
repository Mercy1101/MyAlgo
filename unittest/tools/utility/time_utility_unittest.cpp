#include <catch2/catch.hpp>
#include <utility/utility.h>

SCENARIO("get_weekday 简单测试", "[leetcode][get_weekday]") {
  GIVEN("简单测试") {
    WHEN("Input: 2020, 6, 25") {
      THEN("Output: 4") {
        REQUIRE((4 == lee::get_weedday(2020, 6, 25)));
      }  ///< THEN
    }    ///< WHEN
    WHEN("Input: 1949, 10, 1") {
      THEN("Output: 6") {
        REQUIRE((6 == lee::get_weedday(1949, 10, 1)));
      }  ///< THEN
    }    ///< WHEN
  }  ///< GIVEN

 /// GIVEN("临界测试"){
 ///   WHEN("Input: 1582, 10, 4"){
 ///     THEN("Output: 1"){
 ///       REQUIRE((1 == lee::get_weedday(1582, 10, 4)));
 ///     }
 ///   }
 ///   WHEN("Input: 1582, 10, 3"){
 ///     THEN("Output: 7"){
 ///       REQUIRE((7 == lee::get_weedday(1582, 10, 3)));
 ///     }
 ///   }
 ///   WHEN("Input: 1582, 10, 2"){
 ///     THEN("Output: 6"){
 ///       REQUIRE((6 == lee::get_weedday(1582, 10, 2)));
 ///     }
 ///   }
 /// }
}  ///< SCENARIO
