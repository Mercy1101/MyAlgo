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
/// 每个树的结点
template <typename T>
struct BinaryTreeNode {
  BinaryTreeNode(const T &data) : data(data), left(nullptr), right(nullptr) {
    // dbg("construct in %d", this);
    ++construct_count;
  }
  ~BinaryTreeNode() {
    // dbg("desconstruct in %d", this);
    --construct_count;
  }
  T data;
  BinaryTreeNode *left;
  BinaryTreeNode *right;

 public:
  /// 用于构造时自加, 析构时自减
  inline static std::atomic<int> construct_count = 0;
};

/// @name     Binary_Tree
/// @brief    二叉树的实现
/// @details  设计的思想是, 数值大的放右边,数值小的放左边,相等的值不会重复插入
///
/// @author   Lijiancong, pipinstall@163.com
/// @date     2019-12-18 09:36:23
/// @warning  线程不安全
template <typename T>
class Binary_Tree {
 public:
  /// 用于构造时自加, 析构时自减
  inline static std::atomic<int> construct_count = 0;

 public:
  Binary_Tree(const T &data) : root_(new BinaryTreeNode<T>(data)) {
    ++construct_count;
  }
  Binary_Tree(const std::initializer_list<T> &init_list) {
    ++construct_count;
    root_ = new BinaryTreeNode<T>(*init_list.begin());
    for (const auto &it : init_list) {
      this->Insert(it);
    }
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
    } else {
      /// 相等的情况暂时什么都不做
      return;
    }
  }
  /// @name     FindMax
  /// @brief    寻找最右节点, 即数值最大的节点
  ///
  /// @param    node_ptr  [in]  从哪个节点开始找
  ///
  /// @return   寻找最右节点, 即数值最大的节点的地址
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-18 10:55:33
  /// @warning  线程不安全
  BinaryTreeNode<T> *FindMax(BinaryTreeNode<T> *node_ptr) {
    if (nullptr != node_ptr) {
      while (nullptr != node_ptr->right) {
        node_ptr = node_ptr->right;
      }
    }
    return node_ptr;
  }
  BinaryTreeNode<T> *FindMax() { FindMax(root_); }

  /// @name     Delete
  /// @brief    删除某个特定节点
  ///
  /// @param    data  [in]
  ///
  /// @return   删除成功与否
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-18 08:48:47
  /// @warning  线程不安全
  bool Delete(const T &data) {
    auto node_ptr = Find(data);
    if (node_ptr != nullptr) {
      if (nullptr != node_ptr->left && nullptr != node_ptr->right) {
        /// 双子树的节点的删除, 使用该节点的最右节点来代替本节点
        auto max_node = FindMax(node_ptr);
        auto pre_max_node = Find_Previously(max_node);
        auto pre_delete_node_ptr = Find_Previously(node_ptr);
        if (data > pre_delete_node_ptr->data) {
          pre_delete_node_ptr->right = pre_max_node->right;
        } else if (data < pre_delete_node_ptr->data) {
          pre_delete_node_ptr->left = pre_max_node->right;
        }
        max_node->right = node_ptr->right;
        if (nullptr == max_node->left) {
          max_node->left = node_ptr->left;
        } else {
          /// TODO(lijiancong) 这里写的很丑陋, 回头要重写
          /// 当找寻到的最右节点有左子树时把该左子树完全取出,再重新插入
          std::vector<T> data_vector;
          GetData(max_node->left, &data_vector);
          for (const auto &it : data_vector) {
            this->Insert(it);
          }
        }
        delete node_ptr;
        pre_max_node->right = nullptr;
      } else if (nullptr == node_ptr->left && nullptr == node_ptr->right) {
        /// 树叶节点的删除
        auto pre_delete_node_ptr = Find_Previously(node_ptr);
        if (data > pre_delete_node_ptr->data) {
          node_ptr = pre_delete_node_ptr->right;
          pre_delete_node_ptr->right = nullptr;
        } else if (data < pre_delete_node_ptr->data) {
          node_ptr = pre_delete_node_ptr->left;
          pre_delete_node_ptr->left = nullptr;
        } else {
          /// 相等情况不考虑
        }
        delete node_ptr;
      } else {
        /// 单子树删除
        auto pre_delete_node_ptr = Find_Previously(node_ptr);
        if (data > pre_delete_node_ptr->data) {
          auto data_ptr = pre_delete_node_ptr->right;
          pre_delete_node_ptr->right = Find(data);
          delete data_ptr;
        } else if (data < pre_delete_node_ptr->data) {
          auto data_ptr = pre_delete_node_ptr->left;
          pre_delete_node_ptr->left = Find(data);
          delete data_ptr;
        } else {
          /// 相等情况不考虑
        }
      }
    } else {
      /// 没找到该节点就直接返回成功
      return true;
    }
    return true;
  }

  /// @name     Find
  /// @brief    查询某个数值在不在本二叉树内
  ///
  /// @param    data  [in]
  ///
  /// @return   返回指向拥有这个值的节点的地址,找不到则返回nullptr
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-18 08:59:05
  /// @warning  线程不安全
  BinaryTreeNode<T> *Find(const T &data) const {
    auto node_ptr = GetRootNode();
    while (node_ptr != nullptr) {
      if (data == node_ptr->data) {
        return node_ptr;
      } else if (data > node_ptr->data) {
        node_ptr = node_ptr->right;
      } else if (data < node_ptr->data) {
        node_ptr = node_ptr->left;
      }
    }
    return nullptr;
  }

  /// @name     Size
  /// @brief    获取一棵树的所有元素个数
  ///
  /// @param    node  [in]
  ///
  /// @return   本实例的元素个数
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-17 14:10:14
  /// @warning  线程不安全
  int Size() const { return Size(root_); }
  int Size(const BinaryTreeNode<T> *node) const {
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

  /// @name     GetData
  /// @brief    获取所有节点的data, 并放入容器中
  ///
  /// @param    node          [in]
  /// @param    data_vector   [out]
  ///
  /// @return   NONE
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-18 13:25:28
  /// @warning  线程不安全
  void GetData(BinaryTreeNode<T> *node, std::vector<T> *data_vector) {
    while (nullptr != node) {
      data_vector->emplace_back(node->data);
      GetData(node->left, data_vector);
      GetData(node->right, data_vector);
    }
  }
  void GetData(std::vector<T> *data_vector) { GetData(root_, data_vector); }

  void Update() {}

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

  /// @name     Find_Previously
  /// @brief    查询指向某个数值节点的上一个节点的指针
  ///
  /// @param    data  [in]
  ///
  /// @return   有这个节点,则返回这个节点的上一个节点的指针,找不到则返回nullptr
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-18 08:59:05
  /// @warning  线程不安全
  BinaryTreeNode<T> *Find_Previously(const T &data) const {
    auto node_ptr = GetRootNode();
    auto last_node_ptr = node_ptr;
    while (node_ptr != nullptr) {
      if (data == node_ptr->data) {
        return last_node_ptr;
      } else if (data > node_ptr->data) {
        last_node_ptr = node_ptr;
        node_ptr = node_ptr->right;
      } else if (data < node_ptr->data) {
        last_node_ptr = node_ptr;
        node_ptr = node_ptr->left;
      }
    }
    return nullptr;
  }
  BinaryTreeNode<T> *Find_Previously(BinaryTreeNode<T> *node) const {
    return Find_Previously(node->data);
  }
};  // namespace binary_tree

}  // namespace binary_tree
}  // namespace data_struct
}  // namespace Lee

#endif  // end of MYALGO_INCLUDE_TOOLS_DATASTRUCT_BINARY_TREE_H_
