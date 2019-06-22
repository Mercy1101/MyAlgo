#include <vector>
#include <map>
#include <random>
#include "gtest/gtest.h"
#include "algorithm_imp.h"

namespace {
    class Unique : public ::testing::Test
    {};

    TEST_F(Unique, NormalTest)
    {
        using Lee::STL::unique;
        std::vector<int> vInput;
        vInput.push_back(1);
        vInput.push_back(1);
        vInput.push_back(2);
        vInput.push_back(1);
        vInput.push_back(2);
        vInput.push_back(9);
        vInput.push_back(3);
        vInput.push_back(1);
        vInput.push_back(4);
        vInput.push_back(4);
        vInput.push_back(9);
        vInput.push_back(1);
        vInput.push_back(4);
        vInput.push_back(1);

        unique<int>(vInput);
        EXPECT_EQ(vInput[0], 1);
        EXPECT_EQ(vInput[1], 2);
        EXPECT_EQ(vInput[2], 3);
        EXPECT_EQ(vInput[3], 4);
        EXPECT_EQ(vInput[4], 9);
    }

    TEST_F(Unique, Double_Test)
    {
        using Lee::STL::unique;
        std::vector<double> vInput;
        vInput.push_back(1.00000001);
        vInput.push_back(1.00000001);
        vInput.push_back(1.00000002);
        vInput.push_back(1.00000001);
        vInput.push_back(1.00000002);
        vInput.push_back(1.00000009);
        vInput.push_back(1.00000003);
        vInput.push_back(1.00000001);
        vInput.push_back(1.00000004);
        vInput.push_back(1.00000004);
        vInput.push_back(1.00000009);
        vInput.push_back(1.00000001);
        vInput.push_back(1.00000004);
        vInput.push_back(1.00000001);

        unique<double>(vInput);
        EXPECT_EQ(vInput[0], 1.00000001);
        EXPECT_EQ(vInput[1], 1.00000002);
        EXPECT_EQ(vInput[2], 1.00000003);
        EXPECT_EQ(vInput[3], 1.00000004);
        EXPECT_EQ(vInput[4], 1.00000009);
    }

    TEST_F(Unique, String_Test)
    {
        using Lee::STL::unique;
        std::vector<std::string> vInput;
        vInput.push_back("1.00000001");
        vInput.push_back("1.00000001");
        vInput.push_back("1.00000002");
        vInput.push_back("1.00000001");
        vInput.push_back("1.00000002");
        vInput.push_back("1.00000009");
        vInput.push_back("1.00000003");
        vInput.push_back("1.00000001");
        vInput.push_back("1.00000004");
        vInput.push_back("1.00000004");
        vInput.push_back("1.00000009");
        vInput.push_back("1.00000001");
        vInput.push_back("1.00000004");
        vInput.push_back("1.00000001");

        unique<std::string>(vInput);
        EXPECT_STREQ(vInput[0].c_str(), "1.00000001");
        EXPECT_STREQ(vInput[1].c_str(), "1.00000002");
        EXPECT_STREQ(vInput[2].c_str(), "1.00000003");
        EXPECT_STREQ(vInput[3].c_str(), "1.00000004");
        EXPECT_STREQ(vInput[4].c_str(), "1.00000009");
    }

    class QuickSort : public ::testing::Test
    {
    public : 
        template<typename T>
        static
        bool GP_Compare(const T &a, const T &b)
        {
            return a > b;
        }

        struct stDouble
        {
            int     first;
            double  second;
        };

        static bool Struct_Compare(stDouble &a, stDouble &b)
        {
            return (a.second > b.second);
        }
    };

    TEST_F(QuickSort, Int)
    {
        using namespace Lee::STL;
        RANDOM_INIT();
        const int MAX_ELEMENTS = 10;
        int list[MAX_ELEMENTS] = {6, 9, 5, 2, 4, 8, 7, 0, 1, 3};

        // sort the list using quicksort
        quicksort(list, 0, MAX_ELEMENTS-1, QuickSort::GP_Compare<int>);

        for (int i = 0; i < MAX_ELEMENTS-1; i++)
        {
            EXPECT_EQ(list[i], i);
        }
    }

