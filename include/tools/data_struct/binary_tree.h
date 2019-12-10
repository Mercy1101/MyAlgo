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

namespace Lee {
inline namespace data_struct {
inline namespace binary_tree {
/// 每个树的结点
template <typename T>
struct BinaryTreeNode {
  BinaryTreeNode(T element) : data(element), left(nullptr), right(nullptr) {
    std::cout << "construct in " << this << "\n";
  }
  ~BinaryTreeNode() { std::cout << "desconstruct in " << this << "\n"; }
  T data;
  BinaryTreeNode *left;
  BinaryTreeNode *right;
};

/// 二叉树的实现
template <typename T>
class Binary_Tree {
  Binary_Tree(const T &element) { root_ = new BinaryTreeNode(element); }
  ~Binary_Tree() { Destory(root_); }

  /// @name     Insert
  /// @brief    插入一个节点到树中，其中的插入的位置依靠入参的值排序所得
  ///
  /// @param    element [in]  要插入的数据
  ///
  /// @return   NONE
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-10 09:05:02
  /// @warning  线程不安全
  void Insert(const T &element) {
    BinaryTreeNode *data_ptr = root_;
    while (data_ptr != nullptr) {
      if (element < root_->data) {
        data_ptr = data_ptr->left;
      } else if (element == root_->data) {
        /// 相等的情况先放左边
        data_ptr = data_ptr->left;
      } else {
        data_ptr = data_ptr->right;
      }
    }
    data_ptr = new BinaryTreeNode(element);
  }
  void Delete();
  void Update();
  void Find();

 private:
  BinaryTreeNode *root_;  ///< 根节点
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
  void Destory(const BinaryTreeNode *data_ptr) {
    if (data_ptr != nullptr) {
      Destory(data_ptr->left);
      Destory(data_ptr->right);
      delete data_ptr;
    }
  }
};

}  // namespace binary_tree
}  // namespace data_struct
}  // namespace Lee

#endif  // end of MYALGO_INCLUDE_TOOLS_DATASTRUCT_BINARY_TREE_H_
