#include "gtest/gtest.h"
#include "gos_get_file_size.h"

/* 72¸ö×Ö½Ú */
const char * g_TestFilePath = "E:\\1WorkStation\\MyAlgo\\build\\MyAlgo\\Debug\\FileSizeTest.txt";
const char * g_TestFileZeroPath = "E:\\1WorkStation\\MyAlgo\\build\\MyAlgo\\Debug\\FileSizeTest_Zero.txt";

namespace {
    // The fixture for testing class Foo.
    class Gos_get_file_size : public ::testing::Test
    {
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        Gos_get_file_size()
        {
            // You can do set-up work for each test here.
        }

        ~Gos_get_file_size() override
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

    TEST_F(Gos_get_file_size, Normal_TXTFile)
    {
       EXPECT_EQ(gos_get_file_size(g_TestFilePath), 72);
    }

    TEST_F(Gos_get_file_size, No_Input_File)
    {
        EXPECT_EQ(gos_get_file_size(""), -1);
    }

    TEST_F(Gos_get_file_size, Input_Zero_File)
    {
        EXPECT_EQ(gos_get_file_size(g_TestFileZeroPath), 0);
    }

    TEST_F(Gos_get_file_size, Linux_File)
    {
#ifdef _OSWIN32_
#undef _OSWIN32_
        EXPECT_EQ(gos_get_file_size(g_TestFilePath), 72);
        EXPECT_EQ(gos_get_file_size(""), -1);
        EXPECT_EQ(gos_get_file_size(g_TestFileZeroPath), 0);
#define _OSWIN32_
#endif
    }

}  // namespace