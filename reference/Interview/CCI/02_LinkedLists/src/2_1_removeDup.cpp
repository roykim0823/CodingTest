// Q. Write code to remove duplicates from an unsorted linked list
// Q+. what if a temporary buffer is not allowed?

/* 
Solutions:
	1. Use hashtable to detect duplicate
		-> O(n)
	2. Without hashtable, check every possible duplicate pairs
		-> O(n^2)
*/

#include <iostream>
#include <cstring>
#include <map>

using namespace std;

typedef struct node{
    int data;
    node *next;
}node;

node* init(int a[], int n){
    node *head, *cur;

    for(int i=0; i<n; ++i){
        node *newNode = new node();
        newNode->data = a[i];
        if(i==0){
            head = cur = newNode;
            continue;
        }
        cur->next = newNode;
        cur = newNode;
    }
    return head;
}

// O(n) with hashtable
void removeDulicate(node *n){
    if(n==NULL) return;

    node *prev=NULL;
 
 	map<int, bool> hash_table;
	while(n){
        if(hash_table[n->data]){ 	// Hit on hash_table -> duplicate
            prev->next = n->next; 	// skip the node in the list
            // delete the duplicated node 
			node *t = n;
            delete t;               
        } else {
            hash_table[n->data] = true; 	// update hash_table
            prev = n;                       // update prev pointer
        }
		n = n->next;
    }
}

// O(n^2) without a buffer
void removeDulicate1(node *n){
    if(n==NULL) return;

	node *cur = n;
    while(cur){
		// remove all future nodes that have the same value
		node *runner = cur;
		while(runner->next != NULL) {
            if(runner->next->data==cur->data){
				runner->next = runner->next->next;
            	// delete the duplicated node 
                node *t = runner->next;
                delete t;                 	
            } else {
				runner = runner->next;
            }
        }
		cur=cur->next;
    }
}

void print(node *n){
    while(n){
        cout<<n->data<<" ";
        n = n->next;
    }
    cout<<endl;
}
int main(){
    int n = 10;
    int a[] = { 3, 2, 1, 3, 5, 6, 2, 6, 3, 1 };
    //memset(hash, false, sizeof(hash));
    node *head = init(a, n);
	print(head);
    removeDulicate(head);
    print(head);
    return 0;
}
