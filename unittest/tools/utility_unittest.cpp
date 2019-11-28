#include <string>
#include <limits>
#include "utility/utility.h"
#include "gtest/gtest.h"

/** 
 * @brief 测试KMP这个函数
 */
TEST(KMP, Simple)
{
    using namespace Lee;
    std::string test1("10110100100011111010101");
    size_t pos = 0;

    ASSERT_TRUE(KMP::Search(test1, "1011", pos));
    EXPECT_EQ(pos, 0);

    ASSERT_TRUE(KMP::Search(test1, "1010", pos));
    EXPECT_EQ(pos, 3);

    ASSERT_TRUE(KMP::Search(test1, "10110100100011111010101", pos));
    EXPECT_EQ(pos, 0);

    ASSERT_TRUE(KMP::Search(test1, "0011", pos));
    EXPECT_EQ(pos, 10);

    ASSERT_TRUE(KMP::Search(test1, "10101", pos));
    EXPECT_EQ(pos, 16);
}

/** 
* @brief 测试Lee::IsFileExist判断文件夹存不存在的情况
*/
TEST (IsFileExist, FolderExist)
{
    std::string Path;
    /** 存在的情况 */
    Path = "C:\\Program Files";
    ASSERT_TRUE(Lee::IsFileExist(Path));
    Path = "C:\\Program Files (x86)";
    ASSERT_TRUE(Lee::IsFileExist(Path));
    Path = "C:\\Windows";
    ASSERT_TRUE(Lee::IsFileExist(Path));

    /** 不存在的情况 */
    Path = "C:\\Wws";
    ASSERT_FALSE(Lee::IsFileExist(Path));
    Path = "C:\\sProgram Files (x86)s";
    ASSERT_FALSE(Lee::IsFileExist(Path));
    Path = "C:\\sProram Fils";
    ASSERT_FALSE(Lee::IsFileExist(Path));
}

/** 
* @brief 测试Lee::IsMultiOverFlow()这个函数在int类型下的测试
*/
TEST (IsMultiOverFlow, Simple_Int)
{
    int x = 0;
    int y = 0;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));
    x = 0;
    y = INT_MAX;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));
    x = INT_MAX;
    y = 0;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));
    x = 1;
    y = INT_MAX;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));
    x = INT_MAX;
    y = 1;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));

    x = INT_MIN;
    y = 0;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));
    x = INT_MIN;
    y = 1;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));

    x = INT_MIN;
    y = -1;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));
    x = INT_MIN;
    y = -2;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));

    x = INT_MAX / 5;
    y = 5;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));
    x = 1 + (INT_MAX / 5);
    y = 5;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));

    x = INT_MIN / 5;
    y = 5;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));
    x = -1 + (INT_MIN / 5);
    y = 5;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));
    x = INT_MIN / 5;
    y = -5;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));
    x = -1 + (INT_MIN / 5);
    y = -5;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));

    x = INT_MAX / 2;
    y = 2;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));
    x = 1 + (INT_MAX / 2);
    y = 2;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));

    x = INT_MIN / 2;
    y = 2;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));
    x = -1 + (INT_MIN / 2);
    y = 2;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));
    x = INT_MIN / 2;
    y = -2;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));
    x = -1 + (INT_MIN / 2);
    y = -2;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));
}

/**
* @brief 测试Lee::IsMultiOverFlow()这个函数在long long类型下成功的测试
*/
TEST (IsMultiOverFlow, Simple_LLongTrue)
{
    long long x = 0;
    long long y = 0;

    /** LLONG_MIN * -1 溢出了 */
    x = LLONG_MIN;
    y = -1;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));

    /** LLONG_MIN * -2 溢出了 */
    x = LLONG_MIN;
    y = -2;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));

    /** （LLONG_MAX / 5 + 1） * 5 溢出了 */
    x = 1 + (LLONG_MAX / 5);
    y = 5;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));

    /** (LLONG_MIN / 5 - 1) * 5 溢出了 */
    x = -1 + (LLONG_MIN / 5);
    y = 5;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));

    /** (LLONG_MIN / 5 - 2) * 5 溢出了 */
    x = -2 + (LLONG_MIN / 5);
    y = 5;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));

    /**(LLONG_MIN / 5 - 1) * -5 溢出了 */
    x = -1 + (LLONG_MIN / 5);
    y = -5;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));

    /** (LLONG_MAX / 2 + 1) * 2 溢出了 */
    x = 1 + (LLONG_MAX / 2);
    y = 2;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));

    /**  (LLONG_MIN / 2 - 1) * 2 溢出了 */
    x = -1 + (LLONG_MIN / 2);
    y = 2;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));

    /**  LLONG_MIN / 2 * -2 溢出了 */
    x = LLONG_MIN / 2;
    y = -2;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));

    /**  (LLONG_MIN / 2 - 1) * -2 溢出了 */
    x = -1 + (LLONG_MIN / 2);
    y = -2;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));
}

