///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019, FRITT Inc. All rights reserved.
///
/// @file   binary_tree_sample.h
/// @brief  二叉树示例，
///         从<Data Structures And Algorithm Analysis In Cpp>书中抄来的
///
/// @author lijiancong, pipinstall@163.com
/// @date   2019-12-21 19:42:22
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_DATASTRUCT_BINARY_TREE_SAMPLE_H_
#define MYALGO_INCLUDE_TOOLS_DATASTRUCT_BINARY_TREE_SAMPLE_H_

#include <atomic>
#include <initializer_list>
#include <iostream>

#include "utility/detail/dbg.h"

namespace Lee {
inline namespace data_struct {
inline namespace binary_tree {

template <typename Comparable>
class BinarySearchTree {
 public:
  BinarySearchTree() : root{nullptr} {}
  BinarySearchTree(const std::initializer_list<Comparable> &data_list)
      : root{nullptr} {
    for (const int &it : data_list) {
      this->insert(it);
    }
  }
  /**
   * Copy constructor
   */
  BinarySearchTree(const BinarySearchTree &rhs) : root{nullptr} {
    root = clone(rhs.root);
  }
  BinarySearchTree(BinarySearchTree &&rhs) : root{nullptr} {
    root = clone(rhs.root);
  }

  ~BinarySearchTree() {
    makeEmpty();
    dbg((BinaryNode::node_count == 0));
  }

  const Comparable &findMin() const { findMin(root); }
  const Comparable &findMax() const { findMax(root); }
  /**
   * Returns true if x is found in the tree.
   */
  bool contains(const Comparable &x) const { return contains(x, root); }

  bool isEmpty() const { return nullptr == root; };
  void printTree(std::ostream &out = std::cout) const { printTree(root, out); }

  void makeEmpty() { makeEmpty(root); }
  /**
   * Insert x into the tree; duplicates are ignored.
   */
  void insert(const Comparable &x) { insert(x, root); }

  void insert(Comparable &&x) { insert(x, root); }
  /**
   * Remove x from the tree. Nothing is done if x is not found.
   */
  void remove(const Comparable &x) { remove(x, root); }

  BinarySearchTree &operator=(const BinarySearchTree &rhs) {
    /// 假设这里的入参是自己则什么也不做
    if (this == &rhs) {
      return *this;
    }
    this->makeEmpty();
    this->clone(rhs.root);
  }
  BinarySearchTree &operator=(BinarySearchTree &&rhs) {
    /// 假设这里的入参是自己则什么也不做
    if (this == &rhs) {
      return *this;
    }
    this->makeEmpty();
    this->clone(rhs.root);
  }

 private:
  struct BinaryNode {
    Comparable element;
    BinaryNode *left;
    BinaryNode *right;

    BinaryNode(const Comparable &theElement, BinaryNode *lt, BinaryNode *rt)
        : element{theElement}, left{lt}, right{rt} {}

    BinaryNode(Comparable &&theElement, BinaryNode *lt, BinaryNode *rt)
        : element{std::move(theElement)}, left{lt}, right{rt} {}

    ~BinaryNode() { --node_count; }
    inline static std::atomic<int> node_count = 0;
  };

  BinaryNode *root;

  /**
   * Internal method to insert into a subtree.
   * x is the item to insert.
   * t is the node that roots the subtree.
   * Set the new root of the subtree.
   */
  void insert(const Comparable &x, BinaryNode *&t) {
    if (t == nullptr)
      t = new BinaryNode{x, nullptr, nullptr};
    else if (x < t->element)
      insert(x, t->left);
    else if (t->element < x)
      insert(x, t->right);
    else
      ;  // Duplicate; do nothing
  }

  /**
   * Internal method to insert into a subtree.
   * x is the item to insert by moving.
   * t is the node that roots the subtree.
   * Set the new root of the subtree.
   */
  void insert(Comparable &&x, BinaryNode *&t) {
    if (t == nullptr)
      t = new BinaryNode{std::move(x), nullptr, nullptr};
    else if (x < t->element)
      insert(std::move(x), t->left);
    else if (t->element < x)
      insert(std::move(x), t->right);
    else
      ;  // Duplicate; do nothing
  }
  /**
   * Internal method to remove from a subtree.
   * x is the item to remove.
   * t is the node that roots the subtree.
   * Set the new root of the subtree.
   */
  void remove(const Comparable &x, BinaryNode *&t) {
    if (t == nullptr) return;  // Item not found; do nothing
    if (x < t->element)
      remove(x, t->left);
    else if (t->element < x)
      remove(x, t->right);
    else if (t->left != nullptr && t->right != nullptr)  // Two children
    {
      t->element = findMin(t->right)->element;
      remove(t->element, t->right);
    } else {
      BinaryNode *oldNode = t;
      t = (t->left != nullptr) ? t->left : t->right;
      delete oldNode;
    }
  }

  /**
   * Internal method to find the smallest item in a subtree t.
   * Return node containing the smallest item.
   */
  BinaryNode *findMin(BinaryNode *t) const {
    if (t == nullptr) return nullptr;
    if (t->left == nullptr) return t;
    return findMin(t->left);
  }
  /**
   * Internal method to find the largest item in a subtree t.
   * Return node containing the largest item.
   */
  BinaryNode *findMax(BinaryNode *t) const {
    if (t != nullptr)
      while (t->right != nullptr) t = t->right;
    return t;
  }
  /**
   * Internal method to test if an item is in a subtree.
   * x is item to search for.
   * t is the node that roots the subtree.
   */
  bool contains(const Comparable &x, BinaryNode *t) const {
    if (t == nullptr)
      return false;
    else if (x < t->element)
      return contains(x, t->left);
    else if (t->element < x)
      return contains(x, t->right);
    else
      return true;  // Match
  }
  /**
   * Internal method to make subtree empty.
   */
  void makeEmpty(BinaryNode *&t) {
    if (t != nullptr) {
      makeEmpty(t->left);
      makeEmpty(t->right);
      delete t;
    }
    t = nullptr;
  }
  void printTree(BinaryNode *t, std::ostream &out) const;

  /**
   * Internal method to clone subtree.
   */
  BinaryNode *clone(BinaryNode *t) const {
    if (t == nullptr) {
      return nullptr;
    } else {
      return new BinaryNode{t->element, clone(t->left), clone(t->right)};
    }
  }
};

template <typename KeyT, typename ValueT>
class BST {
 private:
  /**
   * binary search tree definition.
   */
  struct treeNode {
    KeyT key;          // key
    ValueT value;      // data
    treeNode *parent;  // parent
    treeNode *left;    // left child
    treeNode *right;   // right child
  };

  class BSTException : public std::exception {
   public:
    virtual const char *what() const throw() { return "key does not exist"; }
  } excp_key;

 private:
  treeNode *m_root;  // the root
 private:
  BST(const BST &);
  BST &operator=(const BST &);

 public:
  BST() : m_root(NULL){};

  ~BST() { destruct_(m_root); }

  /**
   * find key
   */
  treeNode *find(const KeyT &key) {
    treeNode *n = m_root;
    while (n != NULL && key != n->key) {
      if (key < n->key) {
        n = n->left;
      } else {
        n = n->right;
      }
    }

    return n;
  }

  /**
   * insert a new data into the binary search tree.
   */
  void insert(const KeyT &key, const ValueT &value) {
    treeNode *z = new treeNode;
    z->key = key;
    z->value = value;
    z->left = z->right = z->parent = NULL;

    treeNode *n = m_root;
    treeNode *y = NULL;
    while (n != NULL) {
      y = n;
      if (key < n->key) {
        n = n->left;
      } else {
        n = n->right;
      }
    }

    z->parent = y;
    if (y == NULL) {
      m_root = z;
    } else if (key < y->key) {
      y->left = z;
    } else {
      y->right = z;
    }
  }

  /**
   * delete a key from the binary search tree.
   */
  bool deleteKey(const KeyT &key) {
    treeNode *z = find(key);
    if (z == NULL) {
      return false;
    }

    if (z->left == NULL) {
      transplant(z, z->right);
    } else if (z->right == NULL) {
      transplant(z, z->left);
    } else {
      // find the minimum element of the right subtree
      treeNode *y = minimum(z->right);
      if (y->parent != z) {
        // replace y with right-child
        transplant(y, y->right);
        // replace right-child of y with the right-child of z
        y->right = z->right;
        // make y the parent of the right-child
        y->right->parent = y;
      }

      // replace z with y
      transplant(z, y);
      y->left = z->left;
      y->left->parent = y;
    }

    delete z;
    return true;
  }

  void print_tree(treeNode *n, int indent) {
    if (n == NULL) {
      return;
    }
    print_tree(n->right, indent + 1);
    int i;
    for (i = 0; i < indent; i++) {
      printf(" ");
    }
    std::cout << "[" << n->key << "," << n->value << "]" << std::endl;
    print_tree(n->left, indent + 1);
  }

  void print_helper() { print_tree(m_root, 0); }

 private:
  void destruct_(treeNode *n) {
    if (n == NULL) return;
    destruct_(n->left);
    destruct_(n->right);
    delete n;
  }

  /**
   * replace node u with v.
   */
  void transplant(treeNode *u, treeNode *v) {
    if (u->parent == NULL) {
      m_root = v;
    } else if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }

    if (v != NULL) {
      v->parent = u->parent;
    }
  }

  /**
   * find the minimum element of the subtree
   */
  treeNode *minimum(treeNode *x) {
    while (x->left != NULL) {
      x = x->left;
    }

    return x;
  }
};

}  // namespace binary_tree
}  // namespace data_struct
}  // namespace Lee

#endif  // end of MYALGO_INCLUDE_TOOLS_DATASTRUCT_BINARY_TREE_SAMPLE_H_
