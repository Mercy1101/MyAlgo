#include <catch2/catch.hpp>

#include "data_struct/binary_tree.h"
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
    WHEN("使用初始化链表来构造一个二叉树") {
      Lee::Binary_Tree<int> tree{1};
      REQUIRE(tree.Size() == 1);
      REQUIRE(tree.GetHeight() == 0);
      REQUIRE(tree.GetRootNode()->data == 1);

      Lee::Binary_Tree<int> tree2{5, 6, 1, 3, 4, 7, 8, 9, 10};
      REQUIRE(tree2.Size() == 9);
      REQUIRE(5 == tree2.GetHeight());
      REQUIRE(tree2.GetRootNode()->data == 5);
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
  }

  GIVEN("测试Find函数") {
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

  GIVEN("Delete函数测试") {
    WHEN("删除一个树叶（没有子节点），然后再删除一个") {
      Lee::Binary_Tree<int> delete_tree(5);
      delete_tree.Insert(2);
      delete_tree.Insert(7);
      delete_tree.Insert(1);
      delete_tree.Insert(3);
      delete_tree.Insert(6);
      delete_tree.Insert(8);
      delete_tree.Insert(4);
      /// 删除一个树叶
      delete_tree.Delete(4);
      THEN("现在的树有七个节点组成， 高度而2") {
        REQUIRE(Lee::Binary_Tree<int>::construct_count == 1);
        REQUIRE(Lee::BinaryTreeNode<int>::construct_count == 7);
        REQUIRE(delete_tree.GetHeight() == 2);
      }
      THEN("删除后是一个完全二叉树") {
        auto node_ptr = delete_tree.GetRootNode();
        /// root
        REQUIRE(node_ptr->data == 5);
        /// 第二层
        REQUIRE(node_ptr->left->data == 2);
        REQUIRE(node_ptr->right->data == 7);
        /// 第三层
        REQUIRE(node_ptr->left->left->data == 1);
        REQUIRE(node_ptr->left->right->data == 3);
        REQUIRE(node_ptr->right->left->data == 6);
        REQUIRE(node_ptr->right->right->data == 8);
        /// 第四层全部为空
        REQUIRE(node_ptr->left->left->left == nullptr);
        REQUIRE(node_ptr->left->left->right == nullptr);
        REQUIRE(node_ptr->left->right->left == nullptr);
        REQUIRE(node_ptr->left->right->right == nullptr);
        REQUIRE(node_ptr->right->left->left == nullptr);
        REQUIRE(node_ptr->right->left->right == nullptr);
        REQUIRE(node_ptr->right->right->left == nullptr);
        REQUIRE(node_ptr->right->right->right == nullptr);
      }
      //   /// 删除一个拥有两个节点的节点
      //   delete_tree.Delete(2);
      //   THEN("拥有六个节点, 高度仍为2") {
      //     REQUIRE(Lee::Binary_Tree<int>::construct_count == 1);
      //     REQUIRE(Lee::BinaryTreeNode<int>::construct_count == 6);
      //     REQUIRE(delete_tree.GetHeight() == 2);
      //   }
      //   THEN("接下来判断树节点的有效性") {
      //     auto node_ptr = delete_tree.GetRootNode();
      //     /// root
      //     REQUIRE(node_ptr->data == 5);
      //     /// 第二层
      //     REQUIRE(node_ptr->left->data == 3);
      //     REQUIRE(node_ptr->right->data == 7);
      //     /// 第三层
      //     REQUIRE(node_ptr->left->left->data == 1);
      //     REQUIRE(node_ptr->left->right == nullptr);
      //     REQUIRE(node_ptr->right->left->data == 6);
      //     REQUIRE(node_ptr->right->right->data == 8);
      //     /// 第四层全部为空
      //     REQUIRE(node_ptr->left->left->left == nullptr);
      //     REQUIRE(node_ptr->left->left->right == nullptr);
      //     REQUIRE(node_ptr->right->left->left == nullptr);
      //     REQUIRE(node_ptr->right->left->right == nullptr);
      //     REQUIRE(node_ptr->right->right->left == nullptr);
      //     REQUIRE(node_ptr->right->right->right == nullptr);
      //   }
    }
  }
}
