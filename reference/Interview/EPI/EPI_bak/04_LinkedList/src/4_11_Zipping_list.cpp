// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <cassert>
#include <iostream>
#include <memory>
#include <random>
#include <string>

#include "Reverse_linked_list.h"

using std::default_random_engine;
using std::make_shared;
using std::random_device;
using std::stoi;
using std::uniform_int_distribution;


void connect_a_next_to_b_advance_a(shared_ptr<ListNode<int>>& a,
                                   const shared_ptr<ListNode<int>>& b) {
  shared_ptr<ListNode<int>> temp = a->next;
  a->next = b;
  a = temp;
}

// Zipping the list
// 1. find the middle of the list
// 2. reverse the second half
// 3. interleave the first and second halves
// corner cases: the empty list, lists of length 1, even/odd lengths lists
shared_ptr<ListNode<int>> zipping_linked_list(
    const shared_ptr<ListNode<int>>& L) {

  // 1. Find the middle point of L.
  shared_ptr<ListNode<int>> slow = L, fast = L, pre_slow = nullptr;
  while (fast) {
    fast = fast->next;
    if (fast) {
      pre_slow = slow;
      fast = fast->next, slow = slow->next;
    }
  }

  if (!pre_slow) {
    return L;  // only contains one node in the list.
  }

  // splits the list into two lists.
  pre_slow->next = nullptr;  
  
  // 2. reverse the second half 
  shared_ptr<ListNode<int>> reverse = reverse_linked_list(slow); 
  
  // Zipping the list.
  auto curr = L;
  while (curr && reverse) {
    // Connect curr->next to reverse, and advance curr.
    connect_a_next_to_b_advance_a(curr, reverse);
    if (curr) {
      // Connect reverse->next to curr, and advance reverse.
      connect_a_next_to_b_advance_a(reverse, curr);
    }
  }
  return L;
}


int main(int argc, char* argv[]) {
  default_random_engine gen((random_device())());
  
  shared_ptr<ListNode<int>> head = nullptr;
  int n;
  if (argc > 2) {
    for (int i = 1; i < argc; ++i) {
      auto curr =
          make_shared<ListNode<int>>(ListNode<int>{stoi(argv[i]), nullptr});
      curr->next = head;
      head = curr;
    }
  } else {
    if (argc == 2) {
      n = stoi(argv[1]);
    } else {
      uniform_int_distribution<int> dis(0, 10);
      n = dis(gen);
    }

    for (int i = n; i >= 0; --i) {
      auto curr = make_shared<ListNode<int>>(ListNode<int>{i, nullptr});
      curr->next = head;
      head = curr;
    }
  }
  cout << "before zipping a list" << endl;
  print(head);

  shared_ptr<ListNode<int>> curr = zipping_linked_list(head);
  cout << "the zipped list" << endl;
  print(curr);
  int idx = 0, pre;
  while (curr) {
    if (argc <= 2) {
      if (idx & 1) {
        assert(pre + curr->data == n);
      }
    }
    ++idx;
    cout << curr->data << endl;
    pre = curr->data;
    curr = curr->next;
  }
  return 0;
}
