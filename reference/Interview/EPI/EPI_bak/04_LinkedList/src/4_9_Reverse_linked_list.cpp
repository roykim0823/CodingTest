// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <iostream>
#include <memory>
#include "Linked_list_prototype.h"
#include "Reverse_linked_list.h"

using namespace std;


int main(int argc, char* argv[]) {
  shared_ptr<ListNode<int>> L1 =
      make_shared<ListNode<int>>(ListNode<int>{1, nullptr});
  shared_ptr<ListNode<int>> L2 =
      make_shared<ListNode<int>>(ListNode<int>{2, nullptr});
  L1->next = L2;
  shared_ptr<ListNode<int>> L3 =
      make_shared<ListNode<int>>(ListNode<int>{3, nullptr});
  L2->next = L3;

  cout << "before reverse" << endl;
  print(L1);
  shared_ptr<ListNode<int>> newhead = reverse_linked_list(L1);
  cout << endl << "after reverse" << endl;
  print(newhead);
  newhead = reverse_linked_listR(newhead);
  cout << endl << "after another reverse" << endl;
  print(newhead);
  return 0;
}
