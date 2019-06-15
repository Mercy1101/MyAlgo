#include <limits>
#include "gtest/gtest.h"
#include "gos_func.h"

namespace {
    // The fixture for testing class Foo.
    class Gos_itoa : public ::testing::Test
    {
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        Gos_itoa()
        {
            // You can do set-up work for each test here.
        }

        ~Gos_itoa() override
        {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        void SetUp() override
        {
            // Code here will be called immediately after the constructor (right
            // before each test).
        }

        void TearDown() override
        {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }

        // Objects declared here can be used by all tests in the test case for Foo.
    };

    // Tests that the Foo::Bar() method does Abc.
    TEST_F(Gos_itoa, NormalTest)
    {
        CHAR    acDigit[10] = {0};
        UINT32  ulDigit = 0;

        ulDigit = 1;
        EXPECT_EQ(gos_itoa(ulDigit, acDigit), 1);
        ASSERT_STREQ(acDigit, "1");

        ulDigit = 0;
        EXPECT_EQ(gos_itoa(ulDigit, acDigit), 1);
        ASSERT_STREQ(acDigit, "0");

        ulDigit = 100;
        EXPECT_EQ(gos_itoa(ulDigit, acDigit), 3);
        ASSERT_STREQ(acDigit, "100");

        ulDigit = 100005;
        EXPECT_EQ(gos_itoa(ulDigit, acDigit), 6);
        ASSERT_STREQ(acDigit, "100005");
    }

    TEST_F(Gos_itoa, Out_Of_Bound)
    {
        CHAR    acDigit[11] = {0};
        UINT32  ulDigit = 0;

        // ��ֵ����������
        ulDigit = -1;
        EXPECT_EQ(gos_itoa(ulDigit, acDigit), 10);
        ASSERT_STREQ(acDigit, "4294967295");

        // int������ȡֵ��Χ������������
        {
            ulDigit = INT_MAX;
            EXPECT_EQ(gos_itoa(ulDigit, acDigit), 10);
            ASSERT_STREQ(acDigit, "2147483647");

            ulDigit = INT_MAX+1;
            EXPECT_EQ(gos_itoa(ulDigit, acDigit), 10);
            ASSERT_STREQ(acDigit, "2147483648");

            ulDigit = INT_MIN;
            EXPECT_EQ(gos_itoa(ulDigit, acDigit), 10);
            ASSERT_STREQ(acDigit, "2147483648");

            ulDigit = INT_MIN - 1;
            EXPECT_EQ(gos_itoa(ulDigit, acDigit), 10);
            ASSERT_STREQ(acDigit, "2147483647");
        }

        // ���������������Ϊ0~UINT_MAX
        {
            ulDigit = UINT_MAX;
            EXPECT_EQ(gos_itoa(ulDigit, acDigit), 10);
            ASSERT_STREQ(acDigit, "4294967295");

            ulDigit = UINT_MAX+1;// ulDigit?0
            EXPECT_EQ(gos_itoa(ulDigit, acDigit), 1);
            ASSERT_STRNE(acDigit, "4294967296");
            ASSERT_STREQ(acDigit, "0");
        }

    }

    TEST_F(Gos_itoa, Imporve_Normal)
    {
        char acBuf[1024] = {0};
        int  value = -123456;
        ASSERT_STREQ(gos_itoa_imporve(value, acBuf), "-123456");
        ASSERT_STREQ(acBuf, "-123456");

        memset(acBuf, sizeof(acBuf), 0);
        value = 0;
        ASSERT_STREQ(gos_itoa_imporve(value, acBuf), "0");
        ASSERT_STREQ(acBuf, "0");

        memset(acBuf, sizeof(acBuf), 0);
        value = 12345;
        ASSERT_STREQ(gos_itoa_imporve(value, acBuf), "12345");
        ASSERT_STREQ(acBuf, "12345");
    }

    /* �������������ֵ����Сֵ */
    TEST_F(Gos_itoa, Imporve_MaxAndMin)
    {
        char acBuf[1024] = {0};
        int  value = INT_MAX;

        ASSERT_STREQ(gos_itoa_imporve(value, acBuf), "2147483647");
        ASSERT_STREQ(acBuf, "2147483647");

        memset(acBuf, sizeof(acBuf), 0);
        value = INT_MIN;
        ASSERT_STREQ(gos_itoa_imporve(value, acBuf), "-2147483648");
        ASSERT_STREQ(acBuf, "-2147483648");
    }

    /* int�����ʱ������������ */
    TEST_F(Gos_itoa, Imporve_OutOfBound)
    {
        char acBuf[1024] = {0};
        int  value = INT_MAX;

        value = INT_MAX+1;
        ASSERT_STREQ(gos_itoa_imporve(value, acBuf), "-2147483648");
        ASSERT_STREQ(acBuf, "-2147483648");

        memset(acBuf, sizeof(acBuf), 0);
        value = INT_MIN-1;
        ASSERT_STREQ(gos_itoa_imporve(value, acBuf), "2147483647");
        ASSERT_STREQ(acBuf, "2147483647");
    }

}  // namespace