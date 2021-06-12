#include "catch/catch.hpp"
#include "my_algo/my_algo.hpp"

#pragma warning(disable : 4819)

TEST_CASE("lee::optional() 简单测�?", "[lee][optional]")
{
    lee::optional<int> op;
    REQUIRE(!op.has_value());
    REQUIRE(op.value_or(1111) == 1111);
}

TEST_CASE("lee::optional(T) 简单测�?", "[lee][optional]")
{
    lee::optional<int> op(1);
    REQUIRE(op.has_value());
    REQUIRE(op.value_or(1111) == 1);
}

TEST_CASE("lee::optional(const optional& other) 简单测�?", "[lee][optional]")
{
    lee::optional<int> op;
    auto temp = op;
    REQUIRE(!temp.has_value());
    REQUIRE(temp.value_or(1111) == 1111);

    lee::optional<int> op2(2);
    auto temp2(op2);
    REQUIRE(temp2.has_value());
    REQUIRE(temp2.value_or(1111) == 2);
}
