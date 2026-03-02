// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <cassert>
#include <iostream>
#include <memory>
#include <stack>
#include <vector>

#include "./BST_prototype.h"

using std::cout;
using std::endl;
using std::stack;
using std::unique_ptr;
using std::vector;

vector<int> result;


// Inorder traverse, Trivial Recursive Version
void print_BST_in_sorted_orderR(const unique_ptr<BSTNode<int>>& n) {
	if(n) {
		print_BST_in_sorted_orderR(n->left);
      	cout << n->data << endl;
		print_BST_in_sorted_orderR(n->right);
	}
}


// Inorder traverse, Iterative Version with no visited field
void print_BST_in_sorted_order(const unique_ptr<BSTNode<int>>& n) {
  stack<const BSTNode<int>*> s;
  const BSTNode<int>* curr = n.get();

  while (!s.empty() || curr) {
    if (curr) {
      s.push(curr);
      curr = curr->left.get();
    } else {
      curr = s.top();
      s.pop();
      cout << curr->data << endl;
      
	  result.emplace_back(curr->data);
      curr = curr->right.get();
    }
  }
}

int main(int argc, char* argv[]) {
  //      3
  //    2   5
  //  1    4 6
  unique_ptr<BSTNode<int>> root =
      unique_ptr<BSTNode<int>>(new BSTNode<int>{3, nullptr});
  root->left =
      unique_ptr<BSTNode<int>>(new BSTNode<int>{2, nullptr});
  root->left->left =
      unique_ptr<BSTNode<int>>(new BSTNode<int>{1, nullptr});
  root->right =
      unique_ptr<BSTNode<int>>(new BSTNode<int>{5, nullptr});
  root->right->left =
      unique_ptr<BSTNode<int>>(new BSTNode<int>{4, nullptr});
  root->right->right =
      unique_ptr<BSTNode<int>>(new BSTNode<int>{6, nullptr});
  // should output 1 2 3 4 5 6
  print_BST_in_sorted_order(root);
  print_BST_in_sorted_orderR(root);
  vector<int> golden_res = {1, 2, 3, 4, 5, 6};
  assert(golden_res.size() == result.size());
  assert(equal(golden_res.begin(), golden_res.end(), result.begin()));
  return 0;
}
