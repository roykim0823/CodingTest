// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <cassert>
#include <memory>

#include "BST_prototype.h"

using std::unique_ptr;

/* The key idea is to search k. However, even if we find k, after recording this we continue
   the search on the left subarray.
O(logN) time: since each iteration reduces the size of subarray being searched by half.
*/
BSTNode<int>* find_first_equal_k(const unique_ptr<BSTNode<int>>& T, int k) {
  BSTNode<int>* first = nullptr, *curr = T.get();
  while (curr) {
    if (curr->data < k) {
      curr = curr->right.get();
    } else if (curr->data > k) {
      curr = curr->left.get();
    } else {  // curr->data == k.
      // Search for the leftmost in the left subtree.
      first = curr;
      curr = curr->left.get();
    }
  }
  return first;
}

// Recursive Version
BSTNode<int>* find_first_equal_k_R(const unique_ptr<BSTNode<int>>& T, int k) {
  if (!T) {
    return nullptr;  // no match.
  } else if (T->data == k) {
    // Recursively search the left subtree for first one == k.
    auto* n = find_first_equal_k(T->left, k);
    return n ? n : T.get();
  }
  // Search left or right tree according to T->data and k.
  return find_first_equal_k(T->data < k ? T->right : T->left, k);
}

int main(int argc, char* argv[]) {
  //    3
  //  2   5
  // 1   4 6
  auto root = unique_ptr<BSTNode<int>>(new BSTNode<int>{3});
  root->left = unique_ptr<BSTNode<int>>(new BSTNode<int>{2});
  root->left->left = unique_ptr<BSTNode<int>>(new BSTNode<int>{1});
  root->right = unique_ptr<BSTNode<int>>(new BSTNode<int>{5});
  root->right->left = unique_ptr<BSTNode<int>>(new BSTNode<int>{4});
  root->right->right = unique_ptr<BSTNode<int>>(new BSTNode<int>{6});
  assert(!find_first_equal_k(root, 7));
  assert(find_first_equal_k(root, 6)->data == 6);
  return 0;
}
