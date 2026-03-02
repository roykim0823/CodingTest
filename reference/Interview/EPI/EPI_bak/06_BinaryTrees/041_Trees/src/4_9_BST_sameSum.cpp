// Q. Design an algorithm to print all paths which sum to a given value. Note that a path can start or
//    end anywhere in the tree.

/*
Checks:
	1. Simplify: what if the path had to start at the root, but could end any where?
		a. Start from the root and branch left and right, computing the sum
		b. When the sum is found, print the current path. But keep branching the tree.
	2. Generalize: every node, check if the sum is found. 
*/

#include <iostream>
#include <vector>
#include <cstring>
#include "BST_parent.h"
using namespace std;

void print(Node* head, int level){
    vector<int> v;
    for(int i=0; i<level; ++i){
        v.push_back(head->key);
        head = head->parent;
    }
    while(!v.empty()){
        cout<<v.back()<<" ";
        v.pop_back();
    }
    cout<<endl;
}

void find_sum(Node* head, int sum){
    if(head == NULL) return;
    Node *no = head;
    int tmp = 0;
	// Check each node to root to find sum
    for(int i=1; no!=NULL; ++i){
        tmp += no->key;
        if(tmp == sum)
            print(head, i);
        no = no->parent;
    }
    find_sum(head->lchild, sum);
    find_sum(head->rchild, sum);
}

void print2(vector<int> v, int level){
    for(int i=level; i<v.size(); ++i)
        cout<<v.at(i)<<" ";
    cout<<endl;
}
void find_sum2(Node* head, int sum, vector<int> v, int level){
    if(head == NULL) return;
    v.push_back(head->key);
    int tmp = 0;
    for(int i=level; i>-1; --i){
        tmp += v.at(i);
        if(tmp == sum)
            print2(v, i);
    }
    vector<int> v1(v), v2(v);
    find_sum2(head->lchild, sum, v1, level+1);
    find_sum2(head->rchild, sum, v2, level+1);
}

int main(){
    init();
    int a[] = {
        4, 3, 8, 5, 2, 1, 6
    };
    Node *head = NULL;
    create_minimal_tree(head, NULL, a, 0, 6);
	cout << "v1..." << endl;
    find_sum(head, 8);
    vector<int> v;
	cout << "v2..." << endl;
    find_sum2(head, 8, v, 0);
    return 0;
}
