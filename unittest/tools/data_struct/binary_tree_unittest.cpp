#include "data_struct/binary_tree.h"

#include <catch2/catch.hpp>

#include "profiler/Profiler.h"

SCENARIO("binary_tree 二叉树的构造函数测试", "[data_struct][binary_tree]") {
  GIVEN("一个单独的元素来创建一个树") {
    WHEN("数值类型为int时") {
      Lee::Binary_Tree<int> tree(1);
      THEN("root不为空指针, root节点的data则为1, 左边和右边都是空指针") {
        REQUIRE(tree.GetRoot()->left == nullptr);
        REQUIRE(tree.GetRoot()->right == nullptr);
        REQUIRE(tree.GetRoot()->data == 1);
      } /// THEN
    } /// WHEN
    REQUIRE(0 == Lee::Binary_Tree<int>::BinaryTreeNode::construct_count);
  } /// GIVEN
  GIVEN("一个初值列表来创建一个树") {
    WHEN("只有一个元素的初值列表") {
      Lee::Binary_Tree<int> tree{1};
      THEN("root不为空指针, root节点的data则为1, 左边和右边都是空指针") {
        REQUIRE(tree.GetRoot()->left == nullptr);
        REQUIRE(tree.GetRoot()->right == nullptr);
        REQUIRE(tree.GetRoot()->data == 1);
      } /// THEN
    } /// WHEN
    WHEN("有多个元素的初值列表") {
      Lee::Binary_Tree<int> tree{5, 2, 8, 7, 6, 10};
      THEN("root不为空指针， 一共有六个节点") {
        /// 验证有几个节点被创建
        REQUIRE(Lee::Binary_Tree<int>::BinaryTreeNode::construct_count == 6);
        /// 验证root节点不为空
        auto node_ptr = tree.GetRoot();
        REQUIRE(node_ptr != nullptr);
        /// #1（root）第0层
        REQUIRE(node_ptr->data == 5);
        /// #2 第1层左节点 （叶子节点）
        REQUIRE(node_ptr->left->data == 2);
        /// #3 第1层右节点
        REQUIRE(node_ptr->right->data == 8);
        /// #4 第2层左节点 (#3 的左节点)
        REQUIRE(node_ptr->right->left->data == 7);
        /// #5 第2层右节点 （叶子节点）(#3 的右节点)
        REQUIRE(node_ptr->right->right->data == 10);
        /// #6 第3层左节点 （叶子节点）（#4 的左节点）
        REQUIRE(node_ptr->right->left->left->data == 6);

        /// #2 叶子节点的验证
        REQUIRE(node_ptr->left->left == nullptr);
        REQUIRE(node_ptr->left->right == nullptr);
        /// #5 叶子节点的验证
        REQUIRE(node_ptr->right->right->left == nullptr);
        REQUIRE(node_ptr->right->right->right == nullptr);
        /// #6 叶子节点的验证
        REQUIRE(node_ptr->right->left->left->left == nullptr);
        REQUIRE(node_ptr->right->left->left->right == nullptr);
        ///// #4 节点的右子树为空的验证
        REQUIRE(node_ptr->right->left->right == nullptr);
      } /// THEN
    } /// WHEN
    REQUIRE(Lee::Binary_Tree<int>::BinaryTreeNode::construct_count == 0);
    WHEN("零个元素的初值列表")
    {
      Lee::Binary_Tree<int> tree{};
      THEN("会默认构造一个数值为0的根节点")
      {
        REQUIRE(Lee::Binary_Tree<int>::BinaryTreeNode::construct_count == 1);
        REQUIRE(tree.GetRoot() != nullptr);
        REQUIRE(tree.GetRoot()->data == 0);
      } /// THEN
    } /// WHEN
    REQUIRE(Lee::Binary_Tree<int>::BinaryTreeNode::construct_count == 0);
  } /// GIVEN
} /// SCENARIO
