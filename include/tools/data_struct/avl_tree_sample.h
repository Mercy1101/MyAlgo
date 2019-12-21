///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019, FRITT Inc. All rights reserved.
///
/// @file   avl_tree_sample.h
/// @brief  AVL树示例，
///         从<Data Structures And Algorithm Analysis In Cpp>书中抄来的
///
/// @author lijiancong, pipinstall@163.com
/// @date   2019-12-21 19:42:22
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_DATASTRUCT_AVL_TREE_SAMPLE_H_
#define MYALGO_INCLUDE_TOOLS_DATASTRUCT_AVL_TREE_SAMPLE_H_

#include <algorithm>

template <typename Comparable>
struct AvlNode {
  Comparable element;
  AvlNode *left;
  AvlNode *right;
  int height;

  AvlNode(const Comparable &ele, AvlNode *lt, AvlNode *rt, int h = 0)
      : element{ele}, left{lt}, right{rt}, height{h} {}

  AvlNode(Comparable &&ele, AvlNode *lt, AvlNode *rt, int h = 0)
      : element{std::move(ele)}, left{lt}, right{rt}, height{h} {}
};

/**
 * Return the height of node t or -1 if nullptr.
 */
int height(AvlNode *t) const { return t == nullptr ? -1 : t->height; }

/**
 * Internal method to insert into a subtree.
 * x is the item to insert.
 * t is the node that roots the subtree.
 * Set the new root of the subtree.
 */
void insert(const Comparable &x, AvlNode *&t) {
  if (t == nullptr)
    t = new AvlNode{x, nullptr, nullptr};
  else if (x < t->element)
    insert(x, t->left);
  else if (t->element < x)
    insert(x, t->right);

  balance(t);
}

static const int ALLOWED_IMBALANCE = 1;

// Assume t is balanced or within one of being balanced
void balance(AvlNode *&t) {
  if (t == nullptr) return;

  if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE)
    if (height(t->left->left) >= height(t->left->right))
      rotateWithLeftChild(t);
    else
      doubleWithLeftChild(t);
  else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE)
    if (height(t->right->right) >= height(t->right->left))
      rotateWithRightChild(t);
    else
      doubleWithRightChild(t);

  t->height = max(height(t->left), height(t->right)) + 1;
}

/**
 * Rotate binary tree node with left child.
 * For AVL trees, this is a single rotation for case 1.
 * Update heights, then set new root.
 */
void rotateWithLeftChild(AvlNode *&k2) {
  AvlNode *k1 = k2->left;
  k2->left = k1->right;
  k1->right = k2;
  k2->height = std::max(height(k2->left), height(k2->right)) + 1;
  k1->height = std::max(height(k1->left), k2->height) + 1;
  k2 = k1;
}

/**
 * Double rotate binary tree node: first left child
 * with its right child; then node k3 with new left child.
 * For AVL trees, this is a double rotation for case 2.
 * Update heights, then set new root.
 */
void doubleWithLeftChild(AvlNode *&k3) {
  rotateWithRightChild(k3->left);
  rotateWithLeftChild(k3);
}

/**
 * Internal method to remove from a subtree.
 * x is the item to remove.
 * t is the node that roots the subtree.
 * Set the new root of the subtree.
 */
void remove(const Comparable &x, AvlNode *&t) {
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
    AvlNode *oldNode = t;
    t = (t->left != nullptr) ? t->left : t->right;
    delete oldNode;
  }

  balance(t);
}

/**
 * Print the tree contents in sorted order.
 */
void printTree(ostream &out = cout) const {
  if (isEmpty())
    out << "Empty tree" << endl;
  else
    printTree(root, out);
}

/**
 * Internal method to print a subtree rooted at t in sorted order.
 */
void printTree(BinaryNode *t, ostream &out) const {
  if (t != nullptr) {
    printTree(t->left, out);
    out << t->element << endl;
    printTree(t->right, out);
  }
}

/**
 * Internal method to compute the height of a subtree rooted at t.
 */
int height(BinaryNode *t) {
  if (t == nullptr)
    return -1;
  else
    return 1 + max(height(t->left), height(t->right));
}
#endif  /// end of MYALGO_INCLUDE_TOOLS_DATASTRUCT_AVL_TREE_SAMPLE_H_