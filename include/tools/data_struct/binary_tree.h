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
 public:
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
  using Position = struct BinaryTreeNode *;
  using Tree = struct BinaryTreeNode *;

  Binary_Tree(const Comparable &data) : root_(new BinaryTreeNode(data)) {}
  Binary_Tree(const std::initializer_list<Comparable> &init_list) {
    if (0 == init_list.size()) {
      root_ = new BinaryTreeNode(0, nullptr);
      return;
    }
    root_ = new BinaryTreeNode(*init_list.begin());
    for (const auto &it : init_list) {
      this->Insert(it);
    }
  }
  ~Binary_Tree() { Destory(root_); }

  void Insert(const Comparable &data) { Insert(data, root_); }
  void Destory() { Destory(root_); }

  Tree GetRoot() const { return root_; }

 private:
  /// @name     Insert
  /// @brief    插入一个元素到树中
  ///
  /// @param    data [in]
  /// @param    tree [in]
  ///
  /// @return   NONE
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-29 13:20:11
  /// @warning  线程不安全
  void Insert(const Comparable &data, Tree tree) {
    if (nullptr == tree) {
      /// 这个树要是为空的话就不会做任何事
      return;
    }
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

    if (data > pre_node_ptr->data) {
      pre_node_ptr->right = new BinaryTreeNode(data, pre_node_ptr);
    } else if (data < pre_node_ptr->data) {
      pre_node_ptr->left = new BinaryTreeNode(data, pre_node_ptr);
    } else {
      /// 相等什么也不做
      return;
    }
    return;
  }

  /// @name     Destory
  /// @brief    删除节点释放内存
  ///
  /// @param    tree  [in]
  ///
  /// @return   NONE
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-29 14:40:02
  /// @warning  线程不安全
  void Destory(const Tree tree) const {
    if (tree != nullptr) {
      Destory(tree->left);
      Destory(tree->right);
      delete tree;
    }
    return;
  }

  /// @name     FindMin
  /// @brief    找寻树中最小的元素
  ///
  /// @param
  ///
  /// @return
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-29 18:14:11
  /// @warning  线程不安全
  Position FindMin() {}

  /// @name     Delete
  /// @brief    删除某特定元素
  ///
  /// @param
  ///
  /// @return
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-29 18:11:41
  /// @warning  线程不安全
  void Delete() {}

 private:
  Tree root_;
};  // namespace binary_tree

}  // namespace binary_tree
}  // namespace data_struct
}  // namespace Lee

#endif  // end of MYALGO_INCLUDE_TOOLS_DATASTRUCT_BINARY_TREE_H_
