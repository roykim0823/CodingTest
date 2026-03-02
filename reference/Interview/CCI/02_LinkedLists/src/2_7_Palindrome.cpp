// Q. Implement a function to check if a linked list is a palindrome. 
//	  e.g., 0->1->2->3->2->1->0
/* 
Solution: 
1. Reverse and Compare: only need to compare the first half of the lists
A. Iterative Approach
	1. Push the first half element onto a stack (Be careful if the size is odd)
		- The list size is known, then half size is size/2.
		- The list size is unknown, use the fast and slow runner
	2. Compare the first half element from the stack to the second half
*/

#include <iostream>
#include <stack>
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

bool isPalindrome_iter(node *head) {
    node *fast = head, *slow = head;
    stack<int> first_half;

	// Push elements from first half of linked list onto a stack
	while(fast && fast->next) {
		first_half.push(slow->data);
		slow=slow->next;
		fast = fast->next->next;
    }
	// if odd number of elements, skip the middle element
	if(fast) 
		slow=slow->next;
	
	while(slow) {
		int top=first_half.top();
		first_half.pop();
        if(top !=slow->data) {
			//cout << top << " " << slow->data << endl;
			return false;
		}
		slow=slow->next;
	}
	return true;
}

// no if for the odd number elements
bool isPalindrome_iter2(node *head) {
    node *fast = head, *slow = head;
    stack<int> first_half;

	// Push elements from first half of linked list onto a stack
	while(fast) {
		first_half.push(slow->data);
		fast=fast->next;
		if(fast) {       
			fast=fast->next;
			slow=slow->next;
		}
    }
	while(slow) {
		int top=first_half.top();
		first_half.pop();
        if(top !=slow->data) {
			//cout << top << " " << slow->data << endl;
			return false;
		}
		slow=slow->next;
	}
	return true;
}

void print(node *n){
    while(n){
        cout<<n->data<<" ";
        n = n->next;
    }
    cout<<endl;
}

int main(){
    int a[] = {
        0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0 
    };
    int b[] = {
        0, 1, 2, 3, 4, 4, 3, 2, 1, 0 
    };
    int c[] = {
        0, 1, 2, 3, 4,  
    };
    node *head = init(a, 11);
	print(head);
	cout << isPalindrome_iter(head) << endl;;
	cout << isPalindrome_iter2(head) << endl;;
  
  	head = init(b, 10);
	print(head);
	cout << isPalindrome_iter(head) << endl;;
	cout << isPalindrome_iter2(head) << endl;;
    
	head = init(b, 5);
	print(head);
	cout << isPalindrome_iter(head) << endl;;
	cout << isPalindrome_iter2(head) << endl;;
    return 0;
}
