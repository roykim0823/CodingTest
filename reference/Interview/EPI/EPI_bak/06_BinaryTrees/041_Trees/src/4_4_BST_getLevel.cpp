// Get a linked list of all the nodes at each depth (in BST.h)
#include "BST.h"
#include "4_1.h"

using namespace std;

int main(){
    
	// Problem 1
	int a[] = { 5, 3, 8, 1, 2, 7, 10, 0, 6, 9, 11, 12 };
	bstree tree1;
	for(int i=0; i<12; ++i)
        tree1.insert(a[i]);
	cout << tree1.isBalanced() << endl;
	cout << tree1.getHeight() << endl;
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
