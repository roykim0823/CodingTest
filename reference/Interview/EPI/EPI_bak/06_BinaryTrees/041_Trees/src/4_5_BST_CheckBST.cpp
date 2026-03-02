#include "BST.h"
#include "4_1.h"
#include "4_3.h"
#include "4_5.h"

int main(){
    
	int a[] = { 5, 3, 8, 1, 2, 7, 10, 0, 6, 9, 11, 12 };
	bstree tree1;
	for(int i=0; i<12; ++i)
        tree1.insert(a[i]);

	int b[20];
	for(int i=0; i<20; i++) 
		b[i] = i*2;
    bstree maxTree, minTree;
	for(int i=0; i<20; i++)
		maxTree.insert(b[i]);
	minTree.insertSorted(b, 0, 19);

	cout << "BST? " << tree1.checkBST_v1() << endl;
	cout << "BST? " << minTree.checkBST_v1() << endl;
	cout << "BST? " << maxTree.checkBST_v1() << endl;
    cout << "\n";
	cout << "BST? " << tree1.checkBST_v2() << endl;
	cout << "BST? " << minTree.checkBST_v2() << endl;
	cout << "BST? " << maxTree.checkBST_v2() << endl;
    cout << "\n";
	cout << "BST? " << tree1.checkBST_v3() << endl;
	cout << "BST? " << minTree.checkBST_v3() << endl;
	cout << "BST? " << maxTree.checkBST_v3() << endl;

    return 0;
}
