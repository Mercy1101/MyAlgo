///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
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

namespace lee {
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
  Position FindMin() { return FindMin(root_); }
  void Delete(const Comparable &data) { Delete(data, root_); };

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
  /// @param    NONE
  ///
  /// @return   返回最小节点的位置
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-29 18:14:11
  /// @warning  线程不安全
  Position FindMin(const Tree tree) {
    if (nullptr == tree) {
      return nullptr;
    }
    auto node_ptr = tree;
    while (node_ptr->left != nullptr) {
      node_ptr = node_ptr->left;
    }
    return node_ptr;
  }

  /// @name     Delete
  /// @brief    删除某特定元素
  ///
  /// @param    data  [in]  要删除的特定元素
  /// @param    tree  [out] 要修改的树节点指针
  ///
  /// @return   NONE
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-29 18:11:41
  /// @warning  线程不安全
  void Delete(const Comparable &x, Tree *t) {
    if (t == nullptr) return;  // Item not found; do nothing
    if (x < t->element)
      Delete(x, t->left);
    else if (t->element < x)
      Delete(x, t->right);
    else if (t->left != nullptr && t->right != nullptr)  // Two children
    {
      t->element = findMin(t->right)->element;
      Delete(t->element, t->right);
    } else {
      BinaryNode *oldNode = t;
      t = (t->left != nullptr) ? t->left : t->right;
      delete oldNode;
    }
  }

  /// @name     Transplant
  /// @brief    把一颗子树挪到另一个节点下面
  ///
  /// @param    tree      [in]
  /// @param    node_from [in]
  /// @param    node_to   [in]
  ///
  /// @return   NONE
  ///
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2019-12-31 11:09:41
  /// @warning  线程不安全
  void Transplant(Tree tree, BinaryTreeNode *node_from,
                  BinaryTreeNode *node_to) {
    if (nullptr == node_from->parent) {
      Destory(root_);
      tree.root_ = node_to;
    } else if (node_from == node_from->parent->left) {
      Destroy(node_from->parent->left);
      node_from->parent->left = node_to;
    } else {
      node_from->parent->right = node_to;
    }
    if (nullptr == node_to) {
      node_to->parent = node_from->parent;
    }
  }

 private:
  Tree root_;
};

template <typename T>
class tree {
 private:
  struct tree_node {
    tree_node *left;
    tree_node *right;
    T val;
    tree_node(const T &element) : left(nullptr), right(nullptr), val(element) {}
  };
  tree_node *root_;

 public:
  tree(const T &element) {
    root_ = new tree_node(element);
    root_->left = nullptr;
    root_->right = nullptr;
  }

  ~tree() {
    if (root_ != nullptr) {
      delete_tree(root_->left);
    }
  }

  tree(const tree &other) = delete;
  tree(const tree &&other) = delete;
  tree operator=(const tree &x) = delete;
  tree operator=(tree &&x) = delete;

  delete_tree(tree_node *node) {
    if (node == nullptr) {
      return;
    }
    delete_tree(node->left);
    delete_tree(node->right);
    delete node;
  }

  void insert(const T &element) { insert_(element, root_); }

  void remove(const T &element) { remove_(element, root_); }
  tree_node *find_min(const T &element) { return find_min_(element, root_); }

 private:
  void insert_(const T &element, tree_node *tree) {
    if (tree == nullptr) {
      tree = new tree_node(element);
    } else {
      if (element > tree->val) {
        insert_(element, tree->right);
      } else if (element < tree->val) {
        insert_(element, tree->left);
      } else {
        /// Do Nothing
      }
    }
  }

  void remove_(const T &element, tree_node *node) {
    if (node == nullptr) {
      return;  /// if not find do nothing
    } else {
      if (element > node->val) {
        remove_(element, node->right);
      } else if (element < node->val) {
        remove_(element, node->left);
      } else {
        /// finded it
        if (node->left != nullptr && node->right != nullptr) {
          t->val = find_min_(node->right)->val;
          remove_(t->val, t->right);
        } else {
          auto temp = node;
          node = node->left == nullptr ? node->right : node->left;
          delete temp;
        }
      }
    }
  }

  tree_node *find_min_(tree_node *node) {
    if (node == nullptr) {
      return nullptr;
    }
    if (node->left == nullptr) {
      return node;
    } else {
      return find_min_(node);
    }
  }

  bool contain_(const T &element, tree_node *node) {
    if (node == nullptr) {
      return false;
    } else if (element > node->val) {
      contain_(element, node->right);
    } else if (element < node->val) {
      contain_(element, node->left);
    } else {
      return true;
    }
  }
};

}  // namespace binary_tree
}  // namespace data_struct
}  // namespace lee

#endif  // end of MYALGO_INCLUDE_TOOLS_DATASTRUCT_BINARY_TREE_H_
