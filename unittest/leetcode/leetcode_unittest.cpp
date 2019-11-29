#include <gtest/gtest.h>
#include "leetcode.h"

/** 
 * @brief Lee::Leetcode::isPalindrome()函数的简单测试
 */
TEST(isPalindrome, Simple)
{
    using namespace Lee;
    EXPECT_TRUE(Leetcode::isPalindrome("a"));
    EXPECT_TRUE(Leetcode::isPalindrome("aba"));
    EXPECT_TRUE(Leetcode::isPalindrome("ABa"));
    EXPECT_TRUE(Leetcode::isPalindrome("A man, a plan, a canal: Panama"));
    
    EXPECT_FALSE(Leetcode::isPalindrome(","));
    EXPECT_FALSE(Leetcode::isPalindrome(""));
    EXPECT_FALSE(Leetcode::isPalindrome("race a car"));
}

/** 
 * @brief Lee::Leetcode::IsValidParentheses()函数的简单测试
 */
TEST(IsValidParentheses, Simple)
{
    using namespace Lee;
    EXPECT_TRUE(Leetcode::IsValidParentheses("((){{}([[]{[]}]())})"));
    EXPECT_TRUE(Leetcode::IsValidParentheses("()"));
    EXPECT_TRUE(Leetcode::IsValidParentheses("()[]"));

    EXPECT_FALSE(Leetcode::IsValidParentheses(""));
    EXPECT_FALSE(Leetcode::IsValidParentheses("(]"));
    EXPECT_FALSE(Leetcode::IsValidParentheses("([)]"));
    EXPECT_FALSE(Leetcode::IsValidParentheses("("));
    EXPECT_FALSE(Leetcode::IsValidParentheses(")"));
}