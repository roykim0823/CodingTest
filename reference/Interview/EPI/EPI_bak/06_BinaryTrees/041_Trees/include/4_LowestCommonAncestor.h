// Find common ancestor of two nodes in a tree (Lowest Common Ancestor)
#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename T>
class BinaryTree {
	public:
	T data;
	BinaryTree<T> *left, *right;
};

template <typename T>
BinaryTree<T>* LCA(const BinaryTree<T> *n, 
	const BinaryTree<T> *a, const BinaryTree<T> *b)
{
	if(!n) {	// empty subtree
		return NULL;
	} else if (n == a || n == b) {	// if a or b is root, then the root is LCA
		return n;
	}

	BinaryTree<T> *l_res = LCA(n->left, a, b);
	BinaryTree<T> *r_res = LCA(n->right, a, b);

	if(l_res && r_res) {
		return n;	// found a and b in different subtree
	} else {
		// if both of  a and b lies in L (or R), 
		// then their LCA is in L (or R)
		return l_res ? l_res : r_res;	
	}
}

