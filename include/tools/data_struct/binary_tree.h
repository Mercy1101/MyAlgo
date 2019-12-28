///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   binary_tree.h
/// @brief  二叉树的实现
///
/// @author lijiancong, pipinstall@163.com
/// @date   2019-12-09 19:56:43
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_DATASTRUCT_BINARY_TREE_H_
#define MYALGO_INCLUDE_TOOLS_DATASTRUCT_BINARY_TREE_H_

#include <algorithm>  // for std::max
#include <atomic>
#include <initializer_list>
#include <vector>

namespace Lee {
inline namespace data_struct {
inline namespace binary_tree {

/// @name     Binary_Tree
/// @brief    二叉树的实现
/// @details  设计的思想是, 数值大的放右边,数值小的放左边,相等的值不会重复插入
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-18 09:36:23
/// @warning  线程不安全
template <typename Comparable>
class Binary_Tree {
  using Position = struct BinaryTreeNode *;
  using Tree = struct BinaryTreeNode *;

 public:
  Binary_Tree(const Comparable &data) : root_(new BinaryTreeNode(data)) {}
  Binary_Tree(const std::initializer_list<Comparable> &init_list) {
    ++construct_count;
    root_ = new BinaryTreeNode<T>(*init_list.begin());
    for (const auto &it : init_list) {
      this->Insert(it);
    }
  }
  ~Binary_Tree() { Destory<T>(root_); }

  void Insert(const Comparable &data) { Insert(data, root_); }

 private:
  void Insert(const Comparable &data, Tree tree) {
    auto node_ptr = tree;
    auto pre_node_ptr = tree;
    while (node_ptr != nullptr) {
      pre_node_ptr = node_ptr;
      if (data > node_ptr->data) {
        node_ptr = node_ptr->right;
      } else if (data < node_ptr->data) {
        node_ptr = node_ptr->left;
      } else {
        /// 相等的情况什么也不做
        return;
      }
    }
    node_ptr = new BinaryTreeNode(data, pre_node_ptr);
  }
  Destory();

 private:
  struct BinaryTreeNode {
    BinaryTreeNode(const Comparable &data, BinaryTreeNode *parent)
        : data(data), left(nullptr), right(nullptr), parent(parent) {
      ++construct_count;
    }
    BinaryTreeNode(const Comparable &data) : BinaryTreeNode(data, nullptr) {}

    ~BinaryTreeNode() { --construct_count; }
    Comparable data;
    BinaryTreeNode *left;
    BinaryTreeNode *right;
    BinaryTreeNode *parent;

   public:
    /// 用于构造时自加, 析构时自减
    inline static std::atomic<int> construct_count = 0;
  };

  Tree root_;
};  // namespace binary_tree

}  // namespace binary_tree
}  // namespace data_struct
}  // namespace Lee

#endif  // end of MYALGO_INCLUDE_TOOLS_DATASTRUCT_BINARY_TREE_H_
