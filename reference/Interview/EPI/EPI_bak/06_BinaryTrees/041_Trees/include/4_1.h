// Q. Implement a function to check if a binary tree is balanced.
//    Balanced tree: the height of the two subtrees of any node never differ by more than one.

// Naive recursive version: Recursive through the entire subtree -> O(n^2)
/*
bool bstree::isBalanced() {
	if (root==NULL) return true;	// Base case
	
	int heightDiff = getHeight(root.left) - getHeight(root.right);
	if(heightDiff>1 or heightDiff<-1) {
		return false
	} else { 	// Recursive
		return isBalanced(root.left) && isBalanced(root.right);
	}
}
*/

#include "BST.h"

using namespace std;

// Problem1: O(N), O(logN) space
int bstree::checkHeight(node *leaf){
	if(leaf == NULL) return 0;	// Height of 0
	
	// Check if left is balanaced
	int leftHeight = checkHeight(leaf->left);
	if(leftHeight == -1)
		return -1;	// Not balanced

	// Check if right is balanaced
	int rightHeight = checkHeight(leaf->right);
	if(rightHeight == -1)
		return -1;	// Not balanced
	
	// check if current node is balanced
	int heightDiff = leftHeight - rightHeight;
	if(abs(heightDiff) > 1)
		return -1;	// Not balanced
	else
		return max(leftHeight, rightHeight) +1;
}
	
bool bstree::isBalanced(){
	if(checkHeight(root) == -1)
		return false;
	else
		return true;
}
