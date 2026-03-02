// Q. Find first common ancestor of two nodes in a binary tree. Avoid storing additional nodes in a 
//    data structure. NOTE: This is not necessariliy a binary search tree.

/* Check:
	1. With Links to Parent: Trace p and q's paths up until they intersect.
		But, it requres
			(a) being able to mark nodes as isVisited 	-> Additional storage
			(b) being able to store some data like Hash	-> Additional storage
	2. Without Links to Parent: follow a chain in which p and q are on the same side
		(a) if p and q are both on the left of the node, branch left to look for common ancestor
		(a) if p and q are both on the right of the node, branch right to look for common ancestor
*/
#include <map>
#include "BST_parent.h"

using namespace std;

// EPI-6_11
// Without additional stroage, without Parent node: O(n)
Node *LCA(Node *n, Node *a, Node *b) {
	if(n==NULL)	// empty subtree 
		return NULL;
	else if( n==a || n==b)	// if one of node is head
		return n;
	
	Node *left_sub = LCA(n->lchild, a, b);
	Node *right_sub = LCA(n->rchild, a, b);
	if(left_sub && right_sub) {	// found a and b in different subtrees
		return n;
	} else {
		return left_sub ? left_sub : right_sub;
	}
}

// EPI-6_13
// Without additional stroage, with parent node: O(h) [h=depth] time, O(1) space
int get_depth(Node *n) {
	int d=0;
	while(n) {
		++d;
		n=n->parent;
	}
	return d;
}

Node *LCA_wP(Node *a, Node *b) {
	int depth_a = get_depth(a);
	int depth_b = get_depth(b);
	
	if(depth_b > depth_a) {
		int temp=depth_a;
		depth_a=depth_b;
		depth_b=temp;
	}

	// Advance deeper node first
	int depth_diff = depth_a - depth_b;
	while(depth_diff--) {
		a = a->parent;
	}

	// Both pointers advance until they found a common ancestor
	while(a!=b) {
		a=a->parent;
		b=b->parent;
	}

	return a;
}

// EPI-6_14
// With hashTable , with parent node: O(max(da-dl, db-dl)) -> O(max(da, db)-dl) space and time
// Trading time with space -> fastest
Node* LCA_wS(Node* a, Node* b) {
    map<Node*, bool> m;	// stroage 
	
	while (a || b) {
		if(a) {
			if(m[a]==true) {
				return a;	// adds a failed because a exists in hash
			}
			m[a]=true;
			a = a->parent;
		}
		if(b) {
			if(m[b]==true) {
				return b;	// adds a failed because a exists in hash
			}
			m[b]=true;
			b = b->parent;
		}
	}
	return a; // ?
}

int main(){
    init();
    int a[] = {
        0, 1, 2, 3, 4, 5, 6
    };
    Node *head = NULL;
    create_minimal_tree(head, NULL, a, 0, 6);
    Node *n1 = search(head, 0);
    Node *n2 = search(head, 4);
    cout<<n1->key<<" "<<n2->key<<endl;

	Node *lca = NULL;
    lca=LCA(head, n1, n2);
    cout<<lca->key<<endl;
	
	Node *lca_wP = NULL;
    lca_wP=LCA_wP(n1, n2);
    cout<<lca_wP->key<<endl;
	
	Node *lca_wS = NULL;
    lca_wS=LCA_wS(n1, n2);
    cout<<lca_wS->key<<endl;
    return 0;
}
