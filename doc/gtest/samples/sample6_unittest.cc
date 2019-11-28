// Copyright 2008 Google Inc.
// All Rights Reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


// This sample shows how to test common properties of multiple
// implementations of the same interface (aka interface tests).

// The interface and its implementations are in this header.
#include "prime_tables.h"

#include "gtest/gtest.h"
namespace {
// First, we define some factory functions for creating instances of
// the implementations.  You may be able to skip this step if all your
// implementations can be constructed the same way.

template <class T>
PrimeTable* CreatePrimeTable();

template <>
PrimeTable* CreatePrimeTable<OnTheFlyPrimeTable>()
{
  return new OnTheFlyPrimeTable;
}

template <>
PrimeTable* CreatePrimeTable<PreCalculatedPrimeTable>()
{
  return new PreCalculatedPrimeTable(10000);
}

// Then we define a test fixture class template.
template <class T>
class PrimeTableTest : public testing::Test
{
 protected:
    // The ctor calls the factory function to create a prime table
    // implemented by T.
    PrimeTableTest() : table_(CreatePrimeTable<T>()) {}

    ~PrimeTableTest() override { delete table_; }

    // Note that we test an implementation via the base interface
    // instead of the actual implementation class.  This is important
    // for keeping the tests close to the real world scenario, where the
    // implementation is invoked via the base interface.  It avoids
    // got-yas where the implementation class has a method that shadows
    // a method with the same name (but slightly different argument
    // types) in the base interface, for example.
    PrimeTable* const table_;
};

#if GTEST_HAS_TYPED_TEST

using testing::Types;

// Google Test提供了两种重用不同类型测试的方法。
// 第一个称为“类型化测试”。 你应该使用它
// 已经知道*所有*你写作时要锻炼的类型测试

// 要编写一个类型化的测试用例，请先使用
//
// TYPED_TEST_SUITE（TestCaseName，TypeList）;
//
// 声明它并指定类型参数。 与TEST_F一样，
// TestCaseName必须与测试夹具名称匹配。

// 我们要测试的类型列表。
typedef Types<OnTheFlyPrimeTable, PreCalculatedPrimeTable> Implementations;

TYPED_TEST_SUITE(PrimeTableTest, Implementations);

// Then use TYPED_TEST(TestCaseName, TestName) to define a typed test,
// similar to TEST_F.
TYPED_TEST(PrimeTableTest, ReturnsFalseForNonPrimes)
{
    // Inside the test body, you can refer to the type parameter by
    // TypeParam, and refer to the fixture class by TestFixture.  We
    // don't need them in this example.

    // Since we are in the template world, C++ requires explicitly
    // writing 'this->' when referring to members of the fixture class.
    // This is something you have to learn to live with.
    EXPECT_FALSE(this->table_->IsPrime(-5));
    EXPECT_FALSE(this->table_->IsPrime(0));
    EXPECT_FALSE(this->table_->IsPrime(1));
    EXPECT_FALSE(this->table_->IsPrime(4));
    EXPECT_FALSE(this->table_->IsPrime(6));
    EXPECT_FALSE(this->table_->IsPrime(100));
}

TYPED_TEST(PrimeTableTest, ReturnsTrueForPrimes)
{
    EXPECT_TRUE(this->table_->IsPrime(2));
    EXPECT_TRUE(this->table_->IsPrime(3));
    EXPECT_TRUE(this->table_->IsPrime(5));
    EXPECT_TRUE(this->table_->IsPrime(7));
    EXPECT_TRUE(this->table_->IsPrime(11));
    EXPECT_TRUE(this->table_->IsPrime(131));
}

TYPED_TEST(PrimeTableTest, CanGetNextPrime)
{
    EXPECT_EQ(2, this->table_->GetNextPrime(0));
    EXPECT_EQ(3, this->table_->GetNextPrime(2));
    EXPECT_EQ(5, this->table_->GetNextPrime(3));
    EXPECT_EQ(7, this->table_->GetNextPrime(5));
    EXPECT_EQ(11, this->table_->GetNextPrime(7));
    EXPECT_EQ(131, this->table_->GetNextPrime(128));
}

// That's it!  Google Test will repeat each TYPED_TEST for each type
// in the type list specified in TYPED_TEST_SUITE.  Sit back and be
// happy that you don't have to define them multiple times.

#endif  // GTEST_HAS_TYPED_TEST

#if GTEST_HAS_TYPED_TEST_P

using testing::Types;

// 但是，有时您还不知道所需的所有类型
// 测试你何时编写测试。 例如，如果你是
// 界面的作者，并希望其他人实现它，你
// 可能想编写一组测试来确保每个实现
// 符合一些基本要求，但你不知道是什么
// 实现将在未来编写。
//
// 如何在不提交类型的情况下编写测试
// 参数？ 这就是“类型参数化测试”可以为您做的事情。
// 它比打字测试更复杂，但作为回报，你会得到一个
// 可以在许多上下文中重用的测试模式，这是一个很大的问题
// 赢 这是你如何做到的：

// 首先，定义一个测试夹具类模板。 在这里我们只是重用
// 前面定义的PrimeTableTest夹具：

template <class T>
class PrimeTableTest2 : public PrimeTableTest<T>
{
};

// 然后，声明测试用例。 参数是测试的名称
// fixture，以及测试用例的名称（像往常一样）。
// _P后缀用于“参数化”或“模式”。
TYPED_TEST_SUITE_P(PrimeTableTest2);

// Next, use TYPED_TEST_P(TestCaseName, TestName) to define a test,
// similar to what you do with TEST_F.
TYPED_TEST_P(PrimeTableTest2, ReturnsFalseForNonPrimes)
{
    EXPECT_FALSE(this->table_->IsPrime(-5));
    EXPECT_FALSE(this->table_->IsPrime(0));
    EXPECT_FALSE(this->table_->IsPrime(1));
    EXPECT_FALSE(this->table_->IsPrime(4));
    EXPECT_FALSE(this->table_->IsPrime(6));
    EXPECT_FALSE(this->table_->IsPrime(100));
}

TYPED_TEST_P(PrimeTableTest2, ReturnsTrueForPrimes) {
  EXPECT_TRUE(this->table_->IsPrime(2));
  EXPECT_TRUE(this->table_->IsPrime(3));
  EXPECT_TRUE(this->table_->IsPrime(5));
  EXPECT_TRUE(this->table_->IsPrime(7));
  EXPECT_TRUE(this->table_->IsPrime(11));
  EXPECT_TRUE(this->table_->IsPrime(131));
}

TYPED_TEST_P(PrimeTableTest2, CanGetNextPrime) {
  EXPECT_EQ(2, this->table_->GetNextPrime(0));
  EXPECT_EQ(3, this->table_->GetNextPrime(2));
  EXPECT_EQ(5, this->table_->GetNextPrime(3));
  EXPECT_EQ(7, this->table_->GetNextPrime(5));
  EXPECT_EQ(11, this->table_->GetNextPrime(7));
  EXPECT_EQ(131, this->table_->GetNextPrime(128));
}

// 类型参数化测试需要一个额外的步骤：你必须这样做
// 枚举您定义的测试：
REGISTER_TYPED_TEST_SUITE_P(
    PrimeTableTest2,  // The first argument is the test case name.
    // The rest of the arguments are the test names.
    ReturnsFalseForNonPrimes, ReturnsTrueForPrimes, CanGetNextPrime);


// 此时测试模式已完成。 但是，你没有
// 任何真正的测试，因为你还没有说出你想要运行的类型
// 测试用。

// 要将抽象测试模式转换为实际测试，请实例化
// 它带有一个类型列表。 通常会定义测试模式
// 在.h文件中，任何人都可以#include并实例化它。 您可以
// 甚至在同一个程序中多次实例化它。 告诉
// 除了不同的实例，你给它们每个人一个名字
// 成为测试用例名称的一部分，可用于测试过滤器。

// 我们要测试的类型列表。 请注意，它不一定是
// 在我们编写TYPED_TEST_P()时定义。

typedef Types<OnTheFlyPrimeTable, PreCalculatedPrimeTable>
    PrimeTableImplementations;
INSTANTIATE_TYPED_TEST_SUITE_P(OnTheFlyAndPreCalculated,    // Instance name
                               PrimeTableTest2,             // Test case name
                               PrimeTableImplementations);  // Type list

#endif  // GTEST_HAS_TYPED_TEST_P
}  // namespace