    TEST_F(QuickSort, Double)
    {
        using namespace Lee::STL;
        RANDOM_INIT();
        const int MAX_ELEMENTS = 10;
        double list[MAX_ELEMENTS] = {6.00000002,
                                     9.00000002,
                                     5.00000002,
                                     2.00000002,
                                     4.00000002,
                                     8.00000002,
                                     7.00000002,
                                     0.00000002,
                                     1.00000002,
                                     3.00000002};

        // sort the list using quicksort
        quicksort(list, 0, MAX_ELEMENTS-1, QuickSort::GP_Compare<double>);

        for (int i = 0; i < MAX_ELEMENTS-1; i++)
        {
            EXPECT_DOUBLE_EQ(list[i], static_cast<double>(i)+0.00000002);
        }
    }

    TEST_F(QuickSort, Struct)
    {
        using namespace Lee::STL;
        RANDOM_INIT();

        const int MAX_ELEMENTS = 10;
        stDouble list[MAX_ELEMENTS] = {
            {1, 6.00000002}, 
            {1, 9.00000002},
            {1, 5.00000002},
            {1, 2.00000002},
            {1, 4.00000002},
            {1, 8.00000002},
            {1, 7.00000002},
            {1, 0.00000002},
            {1, 1.00000002},
            {1, 3.00000002}};

        // sort the list using quicksort
        quicksort(list, 0, MAX_ELEMENTS-1, QuickSort::Struct_Compare);

        for (int i = 0; i < MAX_ELEMENTS-1; i++)
        {
            EXPECT_DOUBLE_EQ(list[i].second, static_cast<double>(i)+0.00000002);
        }
    }

    class Sort : public ::testing::Test
    {
    };

    TEST_F(Sort, NormalTest)
    {
        using namespace Lee::STL;
        int arr1[1] = { 0 };
        sort(std::begin(arr1), std::end(arr1));
        EXPECT_TRUE(std::is_sorted(std::begin(arr1), std::end(arr1)));

        int arr2[2] = { 1, 0 };
        sort(std::begin(arr2), std::end(arr2));
        EXPECT_TRUE(std::is_sorted(std::begin(arr2), std::end(arr2)));

        int arr3[3] = { 2, 1, 3 };
        sort(std::begin(arr3), std::end(arr3));
        EXPECT_TRUE(std::is_sorted(std::begin(arr3), std::end(arr3)));

        int arr4[100];
        std::random_device rd;
        for (auto i = 0; i != 10; ++i)
        {
            for (auto& n : arr4)
            {
                n = rd() % 65536;
            }
            sort(std::begin(arr4), std::end(arr4));
            EXPECT_TRUE(std::is_sorted(std::begin(arr4), std::end(arr4)));
        }
    }

    class MaxMin : public ::testing::Test
    {};

    TEST_F(MaxMin, NormalTest)
    {
        using namespace Lee::STL;
        int foo, bar;
        foo = 999; bar = 888;
        EXPECT_EQ(max(foo, bar), foo);
        EXPECT_EQ(min(foo, bar), bar);

        double dfoo, dbar;
        dfoo = 0.999000001; dbar = 0.888000001;
        EXPECT_EQ(max(dfoo, dbar), dfoo);
        EXPECT_EQ(min(dfoo, dbar), dbar);
    }

    struct StructComp
    {
        int     first;
        double  second;
    };

    bool MaxCompare(const StructComp &a, const StructComp &b)
    {
        return (a.second > b.second);
    }

    bool MinCompare(const StructComp &a, const StructComp &b)
    {
        return (a.first < b.first);
    }
    
    TEST_F(MaxMin, StructComp)
    {

        using namespace Lee::STL;
        struct StructComp foo, bar;
        foo.first = 999;
        foo.second = 0.999;
        bar.first = 888;
        bar.second = 0.888;

        EXPECT_EQ(max(foo, bar, MaxCompare).second, foo.second);
        EXPECT_EQ(min(foo, bar, MinCompare).first, bar.first);
    }

    class ROUND_UP_Test : public ::testing::Test
    {};

    TEST_F(ROUND_UP_Test, Normal)
    {
        using namespace Lee::STL;
        EXPECT_EQ(ROUND_UP(0), 0);
        EXPECT_EQ(ROUND_UP(31), 32);
        EXPECT_EQ(ROUND_UP(100), 104);
        EXPECT_EQ(ROUND_UP(50), 56);
        EXPECT_EQ(ROUND_UP(49), 56);
    }

}  // namespace





