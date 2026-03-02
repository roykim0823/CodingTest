// Q. You have two numbers represented by a linked list, where each node contains a
//    single digit. The digits are stored in reverse order, such that the 1's digit is 
//    at the head of the list. Write a function that adds the two numbers and returns
//    the sum as a linked list.
// Q+. Suppose the digits are stored in forward order. Repeat the above problem.

/*
Check:
	  6 1 7
	+ 2 9 5
	But, p: 7->1->6 and q: 5->9->2 in reverse order
	Q+: Simply reverse the list, add the two list, reverse the sum
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
        } p->next = nd; p = nd; } return head;
}

// (Recursive) Reverse order
node* addlink_v1(node *p, node *q, int carry=0) {
	// Addition is done if both lists are null AND the carry is 0
	if(p==NULL && q==NULL && carry==0)
		return NULL;
	
	node *result = new node();

	// Add value
	int value = carry;
	if(p)	value+=p->data;
	if(q)	value+=q->data;

	result->data = value % 10;

	// Recursive
	if(p || q || value >= 10) {
		node *temp = addlink_v1(
			p==NULL? NULL : p->next, 
			q==NULL? NULL : q->next, 
			value >= 10? 1 : 0 );
		result->next = temp;
	}
	return result;
}

// Iterative Version
node* addlink_v2(node *p, node *q){
    if(p==NULL) return q;
    if(q==NULL) return p;
    node *res, *pre=NULL;
    int c = 0;
    while(p && q){
        int t = p->data + q->data + c;
        node *r = new node();
        r->data = t%10;
        if(pre){
            pre->next = r;
            pre = r;
        }
        else pre = res = r;
        c = t/10;
        p = p->next; q = q->next;
    }
    while(p){
        int t = p->data + c;
        node *r = new node();
        r->data = t%10;
        pre->next = r;
        pre = r;
        c = t/10;
        p = p->next;
    }
    while(q){
        int t = q->data + c;
        node *r = new node();
        r->data = t%10;
        pre->next = r;
        pre = r;
        c = t/10;
        q = q->next;
    }
    if(c>0){
        node *r = new node();
        r->data = c;
        pre->next = r;
    }
    return res;
}

void print(node *head);
// FOLLOW UP: foward add, using insert in front 
node* reverseLink(node *p) {
	node *rev=NULL, *cur=p;
	while(cur) {
		node *temp = cur->next;
		cur->next=rev;
		rev=cur;
		cur=temp;
	}
	return rev;
}

// recursive version
node* reverseLinkR(node *p) {
	if(!p || !p->next)	// if p or p->next is NULL
		return p;
	
	node *rev=reverseLinkR(p->next);
	p->next->next=p;
	p->next=NULL;
	return rev;
}
node* addlink_v3(node *p, node *q) {
	node *p_rev = reverseLinkR(p);
	node *q_rev = reverseLinkR(q);

	node *result_rev = addlink_v1(p_rev, q_rev);
	// return the reverse link
	p = reverseLinkR(p_rev);
	q = reverseLinkR(q_rev);
	return reverseLinkR(result_rev);
}

// Follow up2: do not use reverse 
/*
node* addlink_v4(node *p, node *q) {
	// check the length	: O(n) -> similar to reverse
	int len1 = length(p);
	int len2 - length(q);

	// Pad the shorter list with zeros
	if(len1<len2) {
		l1=padList(l1, len2-len1);
	} else {
		l2=padList(l2, len1-len2);
	}

	// Add List
	PartialSum  sum = addListsHelper(l1, l2);

	// If there is a carry value left over, insert this at the front of the list.
	// Otherwise, just return the linked list
	if(sum.carry==0) {
		return sum.sum;
	} else {
		result = insertBefore(sum.sum, sum.carry);
		return result;
	}
}
*/

void print(node *head){
    while(head){
        cout<<head->data<<" ";
        head = head->next;
    }
    cout<<endl;
}

int main(){
    int n = 5;
    int a[] = {
        1, 2, 9, 9, 3
    };
    int m = 3;
    int b[] = {
        9, 9, 2
    };

    node *p = init(a, n);
    node *q = init(b, m);
    cout << "p= "; if(p) print(p);
    cout << "q= "; if(q) print(q);
    node *res = addlink_v1(p, q);
    node *res2= addlink_v2(p, q);
    node *res3= addlink_v3(p, q);
    if(res) print(res);
    if(res) print(res2);
    if(res) print(res3);
    cout << "p= "; if(p) print(p);
    cout << "q= "; if(q) print(q);
    return 0;
}
