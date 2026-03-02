// Q. Find the "next" node of a given node in a BST in Inorder Traverse
//    Inorder: left subtree -> current node -> right subtree

/*
Checks: So, the next node is the leftmost right subtree
		1. What if the node does not have a right subtree?
			-> We're done traversing n's subtree. -> where the node is left off its parents?
		2. If the node is a left child, then the next node is the parent
		3. If the node is a right child, then we fully traversed the parent's subtree.
			-> check where the parent's node is left off from its parent.
*/

#include <iostream>
#include <cstring>

#include "BST_parent.h"

Node* leftMostChild(Node* no){    
    if(no == NULL) return NULL;
    while(no->lchild)
        no = no->lchild;
    return no;
}

Node* successor(Node* no){
    if(no == NULL) return NULL;

	// 1. Found right child -> return leftmost node of right
    if(no->rchild) return leftMostChild(no->rchild);
	
	// 2. No right child
    Node *y = no->parent;
    while(y && y->rchild==no){ 	// check its parent, if node is left then parent is the successor
        no = y;                 // if node is right child, then keep moving up
        y = y->parent;
    }
    return y;
}

int main(){
    int a[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9
    };
    init();
    Node *head = NULL;
    create_minimal_tree(head, NULL, a, 0, 9);
    cout<<"the head is "<<head->key<<endl;
    cout<<"the successor of head is: "<<(successor(head->lchild))->key<<endl;
    return 0;
}
