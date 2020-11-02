#include "catch/catch.hpp"
#include "my_algo/my_algo.hpp"

TEST_CASE("二维数组序列化与反序列化 简单测试",
          "[lee][two_dimensional_vector]") {
  using namespace lee;
  two_dimensional_vector_serialize p;
  std::vector<std::vector<int>> vec;
  vec.push_back({1, 2});
  vec.push_back({3, 4});
  vec.push_back({5, 6});
  vec.push_back({7, 8});
  REQUIRE(vec == p.deserialize(p.serialize(vec)));
}