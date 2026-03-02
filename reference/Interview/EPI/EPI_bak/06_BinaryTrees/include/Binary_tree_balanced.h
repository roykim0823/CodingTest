// Q. Implement a function to check if a binary tree is balanced.
//    Balanced tree: the height of the two subtrees of any node never differ by more than one.

// Naive recursive version: Recursive through the entire subtree -> O(n^2)
/*
bool isBalanced() {
	if (root==NULL) return true;	// Base case
	
	int heightDiff = getHeight(root.left) - getHeight(root.right);
	if(heightDiff>1 or heightDiff<-1) {
		return false
	} else { 	// Recursive
		return isBalanced(root.left) && isBalanced(root.right);
	}
}
*/

#include "Binary_tree_prototype.h"
#include <cmath>
//#include <algorithm>

template <typename T>
int get_height(const unique_ptr<BinaryTreeNode<T>> &n);

template <typename T>
bool is_balanced_binary_tree(const unique_ptr<BinaryTreeNode<T>> &n) {
	return get_height(n) != -2;
}

// Problem1: O(N), O(logN) space
template <typename T>
int get_height(const unique_ptr<BinaryTreeNode<T>>  &node){
	if(node==nullptr) return -1;	// Height of 0
	
	// Check if left is balanaced
	int leftHeight = get_height(node->left);
	if(leftHeight == -2)
		return -2;	// Not balanced

	// Check if right is balanaced
	int rightHeight = get_height(node->right);
	if(rightHeight == -2)
		return -2;	// Not balanced
	
	// check if current node is balanced
	int heightDiff = leftHeight - rightHeight;
	if(abs(heightDiff) > 1)
		return -2;	// Not balanced
	else
		return std::max(leftHeight, rightHeight) +1;
}
