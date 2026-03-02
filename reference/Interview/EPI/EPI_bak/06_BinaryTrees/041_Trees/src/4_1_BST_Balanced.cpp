// 1. Implement a function to check if a binary tree is balanced.
// Balanced tree: the height of the two subtrees of any node never differ by more than one.
#include "BST.h"
#include "4_1.h"

using namespace std;

int main(){
    
	// Problem 1
	int a[] = { 5, 3, 8, 1, 2, 7, 10, 0, 6, 9, 11, 12 }, n=12;
	//int a[] = { 5, 3, 8, 1, 2, 7, 10, 0, 6, 9, 11}, n=11;
	//int a[] = { 1, 2, 3, 4}, n=4;
	//int a[] = { 1,2,3 }, n=3;
	bstree tree1;
	for(int i=0; i<n; ++i)
        tree1.insert(a[i]);
	cout << "Balanced? " << tree1.isBalanced() << endl;
	cout << "Height= " << tree1.getHeight() << endl;
	vector<list<node*> > temp = tree1.getLevelList();
	printLevelList(temp);
	cout << "Preorder ";
	tree1.preorder();
	cout << endl;
	cout << "Inorder ";
	tree1.inorder();
	cout << endl;
	cout << "Postorder ";
	tree1.postorder();
	cout << endl;
	
	

    return 0;
}
