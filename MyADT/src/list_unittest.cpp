#include "gtest/gtest.h"
#include "list.h"

namespace {
    // The fixture for testing class Foo.
    class ListTest : public ::testing::Test
    {
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        ListTest()
        {
            // You can do set-up work for each test here.
        }

        ~ListTest() override
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
    TEST_F(ListTest, FirstTest_Push)
    {
        {
            list FirstTest;
            EXPECT_EQ(FirstTest.at(0), INVALID_VALUE);
            FirstTest.push_back(0);
            EXPECT_EQ(FirstTest.at(0), 0);
            FirstTest.push_back(1);
            EXPECT_EQ(FirstTest.at(1), 1);
            FirstTest.push_back(2);
            EXPECT_EQ(FirstTest.at(2), 2);
            FirstTest.push_back(3);
            EXPECT_EQ(FirstTest.at(3), 3);
            FirstTest.push_back(4);
            EXPECT_EQ(FirstTest.at(4), 4);

            for (int i = 0; i < 5; ++i)
            {
                EXPECT_EQ(FirstTest.at(i), i);
            }
        }
    }

    TEST_F(ListTest, Out_Of_Bound)
    {
        list OutOfBound;
        EXPECT_EQ(OutOfBound.at(0), INVALID_VALUE);
        for (int i = 0; i < 100; ++i)
        {
            OutOfBound.push_back(i);
            EXPECT_EQ(OutOfBound.at(i + 1), INVALID_VALUE);
        }
    }

    TEST_F(ListTest, Insert_Test)
    {
        list InsertList;
        /* 链表为空的情况 */
        ASSERT_FALSE(InsertList.Insert(555, 0));
        ASSERT_FALSE(InsertList.Insert(666, 1));

        /* 正常情况 */
        InsertList.push_back(0);
        ASSERT_TRUE(InsertList.Insert(1, 0));
        EXPECT_EQ(InsertList.at(0), 0);
        EXPECT_EQ(InsertList.at(1), 1);

        /* index为负数的情况 */
        for (int i = -1; i > -100; --i)
        {
            ASSERT_FALSE(InsertList.Insert(i, i));
        }

        for (int i = 2; i < 100; ++i)
        {
            InsertList.push_back(i);
        }

        /* 正常情况 */
        ASSERT_TRUE(InsertList.Insert(2222, 2));
        EXPECT_EQ(InsertList.at(2 + 1), 2222);
        ASSERT_TRUE(InsertList.Insert(55555, 50));
        EXPECT_EQ(InsertList.at(50 + 1), 55555);

        /* index超过list的最大值情况 */
        list OutOfIndex;
        OutOfIndex.push_back(0);
        for (int i = 0; i < 5; i++)
        {
            OutOfIndex.Insert(i + 1, 1000000);
            EXPECT_EQ(OutOfIndex.at(i), i);
        }
    }
}  // namespace