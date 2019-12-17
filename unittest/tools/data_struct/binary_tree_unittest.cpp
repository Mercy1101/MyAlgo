#include "data_struct/binary_tree.h"
#include <catch2/catch.hpp>
#include "profiler/Profiler.h"

SCENARIO("binary_tree, 二叉树的测试", "[data_struct][binary_tree]") {
  GIVEN("构造函数测试") {
    WHEN("创建一个二叉树") {
      Lee::Binary_Tree<int> tree(1);
      THEN("root不为空指针, root节点的data则为1, 左边和右边都是空指针") {
        REQUIRE(Lee::Binary_Tree<int>::construct_count == 1);
        REQUIRE_FALSE(tree.GetRootNode() == nullptr);
        REQUIRE(tree.GetRootNode()->data == 1);
        REQUIRE(tree.GetRootNode()->left == nullptr);
        REQUIRE(tree.GetRootNode()->right == nullptr);
      }
    }
    WHEN("创建一个二叉树测试用例结束时") {
      THEN("树本身和树节点, 应该被正确析构") {
        REQUIRE(Lee::Binary_Tree<int>::construct_count == 0);
        REQUIRE(Lee::BinaryTreeNode<int>::construct_count == 0);
      }
    }
  }
  GIVEN("Insert函数测试") {
    WHEN("创建一个二叉树, 并插入5个数值") {
      Lee::Binary_Tree<int> insert_tree(1);
      insert_tree.Insert(2);
      insert_tree.Insert(3);
      insert_tree.Insert(4);
      insert_tree.Insert(5);
      insert_tree.Insert(6);
      THEN("这个树被实例化了一次, 节点不为空,高度为5,且元素个数为6") {
        REQUIRE(Lee::Binary_Tree<int>::construct_count == 1);
        REQUIRE_FALSE(insert_tree.GetRootNode() == nullptr);
        REQUIRE(5 == insert_tree.GetHeight());
        REQUIRE(6 == insert_tree.Size());
      }
      THEN("树中所有元素靠右, 分别为1, 2, 3, 4, 5, 6") {
        auto node_ptr = insert_tree.GetRootNode();
        REQUIRE(1 == node_ptr->data);
        REQUIRE(nullptr == node_ptr->left);
        node_ptr = node_ptr->right;
        REQUIRE(2 == node_ptr->data);
        REQUIRE(nullptr == node_ptr->left);
        node_ptr = node_ptr->right;
        REQUIRE(3 == node_ptr->data);
        REQUIRE(nullptr == node_ptr->left);
        node_ptr = node_ptr->right;
        REQUIRE(4 == node_ptr->data);
        REQUIRE(nullptr == node_ptr->left);
        node_ptr = node_ptr->right;
        REQUIRE(5 == node_ptr->data);
        REQUIRE(nullptr == node_ptr->left);
        node_ptr = node_ptr->right;
      }
    }
    WHEN("Insert函数测试结束时") {
      THEN("树本身和树节点, 应该被正确析构") {
        REQUIRE(Lee::Binary_Tree<int>::construct_count == 0);
        REQUIRE(Lee::BinaryTreeNode<int>::construct_count == 0);
      }
    }
  }
}