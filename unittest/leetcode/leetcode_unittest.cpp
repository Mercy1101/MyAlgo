#include "leetcode.h"
#include <catch2/catch.hpp>

/**
 * @brief Lee::Leetcode::isPalindrome()函数的简单测试
 */
TEST_CASE("isPalindrome简单测试", "[leetcode][isPalindrome]") {
  using namespace Lee;
  REQUIRE(Leetcode::isPalindrome("a"));
  REQUIRE(Leetcode::isPalindrome("aba"));
  REQUIRE(Leetcode::isPalindrome("ABa"));
  REQUIRE(Leetcode::isPalindrome("A man, a plan, a canal: Panama"));

  REQUIRE_FALSE(Leetcode::isPalindrome(","));
  REQUIRE_FALSE(Leetcode::isPalindrome(""));
  REQUIRE_FALSE(Leetcode::isPalindrome("race a car"));
}

/**
 * @brief Lee::Leetcode::IsValidParentheses()函数的简单测试
 */
TEST_CASE("IsValidParentheses简单测试", "[leetcode][IsValidParentheses]") {
  using namespace Lee;
  REQUIRE(Leetcode::IsValidParentheses("((){{}([[]{[]}]())})"));
  REQUIRE(Leetcode::IsValidParentheses("()"));
  REQUIRE(Leetcode::IsValidParentheses("()[]"));

  REQUIRE_FALSE(Leetcode::IsValidParentheses(""));
  REQUIRE_FALSE(Leetcode::IsValidParentheses("(]"));
  REQUIRE_FALSE(Leetcode::IsValidParentheses("([)]"));
  REQUIRE_FALSE(Leetcode::IsValidParentheses("("));
  REQUIRE_FALSE(Leetcode::IsValidParentheses(")"));
}
