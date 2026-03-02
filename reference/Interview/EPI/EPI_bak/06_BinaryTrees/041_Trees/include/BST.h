#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <list>
#include <limits>

#ifndef BST_H
#define BST_H
using namespace std;
struct node {
	int key_value;
	node *left, *right;
};

class bstree {
public:
	bstree() 	{ root=NULL; }
	~bstree() 	{ destory_tree(root); }
	
	void insert(int key) {
		if(root!=NULL)
			insert(key, root);
		else {
			root=new node;
			root->key_value=key;
			root->left=NULL;
			root->right=NULL;
		}
	}

	node *search(int key) { return search(key, root); }
    int getHeight() { return getHeight(root); }
	vector<list<node*> > getLevelList();
    void preorder() { this->traverse_preorder(root); }
    void inorder() { this->traverse_inorder(root); }
    void postorder() { this->traverse_postorder(root); }

    // Problem 1
    int checkHeight(node *leaf); 						
	bool isBalanced();                                 	
	
	// Problme 3
	void insertSorted(int arr[], int begin, int end); 	

	// Problem 5
	bool checkBST_v1();                              		
	bool checkBST_v2();                              		
	bool checkBST_v3();                              		
private:

	void checkBST_v1(node *t, vector<int> &sorted);
    bool checkBST_v2(node *t, int &last_item);
	bool checkBST_v3(node *n, int min, int max);

	void destory_tree(node *leaf);
	void insert(int key, node *leaf);
	node *search(int key, node *leaf);
	int getHeight(node *leaf);
	
	void traverse_preorder(node *t) {
		if(t) {
			cout << t->key_value << " ";
			traverse_preorder(t->left);
			traverse_preorder(t->right);
		}
	}

	void traverse_inorder(node *t) {
		if(t) {
			traverse_inorder(t->left);
			cout << t->key_value << " ";
			traverse_inorder(t->right);
		}
	}

	void traverse_postorder(node *t) {
		if(t) {
			traverse_postorder(t->left);
			traverse_postorder(t->right);
			cout << t->key_value << " ";
		}
	}

	node *root;
};

// recursively remove nodes from the root
void bstree::destory_tree(node *leaf) {
	if(leaf!=NULL) {
		destory_tree(leaf->left);
		destory_tree(leaf->right);
		delete leaf;
	}
}

void bstree::insert(int key, node *leaf) 
{
	if(key < leaf->key_value) {	// go to left
		if(leaf->left != NULL)
			insert(key, leaf->left);
		else {
			leaf->left=new node;
			// insert new node in the left
			leaf->left->key_value=key;
			leaf->left->left=NULL;
			leaf->left->right=NULL;
		}
	} else { 	// go to right include equal
		if(leaf->right != NULL)
			insert(key, leaf->right);
		else {
			leaf->right=new node;
			// insert new node in the left
			leaf->right->key_value=key;
			leaf->right->left=NULL;
			leaf->right->right=NULL;
		}
	}
}

node *bstree::search(int key, node *leaf)
{
	if(leaf!=NULL)
	{
		if(key==leaf->key_value)
			return leaf;
		if(key<leaf->key_value)
			return search(key, leaf->left);
		else
			return search(key, leaf->right);
	}else
		return NULL;
}

// Get the height of the tree
int bstree::getHeight(node *leaf) {
	if(leaf==NULL) return 0;
	return max(getHeight(leaf->left), getHeight(leaf->right)) + 1;
}

// 4_4 get a linked list of each depth using BFS
vector<list<node*> > bstree::getLevelList() {
	vector<list<node*> > levelList;
    int level=0;
	list<node*> l; 
	l.push_back(root);
	levelList.push_back(l);
	while(!levelList[level].empty()) {
		list<node*> lt;
		list<node*>::iterator iter;
		for(iter=levelList[level].begin(); iter!=levelList[level].end(); ++iter) {
			node *n = *iter;
			//if(n->left!=NULL) lt.push_back(n->left);
			//if(n->right!=NULL) lt.push_back(n->right);
			if(n->left) lt.push_back(n->left);
			if(n->right) lt.push_back(n->right);
		}
		++level;
		if(lt.empty()) 	// to remove the last empty list
			break;
		levelList.push_back(lt);
	}
	return levelList;
}

void printLevelList(vector<list<node* > > levelList) {
	vector<list<node*> >::iterator iter;
	int level=0;
	for(iter=levelList.begin(); iter!=levelList.end(); ++iter) {
		list<node*> lt = *iter;
		list<node*>::iterator iter2;
		cout << "Level" << level << " : ";
		for(iter2=lt.begin(); iter2!=lt.end(); ++iter2) {
			node *n = *iter2;
			cout<< n->key_value << " ";
		}
		cout << endl;
		level++;
	}
}
#endif
