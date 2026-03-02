// Q. Write code to partition a linked list around a value x, such that all nodes less than x 
//    come before all nodes greater than or equal to x

/*
Checks:
	0. If this were an array, be careful. Array shifts are very expensive
	-> No quicksort is based on this
	1. Use two linked list for <x and >=x 
	2. Then merge the two list at the end
*/

#include <iostream>
using namespace std;

typedef struct node{
    int data;
    node *next;
}node;

node* init(int a[], int n){
    node *head=NULL, *p;
    for(int i=0; i<n; ++i){
        node *nd = new node();
        nd->data = a[i];
        if(i==0){
            head = p = nd;
            continue;
        }
        p->next = nd;
        p = nd;
    }
    return head;
}

void print(node *head);

// Insert a node at the end of each list: Use two pointers for each list
node* partition_v1(node *n, int x) {
	node* beforeStart=NULL;
	node* beforeEnd=NULL;
	node* afterStart=NULL;
	node* afterEnd=NULL;

	// Partition the List
	while(n) {
		node *next = n->next;	// Remember next item
		n->next=NULL; 			// To add one item each time
		// Insert node into end of beforelist
		if(n->data < x) {
			if(beforeStart==NULL) {
				beforeStart=n;
	  	  		beforeEnd = beforeStart;
			} else {
		  	  	beforeEnd->next = n;
		   		beforeEnd = n;
			}
		} else {
        // Insert node into end of afterlist
			if(afterStart==NULL) {
				afterStart=n;
				afterEnd=afterStart;
			} else {
		  	  	afterEnd->next = n;
		   		afterEnd = n;
			}
		}
		n = next;
	}

	print(beforeStart);
	print(afterStart);
	if(beforeStart == NULL) 
		return afterStart;
	
	// Merge beforelist and afterlist
	beforeEnd->next = afterStart;
	return beforeStart;
}

// Insert a node in front of the list: Use one pointer for each list
node* partition_v2(node *n, int x) {
	node* beforeStart=NULL;
	node* afterStart=NULL;

	// Partition the List
	// beforeStart or afterStart points to the last added item!
	while(n) {
		node *next = n->next;	// To store next item
		// Insert node into start of beforelist
		if(n->data < x) {
			n->next = beforeStart;
			beforeStart = n;
		} else {
        // Insert node into front of afterlist
			n->next = afterStart;
			afterStart = n;
		}
		n = next;
	}

	print(beforeStart);
	print(afterStart);
	
	if(beforeStart == NULL) 
		return afterStart;
	
	// Find end of before list, and merge beforelist and afterlist
	// Extra complexity
	node *list = beforeStart;
	while(beforeStart->next) { 
		beforeStart = beforeStart->next;
	}
	beforeStart->next = afterStart;
	return list;
}
	


void print(node *head){
    while(head){
        cout<<head->data<<" ";
        head = head->next;
    }
    cout<<endl;
}

int main(){
    int n = 10;
    int a[] = {
        1, 2, 9, 9, 3, 4, 7, 5, 11, 6
    };

    node *list = init(a, n);
	print(list);
    node *p = partition_v1(list, 7);
	print(p);
    node *p2 = partition_v2(list, 7);
	print(p2);
    return 0;
}
