#include <climits>

#include "catch/catch.hpp"
#include "my_algo/my_algo.hpp"

#pragma warning(disable : 4819)

TEST_CASE("multi_s() 简单测�?", "[lee][multi_s]")
{
    using namespace lee;

    auto xx0 = multi_s(2, 4);
    REQUIRE(xx0.has_value());
    REQUIRE(xx0.value() == 2 * 4);

    auto xx1 = multi_s(INT_MIN, -1);
    REQUIRE_FALSE(xx1.has_value());

    auto xx2 = multi_s(INT_MIN / 2, 3);
    REQUIRE_FALSE(xx2.has_value());
    REQUIRE(xx2.value_or(1) == 1);
}