/**
* @brief 测试Lee::IsMultiOverFlow()这个函数在long long类型下没有溢出的测试
*/
TEST (IsMultiOverFlow, Simple_LLongFalse)
{
    long long x = 0;
    long long y = 0;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));

    /** LLONG_MAX * 0 没有溢出*/
    x = 0;
    y = LLONG_MAX;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));
    /** LLONG_MAX * 0 没有溢出*/

    x = LLONG_MAX;
    y = 0;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));

    /** LLONG_MAX * 1 没有溢出*/
    x = 1;
    y = LLONG_MAX;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));

    /** LLONG_MAX * 1 没有溢出*/
    x = LLONG_MAX;
    y = 1;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));

    /** LLONG_MIN * 0 没有溢出*/
    x = LLONG_MIN;
    y = 0;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));

    /** LLONG_MIN * 1 没有溢出*/
    x = LLONG_MIN;
    y = 1;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));

    /** LLONG_MAX / 5 * 5 没有溢出*/
    x = LLONG_MAX / 5;
    y = 5;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));

    /** LLONG_MIN / 5 * 5 没有溢出*/
    x = LLONG_MIN / 5;
    y = 5;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));

    /** LLONG_MIN / 5 * 5 没有溢出*/
    /** LLONG_MIN / 5由于余数被略去了所以LLONG_MIN / 5 * 5 没有溢出 */
    x = LLONG_MIN / 5;
    y = -5;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));

    /** LLONG_MIN / 2 * 2 没有溢出*/
    x = LLONG_MIN / 2;
    y = 2;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));

    /** LLONG_MAX / 2 * 2 没有溢出*/
    x = LLONG_MAX / 2;
    y = 2;
    EXPECT_FALSE(Lee::IsMultiOverFlow(x, y));
}

/**
* @brief 测试Lee::IsMultiOverFlow()这个函数在long long类型下没有溢出的测试
*/
TEST (IsMultiOverFlow, BigNumber_LLongTrue)
{
    /** LLONG_MAX * LLONG_MAX 溢出了 */
    long long x = LLONG_MAX;
    long long y = LLONG_MAX;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));

    /** LLONG_MAX * LLONG_MIN 溢出了 */
    x = LLONG_MAX;
    y = LLONG_MIN;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));
    EXPECT_TRUE(Lee::IsMultiOverFlow(y, x));

    /** LLONG_MIN * LLONG_MIN 溢出了 */
    x = LLONG_MIN;
    y = LLONG_MIN;
    EXPECT_TRUE(Lee::IsMultiOverFlow(x, y));
}

/**
* @brief Lee::power()这个函数简单测试
*/
TEST (power, SimpleTest)
{
    
    EXPECT_EQ(Lee::power(0, 2), 0);

    EXPECT_EQ(Lee::power(2,  0), 1);
    EXPECT_EQ(Lee::power(2,  5), 32);
    EXPECT_EQ(Lee::power(2,  7), 128);
    EXPECT_EQ(Lee::power(2, 10), 1024);
    EXPECT_EQ(Lee::power(2, 16), 65536);

    EXPECT_EQ(Lee::power(7,  0), 1);
    EXPECT_EQ(Lee::power(7,  5), 16807);
    EXPECT_EQ(Lee::power(7,  7), 823543);

    /** 零不能乘幂0 */
    //EXPECT_EQ(Lee::power(0, 0), 1);
    /** int 溢出为0 */
    //EXPECT_EQ(Lee::power(2, 32), 0);
    /** 不支持负数次幂 */
    //EXPECT_EQ(Lee::power(2, -1), 1);
}