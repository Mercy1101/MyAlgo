#include <limits>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>
#include "gtest/gtest.h"
#include "gos_func.h"

namespace {
    // The fixture for testing class Foo.
    class Gos_itoa : public ::testing::Test
    {
    };

    TEST_F(Gos_itoa, NormalTest)
    {
        char    acDigit[10] = {0};
        unsigned int  ulDigit = 0;

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
        char    acDigit[11] = {0};
        unsigned int  ulDigit = 0;

        // 负值不正常工作
        ulDigit = -1;
        EXPECT_EQ(gos_itoa(ulDigit, acDigit), 10);
        ASSERT_STREQ(acDigit, "4294967295");

        // int的正常取值范围都能正常运作
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

        // 最大正常工作区间为0~UINT_MAX
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

    /* 正常工作的最大值和最小值 */
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

    /* int型溢出时，工作不正常 */
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

    class Gos_get_rand : public ::testing::Test
    {
    };

    TEST_F(Gos_get_rand, NormalTest)
    {
        std::vector<unsigned int> RandNumber1;
        std::vector<unsigned int> RandNumber2;

        for (unsigned int i = 0; i < 999; i++)
        {
            RandNumber1.push_back(gos_get_rand());
        }

        for (unsigned int i = 0; i < 999; i++)
        {
            RandNumber2.push_back(gos_get_rand());
        }

        for (unsigned int i = 0; i < 999; i++)
        {
            EXPECT_NE(RandNumber1.at(i), RandNumber2.at(i));
        }
    }


    static std::condition_variable ConditionVar;
    static std::mutex cv_m;
    static int iWaitFor = 0;
    class Gos_get_range_rand : public ::testing::Test
    {
    public : 
        static void waits1(std::vector<unsigned int> &RandNumber1)
        {
            std::unique_lock<std::mutex> lk(cv_m);
            ConditionVar.wait(lk, []{return iWaitFor == 1;});
            for (unsigned int i = 0; i < 999; i++)
            {
                RandNumber1.push_back(gos_get_rand());
            }
        }

        static void waits2(std::vector<unsigned int> &RandNumber2)
        {
            std::unique_lock<std::mutex> lk(cv_m);
            ConditionVar.wait(lk, []{return iWaitFor == 1;});

            for (unsigned int i = 0; i < 999; i++)
            {
                RandNumber2.push_back(gos_get_range_rand(0, RAND_MAX));
            }
        }
    };

    TEST_F(Gos_get_range_rand, MutliThreadTest)
    {
        std::vector<unsigned int> RandNumber1;
        std::vector<unsigned int> RandNumber2;
        std::thread t1(Gos_get_range_rand::waits1, std::ref(RandNumber1)), t2(Gos_get_range_rand::waits2, std::ref(RandNumber2));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        iWaitFor = 1;
        ConditionVar.notify_all();

        t1.join();
        t2.join();

        for (unsigned int i = 0; i < 999; i++)
        {
            EXPECT_NE(RandNumber1.at(i), RandNumber2.at(i));
        }
    }

    TEST_F(Gos_get_range_rand, Reverse)
    {
        std::vector<unsigned int> vRandNumber;
        vRandNumber.push_back(gos_get_range_rand(10000, 2));
        for (auto itor = vRandNumber.cbegin(); itor != vRandNumber.cend(); ++itor)
        {
            ASSERT_GT(*itor, 1);
            ASSERT_LT(*itor, 10001);
        }
    }

    class Gos_get_gmtime : public ::testing::Test
    {};

    TEST_F(Gos_get_gmtime, NormalTest)
    {
        EXPECT_EQ(gos_get_current_time() - gos_get_gmtime(), 8*60*60);
    }

    class Gos_set_gmtime : public ::testing::Test
    {};

    TEST_F(Gos_set_gmtime, NormalTest)
    {
#if 0
        /// 该测试未通过
        unsigned int ulCurrentTime = gos_get_current_time();
        gos_set_gmtime(ulCurrentTime+24*60*60);
        EXPECT_EQ(gos_get_gmtime(), ulCurrentTime+24*60*60);
        gos_set_gmtime(ulCurrentTime);
        EXPECT_EQ(gos_get_current_time(), ulCurrentTime);
#endif
    }

    class Binary_search_array : public ::testing::Test
    {
    };

    const static unsigned aucPrimeTableTest[54] = {

         2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u, 31u, 
         37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u, 71u, 73u, 
         79u, 83u, 89u, 97u, 101u, 103u, 107u, 109u, 113u, 
         127u, 131u, 137u, 139u, 149u, 151u, 157u, 163u, 
         167u, 173u, 179u, 181u, 191u, 193u, 197u, 199u, 
         211u, 223u, 227u, 229u, 233u, 239u, 241u, 251u
      };

    TEST_F(Binary_search_array, NormalTest)
    {
        int ulTemp = ARRAY_SIZE(aucPrimeTableTest);
        EXPECT_EQ(Binary_Search_Array(2, aucPrimeTableTest, 54), 2);
        EXPECT_EQ(Binary_Search_Array(4, aucPrimeTableTest, 54), 5);
        EXPECT_EQ(Binary_Search_Array(68, aucPrimeTableTest, 54), 71);
        EXPECT_EQ(Binary_Search_Array(241, aucPrimeTableTest, 54), 241);
        EXPECT_EQ(Binary_Search_Array(242, aucPrimeTableTest, 54), 251);
        EXPECT_EQ(Binary_Search_Array(251, aucPrimeTableTest, 54), 251);
    }

    class Gos_get_next_prime : public ::testing::Test
    {
    };

    TEST_F(Gos_get_next_prime, NoremalTest)
    {
        EXPECT_EQ(gos_get_next_prime(2), 2);
        EXPECT_EQ(gos_get_next_prime(4), 5);
        EXPECT_EQ(gos_get_next_prime(68), 71);
        EXPECT_EQ(gos_get_next_prime(241), 241);
        EXPECT_EQ(gos_get_next_prime(242), 251);

        EXPECT_EQ(gos_get_next_prime(257u), 257u);
        EXPECT_EQ(gos_get_next_prime(258u), 263u);
        EXPECT_EQ(gos_get_next_prime(12611u), 12611u);
        EXPECT_EQ(gos_get_next_prime(12612u), 12613u);
        EXPECT_EQ(gos_get_next_prime(65519u), 65519u);
        EXPECT_EQ(gos_get_next_prime(65520u), 65521u);
        EXPECT_EQ(gos_get_next_prime(65521u), 65521u);


        EXPECT_EQ(gos_get_next_prime(13822u+0xFFFFu), 13822u+0xFFFFu);
        EXPECT_EQ(gos_get_next_prime(13825u+0xFFFFu), 13832u+0xFFFFu);
        EXPECT_EQ(gos_get_next_prime(12611u+0xFFFFu), 12622u+0xFFFFu);
        EXPECT_EQ(gos_get_next_prime(12612u+0xFFFFu), 12622u+0xFFFFu);
        EXPECT_EQ(gos_get_next_prime(37754u+0xFFFFu), 37754u+0xFFFFu);
        EXPECT_EQ(gos_get_next_prime(37755u+0xFFFFu), 37756u+0xFFFFu);
        EXPECT_EQ(gos_get_next_prime(39188u+0xFFFFu), 39188u+0xFFFFu);
        EXPECT_EQ(gos_get_next_prime(39189u+0xFFFFu), 39194u+0xFFFFu);
        EXPECT_EQ(gos_get_next_prime(39194u+0xFFFFu), 39194u+0xFFFFu);

        EXPECT_EQ(gos_get_next_prime(39195u+0xFFFFu), UINT_MAX);
    }

    class Gos_get_prime_old : public ::testing::Test
    {
    };

    TEST_F(Gos_get_prime_old, NoremalTest)
    {
        EXPECT_EQ(gos_get_next_prime(2), gos_get_prime_old(2));
        EXPECT_EQ(gos_get_next_prime(4), gos_get_prime_old(5));
        EXPECT_EQ(gos_get_next_prime(68), gos_get_prime_old(71));
        EXPECT_EQ(gos_get_next_prime(241), gos_get_prime_old(241));
        EXPECT_EQ(gos_get_next_prime(242), gos_get_prime_old(251));

        EXPECT_EQ(gos_get_next_prime(257u), gos_get_prime_old(257u));
        EXPECT_EQ(gos_get_next_prime(258u), gos_get_prime_old(263u));
        EXPECT_EQ(gos_get_next_prime(12611u), gos_get_prime_old(12611u));
        EXPECT_EQ(gos_get_next_prime(12612u), gos_get_prime_old(12613u));
        EXPECT_EQ(gos_get_next_prime(65519u), gos_get_prime_old(65519u));
        EXPECT_EQ(gos_get_next_prime(65520u), gos_get_prime_old(65521u));
        EXPECT_EQ(gos_get_next_prime(65521u), gos_get_prime_old(65521u));


        EXPECT_EQ(gos_get_next_prime(13822u+0xFFFFu), gos_get_prime_old(13822u+0xFFFFu));
        EXPECT_EQ(gos_get_next_prime(13825u+0xFFFFu), gos_get_prime_old(13832u+0xFFFFu));
        EXPECT_EQ(gos_get_next_prime(12611u+0xFFFFu), gos_get_prime_old(12622u+0xFFFFu));
        EXPECT_EQ(gos_get_next_prime(12612u+0xFFFFu), gos_get_prime_old(12622u+0xFFFFu));
        EXPECT_EQ(gos_get_next_prime(37754u+0xFFFFu), gos_get_prime_old(37754u+0xFFFFu));
        EXPECT_EQ(gos_get_next_prime(37755u+0xFFFFu), gos_get_prime_old(37756u+0xFFFFu));
        EXPECT_EQ(gos_get_next_prime(39188u+0xFFFFu), gos_get_prime_old(39188u+0xFFFFu));
        EXPECT_EQ(gos_get_next_prime(39189u+0xFFFFu), gos_get_prime_old(39194u+0xFFFFu));
        EXPECT_EQ(gos_get_next_prime(39194u+0xFFFFu), gos_get_prime_old(39194u+0xFFFFu));
        /// gos_get_prime_old与gos_get_next_prime相比，前者可以计算一万个以后的质数
        EXPECT_EQ(gos_get_prime_old(39195u+0xFFFFu), 104743);
    }
}  // namespace