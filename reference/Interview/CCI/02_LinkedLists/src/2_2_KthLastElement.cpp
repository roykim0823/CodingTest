// Q. Implement an algorithm to find the kth to last element of a single linked list

/*
Checks:
	1. if the size of linked list is known: kth last one = (length - k) 

Solutions:
	Recursive: Simple code , but requires O(n) space
	Iterative:
		1. use two pointer, (p and q), and q move forward by k nodes
		2. move the two pointers until q reaches the null.
		3. Then p stays at the kth to last element of the list
*/

#include <iostream>
using namespace std;

typedef struct node{
    int data;
    node *next;
}node; 

node* init(int a[], int n){
    node *head, *p;
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


// O(n) time, O(n) space due to recursive call
node* findNthToLast_v1(node *n, int k) {
	static int i=0;	// or use referce i
	if(n==NULL || k<1) return NULL;

	node *newNode = findNthToLast_v1(n->next, k);
	i++;
	if(i==k)
		return n;
	return newNode;
}

// O(n) time, O(1) space due to iterative
node* findNthToLast_v2(node *head, int k){
    if(head==NULL || k<1) return NULL;

    node *p=head, *q=head;

	// Move q forward k nodes into the list.
	for(int i=0; i<k-1; i++) {
		if(q == NULL) return NULL;	// Error Check
		q = q->next;
	}
	if(q==NULL) return NULL;

	// Now, move p and q together until q hits the end.
	// p will be at the right element.
    while(q->next) {
		p=p->next;
		q=q->next;
	}

	return p;
}

int main(){
    int n = 10;
    int a[] = {
        9, 2, 1, 3, 5, 6, 2, 6, 3, 1 
    };
    node *head = init(a, n);
    node *p = findNthToLast_v2(head, 2);
    if(p) cout<<p->data<<endl;
    else cout<<"the length of link is not long enough"<<endl;

    p = findNthToLast_v1(head, 2);
    if(p) cout<<p->data<<endl;
    else cout<<"the length of link is not long enough"<<endl;
	return 0;
}
