// Given a sorted (increasing order) array, write an alogirthm to create a BST with min height.
#include "BST.h"
#include "4_1.h"
#include "4_3.h"

using namespace std;

int main(){
	int b[20];
	for(int i=0; i<20; i++) 
		b[i] = i*2;
    bstree maxTree, minTree;
	for(int i=0; i<20; i++)
		maxTree.insert(b[i]);
	vector<list<node*> > maxTreeList = maxTree.getLevelList();
	minTree.insertSorted(b, 0, 19);
	vector<list<node*> > minTreeList = minTree.getLevelList();

	cout << "maxTree is Balanced? " << maxTree.isBalanced() << endl;
	cout << "maxTree's height? " << maxTree.getHeight() << endl;
	printLevelList(maxTreeList);
	cout << "Inorder ";
	maxTree.inorder();
	cout << endl;
	cout << endl;
	cout << "minTree is Balanced? " << minTree.isBalanced() << endl;
	cout << "minTree's height? " << minTree.getHeight() << endl;
	printLevelList(minTreeList);
	cout << "Inorder ";
	minTree.inorder();
	cout << endl;
    
	return 0;
}
