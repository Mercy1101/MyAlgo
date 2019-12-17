///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019, FRITT Inc. All rights reserved.
///
/// @file   binary_tree.h
/// @brief  二叉树的实现
///
/// @author lijiancong, pipinstall@163.com
/// @date   2019-12-09 19:56:43
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_DATASTRUCT_BINARY_TREE_H_
#define MYALGO_INCLUDE_TOOLS_DATASTRUCT_BINARY_TREE_H_

#include <utility/detail/dbg.h>
#include <algorithm>  // for std::max
#include <atomic> 

namespace Lee {
inline namespace data_struct {
inline namespace binary_tree {
/// 每个树的结点
template <typename T>
struct BinaryTreeNode {
  BinaryTreeNode(const T &data) : data(data), left(nullptr), right(nullptr) {
    dbg("construct in %d", this);
    ++construct_count;
  }
  ~BinaryTreeNode() {
    dbg("desconstruct in %d", this);
    --construct_count;
  }
  T data;
  BinaryTreeNode *left;
  BinaryTreeNode *right;

 public:
  /// 用于构造时自加, 析构时自减
  inline static std::atomic<int> construct_count = 0;
};

/// 二叉树的实现
template <typename T>
class Binary_Tree {
 public:
  /// 用于构造时自加, 析构时自减
  inline static std::atomic<int> construct_count = 0;

 public:
  Binary_Tree(const T &data) : root_(new BinaryTreeNode<T>(data)) {
    ++construct_count;
  }
  ~Binary_Tree() {
    Destory<T>(root_);
    --construct_count;
  }

  /// @name     Insert
  /// @brief    插入一个节点到树中，其中的插入的位置依靠入参的值排序所得,
  ///           元素重复则不插入
  ///
  /// @param    element [in]  要插入的数据
  ///
  /// @return   NONE
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-10 09:05:02
  /// @warning  线程不安全
  void Insert(const T &data) {
    BinaryTreeNode<T> *data_ptr = root_;
    BinaryTreeNode<T> *last_data_ptr = data_ptr;
    while (data_ptr != nullptr) {
      if (data < data_ptr->data) {
        last_data_ptr = data_ptr;
        data_ptr = data_ptr->left;
      } else if (data > data_ptr->data) {
        last_data_ptr = data_ptr;
        data_ptr = data_ptr->right;
      } else {
        /// 相等的情况暂时什么都不做
        return;
      }
    }

    if (data < last_data_ptr->data) {
      last_data_ptr->left = new BinaryTreeNode<T>(data);
    } else if (last_data_ptr->data < data) {
      last_data_ptr->right = new BinaryTreeNode<T>(data);
    } else {  /// 相等的情况暂时什么都不做
      return;
    }
  }

  void Delete() {}
  void Update() {}
  void Find() {}
  /// @name     Size
  /// @brief    获取本实例的所有元素个数
  ///
  /// @param    node  [in]
  ///
  /// @return   本实例的元素个数
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-17 14:10:14
  /// @warning  线程不安全
  int Size() { return Size(root_); }
  int Size(const BinaryTreeNode<T> *node) {
    if (node == nullptr) {
      return 0;
    } else {
      return 1 + Size(node->left) + Size(node->right);
    }
  }
  /// @name     GetRootNode
  /// @brief    获取根节点指针
  ///
  /// @param    NONE
  ///
  /// @return   根结点指针
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-17 11:52:17
  /// @warning  线程不安全
  BinaryTreeNode<T> *GetRootNode() const { return root_; }
  /// @name     GetHeight
  /// @brief    获取一个树形数据结构的深度
  ///
  /// @param    tree
  ///
  /// @return
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-17 11:48:47
  /// @warning  线程不安全
  int GetHeight(BinaryTreeNode<T> *node) const {
#undef max
    if (node == nullptr) {
      return -1;
    } else {
      return 1 + std::max(GetHeight(node->left), GetHeight(node->right));
    }
  }
  int GetHeight() const { return GetHeight(root_); }

 private:
  BinaryTreeNode<T> *root_;  ///< 根节点
  /// @name     Destory
  /// @brief    删除该节点以及该节点以下的所有节点,
  ///           递归结构，容易爆栈，使用时应注意。
  ///
  /// @param    data_ptr  要删除的节点
  ///
  /// @return   NONE
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-10 08:50:33
  /// @warning  线程不安全
  template <typename T>
  void Destory(const BinaryTreeNode<T> *data_ptr) {
    if (data_ptr != nullptr) {
      Destory(data_ptr->left);
      Destory(data_ptr->right);
      delete data_ptr;
    }
  }
};  // namespace binary_tree

}  // namespace binary_tree
}  // namespace data_struct
}  // namespace Lee

#endif  // end of MYALGO_INCLUDE_TOOLS_DATASTRUCT_BINARY_TREE_H_
