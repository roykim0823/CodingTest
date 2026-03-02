// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>

#include "Binary_tree_prototype.h"
#include "Binary_tree_balanced.h"

using std::boolalpha;
using std::cout;
using std::endl;
using std::unique_ptr;

int main(int argc, char* argv[]) {
  //  balanced binary tree test
  //      3
  //    2   5
  //  1    4 6
  unique_ptr<BinaryTreeNode<int>> root =
      unique_ptr<BinaryTreeNode<int>>(new BinaryTreeNode<int>());
  root->left = unique_ptr<BinaryTreeNode<int>>(new BinaryTreeNode<int>());
  root->left->left = unique_ptr<BinaryTreeNode<int>>(new BinaryTreeNode<int>());
  root->right = unique_ptr<BinaryTreeNode<int>>(new BinaryTreeNode<int>());
  root->right->left = unique_ptr<BinaryTreeNode<int>>(new BinaryTreeNode<int>());
  root->right->right = unique_ptr<BinaryTreeNode<int>>(new BinaryTreeNode<int>());
  assert(is_balanced_binary_tree(root) == true);
  cout << boolalpha << is_balanced_binary_tree(root) << endl;
  // Non-balanced binary tree test.
  root = unique_ptr<BinaryTreeNode<int>>(new BinaryTreeNode<int>());
  root->left = unique_ptr<BinaryTreeNode<int>>(new BinaryTreeNode<int>());
  root->left->left = unique_ptr<BinaryTreeNode<int>>(new BinaryTreeNode<int>());
  assert(is_balanced_binary_tree(root) == false);
  cout << boolalpha << is_balanced_binary_tree(root) << endl;
  return 0;
}
