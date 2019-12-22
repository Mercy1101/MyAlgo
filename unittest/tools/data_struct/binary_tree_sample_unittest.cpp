#include <binary_tree_sample.h>

#include <catch2/catch.hpp>

TEMPLATE_LIST_TEST_CASE("Lee::BinarySearchTree 的测试用例",
                        "[data_struct][binary_tree_sample]", int, double) {
  SECTION("基础构造函数测试") {
    Lee::BinarySearchTree<typename> tree;
    REQUIRE(tree.root == nullptr);
  }
  SECTION("初值列表构造函数测试") {
    Lee::BinarySearchTree<typename> tree{5, 6, 7, 8, 2, 3, 10};
    REQUIRE(tree.contains({2, nullptr, nullptr}));
    REQUIRE(tree.contains({3, nullptr, nullptr}));
    REQUIRE(tree.contains({4, nullptr, nullptr}));
    REQUIRE(tree.contains({5, nullptr, nullptr}));
    REQUIRE(tree.contains({6, nullptr, nullptr}));
    REQUIRE(tree.contains({7, nullptr, nullptr}));
    REQUIRE(tree.contains({10, nullptr, nullptr}));
  }

  SECTION("析构函数测试") {
    Lee::BinarySearchTree<typename> tree;
    tree.insert(50);
  }

  SECTION("Insert函数测试") {
    Lee::BinarySearchTree<typename> tree;
    tree.insert(50);
  }
}
