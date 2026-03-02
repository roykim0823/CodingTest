// Check if a binary tree is a binary search tree
#include "BST.h"

using namespace std;

// Version1: Using the property of Inorder traverse -> a sorted array (ascedning order)
void bstree::checkBST_v1(node *t, vector<int> &sorted)
{
	if (t) {
		checkBST_v1(t->left, sorted);
		sorted.push_back(t->key_value); 	// in-order traverse
		checkBST_v1(t->right, sorted);
	} else
		return;
}
	
bool bstree::checkBST_v1() { 
	vector<int> sorted;
	checkBST_v1(root, sorted);

    for(size_t i=1; i<sorted.size(); i++) {
		if(sorted[i] < sorted[i-1]) return false;
	}
	sorted.clear();
	return true;
}

// Version2: Using the property of Inorder traverse without the vector
bool bstree::checkBST_v2(node *t, int &last_item)
{
	if (t) {
		checkBST_v2(t->left, last_item);
		if(t->key_value<last_item) 
		{
			//cout << t->key_value << " " << last_item << endl;
			return false;
		}
		//cout << "T " <<  t->key_value << " " << last_item << endl;
		last_item=t->key_value;
		//if(!checkBST_v2(t->right)) return false;
		checkBST_v2(t->right, last_item);
		return true;
	} else
		return true;
}
	
bool bstree::checkBST_v2() { 
	int last_item = numeric_limits<int>::min();
	return checkBST_v2(root, last_item); 
}

// Version3: Min/Max Solution
bool bstree::checkBST_v3() {
	return checkBST_v3(root, numeric_limits<int>::min(), numeric_limits<int>::max());
}

bool bstree::checkBST_v3(node* n, int min, int max) {
	if(n==NULL)
		return true;
	
	if(n->key_value<min || n->key_value>max) { // allow duplication
		return false;
	}

	if(!checkBST_v3(n->left, min, n->key_value) || !checkBST_v3(n->right, n->key_value, max)) {
		return false;
	}

	return true;
}
