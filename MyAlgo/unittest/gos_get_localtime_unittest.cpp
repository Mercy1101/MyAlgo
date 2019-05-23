#include "gtest/gtest.h"
#include "gos_get_localtime.h"

namespace {
    // The fixture for testing class Foo.
    class Gos_get_localtime : public ::testing::Test
    {
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        Gos_get_localtime()
        {
            // You can do set-up work for each test here.
        }

        ~Gos_get_localtime() override
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

    TEST_F(Gos_get_localtime, Normal_Time)
    {
        time_t              ulCurrTime = 1558424496;    // 2019/05/21 15:41:36
        GOS_DATETIME_T      stLocalTime = { 0 };

        // ASSERT_TRUE(gos_get_localtime(reinterpret_cast<UINT32 *>(&ulCurrTime), &stLocalTime));
        EXPECT_EQ(gos_get_localtime(reinterpret_cast<UINT32 *>(&ulCurrTime), &stLocalTime), TRUE);
        EXPECT_EQ(stLocalTime.usYear, 2019);
        EXPECT_EQ(stLocalTime.ucMonth, 5);
        EXPECT_EQ(stLocalTime.ucDay, 21); 

        EXPECT_EQ(stLocalTime.ucHour, 15);
        EXPECT_EQ(stLocalTime.ucMinute, 41);
        EXPECT_EQ(stLocalTime.ucSecond, 36);
        EXPECT_EQ(stLocalTime.ucSn, 0);

        EXPECT_EQ(ulCurrTime, 1558424496);

        // imporve版同样测试一遍
        GOS_DATETIME_IMPORVE_T      stLocalTimeImporve = { 0 };

        // 所有数值应跟没有改进版一致
        EXPECT_EQ(gos_get_localtime_imporve(&ulCurrTime, &stLocalTimeImporve), TRUE);
        EXPECT_EQ(stLocalTime.usYear, stLocalTimeImporve.ulYear);
        EXPECT_EQ(stLocalTime.ucMonth, stLocalTimeImporve.ulMonth);
        EXPECT_EQ(stLocalTime.ucDay, stLocalTimeImporve.ulDay);

        EXPECT_EQ(stLocalTime.ucHour, stLocalTimeImporve.ulHour);
        EXPECT_EQ(stLocalTime.ucMinute, stLocalTimeImporve.ulMinute);
        EXPECT_EQ(stLocalTime.ucSecond, stLocalTimeImporve.ulSecond);
        // EXPECT_EQ(stLocalTime.ucSn, 0);
        // 再次检查该数值没有变化
        EXPECT_EQ(ulCurrTime, 1558424496);
    }

    TEST_F(Gos_get_localtime, No_Input_Time)
    {
        time_t              *pulCurrTime = nullptr;
        GOS_DATETIME_T      stLocalTime = { 0 };

        time_t now = time(0);
        tm ltm = {0};
        localtime_s(&ltm, &now);      // Current Time (UINTX time stamp)

        // ASSERT_TRUE(gos_get_localtime(pulCurrTime, &stLocalTime));
        EXPECT_EQ(gos_get_localtime(reinterpret_cast<UINT32 *>(pulCurrTime), &stLocalTime), TRUE);
        EXPECT_EQ(stLocalTime.usYear, 1900 + ltm.tm_year);
        EXPECT_EQ(stLocalTime.ucMonth, 1 + ltm.tm_mon);
        EXPECT_EQ(stLocalTime.ucDay, ltm.tm_mday);

        EXPECT_EQ(stLocalTime.ucHour, ltm.tm_hour);
        EXPECT_EQ(stLocalTime.ucMinute, ltm.tm_min);
        EXPECT_EQ(stLocalTime.ucSecond, ltm.tm_sec);
        EXPECT_EQ(stLocalTime.ucSn, 0);

        EXPECT_EQ(pulCurrTime, nullptr);

        // imporve版同样测试一遍
        GOS_DATETIME_IMPORVE_T      stLocalTimeImporve = { 0 };

        // 所有数值应跟没有改进版一致
        EXPECT_EQ(gos_get_localtime_imporve(pulCurrTime, &stLocalTimeImporve), TRUE);
        EXPECT_EQ(stLocalTime.usYear, stLocalTimeImporve.ulYear);
        EXPECT_EQ(stLocalTime.ucMonth, stLocalTimeImporve.ulMonth);
        EXPECT_EQ(stLocalTime.ucDay, stLocalTimeImporve.ulDay);

        EXPECT_EQ(stLocalTime.ucHour, stLocalTimeImporve.ulHour);
        EXPECT_EQ(stLocalTime.ucMinute, stLocalTimeImporve.ulMinute);
        EXPECT_EQ(stLocalTime.ucSecond, stLocalTimeImporve.ulSecond);
        // EXPECT_EQ(stLocalTime.ucSn, 0);
        // 再次检查该数值没有变化
        EXPECT_EQ(pulCurrTime, nullptr);
    }

