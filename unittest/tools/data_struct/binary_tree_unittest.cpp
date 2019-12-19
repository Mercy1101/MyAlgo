﻿#include "data_struct/binary_tree.h"
#include <catch2/catch.hpp>
#include "profiler/Profiler.h"

SCENARIO("binary_tree, 二叉树的测试", "[data_struct][binary_tree]") {
  GIVEN("构造函数测试") {
    WHEN("创建一个二叉树") {
      Lee::Binary_Tree<int> tree(1);
      THEN("root不为空指针, root节点的data则为1, 左边和右边都是空指针") {
        REQUIRE(Lee::Binary_Tree<int>::construct_count == 1);
        REQUIRE(Lee::BinaryTreeNode<int>::construct_count == 1);
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
        REQUIRE(Lee::BinaryTreeNode<int>::construct_count == 6);
        REQUIRE_FALSE(insert_tree.GetRootNode() == nullptr);
        REQUIRE(5 == insert_tree.GetHeight());
        REQUIRE(6 == insert_tree.Size());
      }
      THEN("树中所有元素靠右, 分别为1, 2, 3, 4, 5, 6.所有左子树都为空") {
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
        // node_ptr = node_ptr->right;
      }
    }
    WHEN("Insert函数测试结束时") {
      THEN("树本身和树节点, 应该被正确析构") {
        REQUIRE(Lee::Binary_Tree<int>::construct_count == 0);
        REQUIRE(Lee::BinaryTreeNode<int>::construct_count == 0);
      }
    }

    WHEN("创建一个二叉树, 并插入8个不重复数值") {
      Lee::Binary_Tree<int> insert_tree(5);
      insert_tree.Insert(2);
      insert_tree.Insert(7);
      insert_tree.Insert(1);
      insert_tree.Insert(3);
      insert_tree.Insert(6);
      insert_tree.Insert(8);
      insert_tree.Insert(4);
      THEN("这个树被实例化了一次, 节点不为空,高度为3,且元素个数为8") {
        REQUIRE(Lee::Binary_Tree<int>::construct_count == 1);
        REQUIRE(Lee::BinaryTreeNode<int>::construct_count == 8);
        REQUIRE_FALSE(insert_tree.GetRootNode() == nullptr);
        REQUIRE(3 == insert_tree.GetHeight());
        REQUIRE(8 == insert_tree.Size());
      }
      THEN("树中所有元素的值, 分别为5,2,7,1,3,6,8,4") {
        auto root_node_ptr = insert_tree.GetRootNode();
        /// 根节点
        REQUIRE(5 == root_node_ptr->data);
        /// 第二层
        REQUIRE(2 == root_node_ptr->left->data);
        REQUIRE(7 == root_node_ptr->right->data);
        /// 第三层
        REQUIRE(1 == root_node_ptr->left->left->data);
        REQUIRE(nullptr == root_node_ptr->left->left->left);
        REQUIRE(nullptr == root_node_ptr->left->left->right);
        REQUIRE(3 == root_node_ptr->left->right->data);
        REQUIRE(nullptr == root_node_ptr->left->right->left);
        REQUIRE(6 == root_node_ptr->right->left->data);
        REQUIRE(nullptr == root_node_ptr->right->left->left);
        REQUIRE(nullptr == root_node_ptr->right->left->right);
        REQUIRE(8 == root_node_ptr->right->right->data);
        REQUIRE(nullptr == root_node_ptr->right->right->left);
        REQUIRE(nullptr == root_node_ptr->right->right->right);
        /// 第四层
        REQUIRE(4 == root_node_ptr->left->right->right->data);
        REQUIRE(nullptr == root_node_ptr->left->right->right->left);
        REQUIRE(nullptr == root_node_ptr->left->right->right->right);
      }
    }
    WHEN("创建一个二叉树, 并插入8个不重复数值函数测试结束时") {
      THEN("树本身和树节点, 应该被正确析构") {
        REQUIRE(Lee::Binary_Tree<int>::construct_count == 0);
        REQUIRE(Lee::BinaryTreeNode<int>::construct_count == 0);
      }
    }
    WHEN("创建一个二叉树, 并插入8个相同的数值") {
      Lee::Binary_Tree<int> insert_tree(5);
      insert_tree.Insert(5);
      insert_tree.Insert(5);
      insert_tree.Insert(5);
      insert_tree.Insert(5);
      insert_tree.Insert(5);
      insert_tree.Insert(5);
      insert_tree.Insert(5);
      THEN("这个树被实例化了一次, 节点不为空, 高度为0,且元素个数为1") {
        REQUIRE(Lee::Binary_Tree<int>::construct_count == 1);
        REQUIRE(Lee::BinaryTreeNode<int>::construct_count == 1);
        REQUIRE_FALSE(insert_tree.GetRootNode() == nullptr);
        REQUIRE(0 == insert_tree.GetHeight());
        REQUIRE(1 == insert_tree.Size());
      }
      THEN("树中只有一个元素,其值为5") {
        auto root_node_ptr = insert_tree.GetRootNode();
        /// 根节点
        REQUIRE(5 == root_node_ptr->data);
        /// 第二层
        REQUIRE(nullptr == root_node_ptr->left);
        REQUIRE(nullptr == root_node_ptr->right);
      }
    }
    WHEN("创建一个二叉树, 并插入8个相同的数值函数测试结束时") {
      THEN("树本身和树节点, 应该被正确析构") {
        REQUIRE(Lee::Binary_Tree<int>::construct_count == 0);
        REQUIRE(Lee::BinaryTreeNode<int>::construct_count == 0);
      }
    }
    WHEN("创建一个二叉树, 并插入7个不重复的数值, 再插入7个重复数值") {
      Lee::Binary_Tree<int> insert_tree(5);
      insert_tree.Insert(2);
      insert_tree.Insert(7);
      insert_tree.Insert(1);
      insert_tree.Insert(3);
      insert_tree.Insert(6);
      insert_tree.Insert(8);
      insert_tree.Insert(4);
      /// 插入重复数值
      insert_tree.Insert(5);
      insert_tree.Insert(2);
      insert_tree.Insert(7);
      insert_tree.Insert(1);
      insert_tree.Insert(3);
      insert_tree.Insert(6);
      insert_tree.Insert(8);
      insert_tree.Insert(4);
      THEN("这个树被实例化了一次, 节点不为空, 高度为0,且元素个数为8") {
        REQUIRE(Lee::Binary_Tree<int>::construct_count == 1);
        REQUIRE(Lee::BinaryTreeNode<int>::construct_count == 8);
        REQUIRE_FALSE(insert_tree.GetRootNode() == nullptr);
        REQUIRE(3 == insert_tree.GetHeight());
        REQUIRE(8 == insert_tree.Size());
      }
      THEN("树中所有元素的值, 分别为5,2,7,1,3,6,4,8") {
        auto root_node_ptr = insert_tree.GetRootNode();
        /// 根节点
        REQUIRE(5 == root_node_ptr->data);
        /// 第二层
        REQUIRE(2 == root_node_ptr->left->data);
        REQUIRE(7 == root_node_ptr->right->data);
        /// 第三层
        REQUIRE(1 == root_node_ptr->left->left->data);
        REQUIRE(nullptr == root_node_ptr->left->left->left);
        REQUIRE(nullptr == root_node_ptr->left->left->right);
        REQUIRE(3 == root_node_ptr->left->right->data);
        REQUIRE(nullptr == root_node_ptr->left->right->left);
        REQUIRE(6 == root_node_ptr->right->left->data);
        REQUIRE(nullptr == root_node_ptr->right->left->left);
        REQUIRE(nullptr == root_node_ptr->right->left->right);
        REQUIRE(8 == root_node_ptr->right->right->data);
        REQUIRE(nullptr == root_node_ptr->right->right->left);
        REQUIRE(nullptr == root_node_ptr->right->right->right);
        /// 第四层
        REQUIRE(4 == root_node_ptr->left->right->right->data);
        REQUIRE(nullptr == root_node_ptr->left->right->right->left);
        REQUIRE(nullptr == root_node_ptr->left->right->right->right);
      }
    }
    WHEN("Insert函数重复插入测试结束时") {
      THEN("树本身和树节点, 应该被正确析构") {
        REQUIRE(Lee::Binary_Tree<int>::construct_count == 0);
        REQUIRE(Lee::BinaryTreeNode<int>::construct_count == 0);
      }
    }

    WHEN("Find函数测试, 在树中找寻特定的值") {
      Lee::Binary_Tree<int> tree(5);
      THEN("找寻数值为5的节点是否存在") {
        REQUIRE_FALSE(nullptr == tree.Find(5));
        REQUIRE(5 == tree.Find(5)->data);
      }
      THEN("找寻不存在的的值, 应该返回 nullptr") {
        REQUIRE(nullptr == tree.Find(0));
        REQUIRE(nullptr == tree.Find(6));
        REQUIRE(nullptr == tree.Find(60));
        REQUIRE(nullptr == tree.Find(66));
      }
      tree.Insert(2);
      tree.Insert(7);
      tree.Insert(1);
      tree.Insert(3);
      tree.Insert(6);
      tree.Insert(8);
      tree.Insert(4);
      THEN("找寻刚刚根节点的数值, 并验证返回的指针是有效的") {
        auto node_ptr = tree.Find(5);
        /// 根节点
        REQUIRE(5 == node_ptr->data);
        /// 第二层
        REQUIRE(2 == node_ptr->left->data);
        REQUIRE(7 == node_ptr->right->data);
        /// 第三层
        REQUIRE(1 == node_ptr->left->left->data);
        REQUIRE(nullptr == node_ptr->left->left->left);
        REQUIRE(nullptr == node_ptr->left->left->right);
        REQUIRE(3 == node_ptr->left->right->data);
        REQUIRE(nullptr == node_ptr->left->right->left);
        REQUIRE(6 == node_ptr->right->left->data);
        REQUIRE(nullptr == node_ptr->right->left->left);
        REQUIRE(nullptr == node_ptr->right->left->right);
        REQUIRE(8 == node_ptr->right->right->data);
        REQUIRE(nullptr == node_ptr->right->right->left);
        REQUIRE(nullptr == node_ptr->right->right->right);
        /// 第四层
        REQUIRE(4 == node_ptr->left->right->right->data);
        REQUIRE(nullptr == node_ptr->left->right->right->left);
        REQUIRE(nullptr == node_ptr->left->right->right->right);
      }
      THEN("找寻刚刚插入的所有数值是否都能找到") {
        REQUIRE_FALSE(nullptr == tree.Find(2));
        REQUIRE(2 == tree.Find(2)->data);

        REQUIRE_FALSE(nullptr == tree.Find(1));
        REQUIRE(1 == tree.Find(1)->data);

        REQUIRE_FALSE(nullptr == tree.Find(7));
        REQUIRE(7 == tree.Find(7)->data);

        REQUIRE_FALSE(nullptr == tree.Find(3));
        REQUIRE(3 == tree.Find(3)->data);

        REQUIRE_FALSE(nullptr == tree.Find(6));
        REQUIRE(6 == tree.Find(6)->data);

        REQUIRE_FALSE(nullptr == tree.Find(8));
        REQUIRE(8 == tree.Find(8)->data);

        REQUIRE_FALSE(nullptr == tree.Find(4));
        REQUIRE(4 == tree.Find(4)->data);
      }
    }

    WHEN("Find函数测试结束时") {
      THEN("树本身和树节点, 应该被正确析构") {
        REQUIRE(Lee::Binary_Tree<int>::construct_count == 0);
        REQUIRE(Lee::BinaryTreeNode<int>::construct_count == 0);
      }
    }
  }
}