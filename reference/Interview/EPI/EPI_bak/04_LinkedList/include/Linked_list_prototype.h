// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#ifndef SOLUTIONS_LINKED_LIST_PROTOTYPE_H_
#define SOLUTIONS_LINKED_LIST_PROTOTYPE_H_

#include <memory>

using std::shared_ptr;
using std::cout;
using std::endl;


template <typename T>
struct ListNode {
  T data;
  shared_ptr<ListNode<T>> next;
};

// Recursive version
template <typename T>
void printR(shared_ptr<ListNode<T>> head) {
	if(head) {
		cout << "(" << head->data << ")" << endl;
		print(head->next);
	}
}

template <typename T>
void print(shared_ptr<ListNode<T>> head) {
	while(head) {	// head != nullptr
		cout << "(" << head->data << ")" << endl;
		head=head->next;
	}
}


#endif  // SOLUTIONS_LINKED_LIST_PROTOTYPE_H_