    TEST_F(Gos_get_localtime, Input_Time_Zero)
    {
        time_t              ulCurrTime = 0;    // 1970/01/01 08:00:00   东八时区，所以是八点
        GOS_DATETIME_T      stLocalTime = { 0 };

        // ASSERT_TRUE(gos_get_localtime(reinterpret_cast<UINT32 *>(&ulCurrTime), &stLocalTime));
        EXPECT_EQ(gos_get_localtime(reinterpret_cast<UINT32 *>(&ulCurrTime), &stLocalTime), TRUE);
        EXPECT_EQ(stLocalTime.usYear, 1970);
        EXPECT_EQ(stLocalTime.ucMonth, 1);
        EXPECT_EQ(stLocalTime.ucDay, 1); 

        EXPECT_EQ(stLocalTime.ucHour, 8);
        EXPECT_EQ(stLocalTime.ucMinute, 0);
        EXPECT_EQ(stLocalTime.ucSecond, 0);
        EXPECT_EQ(stLocalTime.ucSn, 0);

        EXPECT_EQ(ulCurrTime, 0);

        // imporve版同样测试一遍
        GOS_DATETIME_IMPORVE_T      stLocalTimeImporve = { 0 };

        // 所有数值应跟没有改进版一致
        EXPECT_EQ(gos_get_localtime_imporve(&ulCurrTime, &stLocalTimeImporve), TRUE);
        EXPECT_EQ(stLocalTime.usYear, stLocalTimeImporve.ulYear);
        EXPECT_EQ(stLocalTime.ucMonth, stLocalTimeImporve.ulMonth);
        EXPECT_EQ(stLocalTime.ucDay, stLocalTimeImporve.ulDay);

        EXPECT_EQ(stLocalTime.ucHour, stLocalTimeImporve.ulHour);
        EXPECT_EQ(stLocalTime.ucMinute, stLocalTimeImporve.ulMinute);
        EXPECT_EQ(stLocalTime.ucSecond, stLocalTimeImporve.ulSecond);
        // EXPECT_EQ(stLocalTime.ucSn, 0);
        // 再次检查该数值没有变化
        EXPECT_EQ(ulCurrTime, 0);
    }

    void gos_get_localtime_test(int iTestTime)
    {
        unsigned              ulCurrTime = 0;
        GOS_DATETIME_T      stLocalTime = { 0 };
        for (INT32 i = 0; i < iTestTime; ++i)
        {
            ulCurrTime = i;
            gos_get_localtime(&ulCurrTime, &stLocalTime);
        }
    }

    void gos_get_localtime_imporve_test(int iTestTime)
    {
        time_t                      ulCurrTime = 0;
        GOS_DATETIME_IMPORVE_T      stLocalTime = { 0 };
        for (INT32 i = 0; i < iTestTime; ++i)
        {
            ulCurrTime = i;
            gos_get_localtime_imporve(&ulCurrTime, &stLocalTime);
        }
    }

    TEST_F(Gos_get_localtime, Profiler_Gos_get_localtime)
    {
        PROFILER_F(gos_get_localtime_test, LEE_DEBUG::NUMBER_E::Hundred_Thousands_);
        PROFILER_F(gos_get_localtime_imporve_test, LEE_DEBUG::NUMBER_E::Hundred_Thousands_);
    }

}  // namespace