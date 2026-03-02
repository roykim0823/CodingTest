// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <iostream>
#include <memory>

#include "./Binary_tree_prototype.h"

using std::cout;
using std::endl;
using std::unique_ptr;

// @include
template <typename T>
void inorder_traversal(const unique_ptr<BinaryTreeNode<T>>& root) {
  auto* n = root.get();
  while (n) {
    if (n->left.get()) {
      // Find the predecessor of n.
      auto* pre = n->left.get();
      while (pre->right.get() && pre->right.get() != n) {
        pre = pre->right.get();
      }

      // Process the successor link.
      if (pre->right.get()) {  // pre->right.get() == n.
        // Revert the successor link if predecessor's successor is n.
        pre->right.release();
        cout << n->data << endl;
        n = n->right.get();
      } else {  // if predecessor's successor is not n.
        pre->right.reset(n);
        n = n->left.get();
      }
    } else {
      cout << n->data << endl;
      n = n->right.get();
    }
  }
}
// @exclude

int main(int argc, char* argv[]) {
  //      3
  //    2   5
  //  1    4 6
  unique_ptr<BinaryTreeNode<int>> root =
      unique_ptr<BinaryTreeNode<int>>(new BinaryTreeNode<int>{3});
  root->left = unique_ptr<BinaryTreeNode<int>>(new BinaryTreeNode<int>{2});
  root->left->left = unique_ptr<BinaryTreeNode<int>>(new BinaryTreeNode<int>{1});
  root->right = unique_ptr<BinaryTreeNode<int>>(new BinaryTreeNode<int>{5});
  root->right->left = unique_ptr<BinaryTreeNode<int>>(new BinaryTreeNode<int>{4});
  root->right->right = unique_ptr<BinaryTreeNode<int>>(new BinaryTreeNode<int>{6});
  // should output 1 2 3 4 5 6
  inorder_traversal<int>(root);
  return 0;
}
