// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#ifndef SOLUTIONS_REVERSE_LINKED_LIST_TEMPLATE_H_
#define SOLUTIONS_REVERSE_LINKED_LIST_TEMPLATE_H_

#include <memory>

#include "Linked_list_prototype.h"

using std::shared_ptr;

// recursive version
// O(n) time, O(n) space
template <typename T>
shared_ptr<ListNode<T>> reverse_linked_listR(
    const shared_ptr<ListNode<T>>& head) {
	if( !head || !head->next) {
		return head;
	}

	// new_head assigned to nullptr at the last node
	shared_ptr<ListNode<T>> new_head = reverse_linked_listR(head->next);
	head->next->next = head;	// new_head points to head
	head->next = nullptr;       // head->next to nullptr to assign next node in prev node
	return new_head;
}

// iterative version
// O(n) time, O(1) space
template <typename T>
shared_ptr<ListNode<T>> reverse_linked_list(
    const shared_ptr<ListNode<T>>& head) {
  shared_ptr<ListNode<T>> prev = nullptr, curr = head;
  while (curr) {
    shared_ptr<ListNode<T>> temp = curr->next;
    curr->next = prev;
    prev = curr;
    curr = temp;
  }
  return prev;
}

#endif  // SOLUTIONS_REVERSE_LINKED_LIST_TEMPLATE_H_
