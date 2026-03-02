// Q. Given a circular linked list, implement an algorithm which returns the node
//	at the beginning of the loop
/* Solution: 
1. Detect if Linked list has a loop
	FastRunner(move two nodes) and SlowRunner(move one nodes)
	FastRunner hop over SlowRunner at spot i 
	then FastRunner at spot i-1 and SlowRunner at spot i-1
  	So, they would have collided.
2. When Do They Collide
	a. SlowRunner is 0 step in the loop (takes k steps)
		-> FastRunner is k steps in to the loop
	b. Then SlowRunner is k step behind FastRunner 
		-> FastRunner is LoopSize-k step behind SlowRunner
	c. FastRunner catches up the SlowRunner 1 step at a time 
		-> it takes LoopSize-k step
	d. When FastRunner catches up SlowRunner after LoopSize-k steps
		-> They are k steps before the the start of the loop
3. Move SlowRunner at the head of list then move both of them 1 node at a time.
	-> After k steps, they will meet at the beginning of the loop

Solution2: Use hash -> same as detect a duplicated node
		   Use address of each node for the hash key
*/

#include <iostream>
#include <map>
using namespace std;

typedef struct node{
    int data;
    node *next;
}node;

node* init(int a[], int n, int m){
    node *head, *p, *q;
    for(int i=0; i<n; ++i){
        node *nd = new node();
        nd->data = a[i];
        if(i==m) q = nd;	// make a loop
        if(i==0){
            head = p = nd;
            continue;
        }
        p->next = nd;
        p = nd;
    }
    p->next = q;
    return head;
}

node* loopstart(node *head){
    if(head==NULL) return NULL;
    node *fast = head, *slow = head;
    while(fast && fast->next){
        fast = fast->next->next;
        slow = slow->next;
        if(fast==slow) break;
    }
	
	// No meeting point -> no loop
    if(!fast || !fast->next) return NULL;

    slow = head;
    while(fast!=slow){
        fast = fast->next;
        slow = slow->next;
    }
    return fast;
}

/*
// With Hashing
map<node*, bool> hash;

node* loopstart1(node *head){
    while(head){
        if(hash[head]) return head;
        else{
            hash[head] = true;
            head = head->next;
        }
    }
    return head;
}
*/
int main(){
    int n = 10, m = 9;// m<n
    int a[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9 
    };
    node *head = init(a, n, 5);
    node *p = loopstart(head);
    //node *p = loopstart1(head);
    if(p)
        cout<<p->data<<endl;
    return 0;
}
