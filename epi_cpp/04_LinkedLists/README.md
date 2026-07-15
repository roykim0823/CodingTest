# Linked Lists

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 4. See epilight_cpp_new.pdf for the full text. Original exposition — explanations, code, and examples are my own.*

## Overview

A list is an ordered collection of values, repetitions allowed. In a *singly linked list*, each node carries a payload plus a pointer to its successor; the first node is the *head*, the last is the *tail*, and the tail's next pointer is null. Variants abound: a *doubly linked list* adds a predecessor pointer to each node, and the end of a list can be marked by a sentinel node or a self-loop instead of null.

Lists and arrays both hold elements in linear order, but their cost profiles are opposites. Splicing a node in or out of a list at a known position is O(1) — just pointer updates, no shifting — while reaching the k-th element requires walking k links, O(n) in general. Arrays are the mirror image: O(1) access, O(n) insertion and deletion. Lists mostly serve as building blocks inside larger structures, but they also generate genuinely tricky interview problems of their own, usually about careful pointer surgery.

Unless stated otherwise, every problem in this chapter uses a minimal node with a data field and a next field:

```cpp
template <typename T>
struct ListNode {
  T data;
  // shared_ptr rather than a raw pointer because several nodes may
  // reference the same node -- e.g., in a list with a cycle, the
  // cycle-entry node has two distinct predecessors.
  shared_ptr<ListNode<T>> next;
};
```

## Boot camp: implementing the basic list API

List problems come in two flavors: those where you build list machinery yourself, and those where you lean on the standard library. Both matter; start with the former. Writing search, insert, and delete for a singly linked list is short but forces exactly the pointer discipline these problems demand.

**Search for a key.** Follow next pointers until the key appears or the list runs out. Conveniently, "ran out" leaves the cursor equal to null, which doubles as the not-found return value:

```cpp
shared_ptr<ListNode<int>> SearchList(shared_ptr<ListNode<int>> L, int key) {
  while (L && L->data != key) {
    L = L->next;
  }
  return L;  // null if the key was absent
}
```

**Insert after a given node.** Wire the new node's next to the successor *first*, then redirect the predecessor. Ordering matters — do it backwards and the old successor is lost:

```cpp
void InsertAfter(const shared_ptr<ListNode<int>>& node,
                 const shared_ptr<ListNode<int>>& new_node) {
  new_node->next = node->next;
  node->next = new_node;
}
```

**Delete the successor of a given node** (the node must not be the tail):

```cpp
void DeleteAfter(const shared_ptr<ListNode<int>>& node) {
  node->next = node->next->next;
}
```

**Complexity.** Insertion and deletion touch a constant number of pointers: O(1). Search may traverse everything (key at the tail, or absent): O(n) for n nodes.

## Top tips

- **O(1) space is usually possible.** The brute force stores nodes in an auxiliary container; the better solution threads through the *existing* nodes, spending only a few pointer variables.
- **These problems are about clean coding.** Conceptually most list questions are simple; the challenge is executing the pointer updates without leaks, drops, or dangling references.
- **Use a dummy head (sentinel).** Allocating a throwaway node in front of the real list eliminates the empty-list and first-node special cases — less branching, fewer bugs.
- **Watch the endpoints.** Forgetting to fix up next (and prev, for doubly linked lists) at the head and tail is *the* classic list bug.
- **Two iterators.** A huge share of singly-linked-list tricks use a pair of cursors — one lagging the other by a fixed distance, or one advancing twice per step. Cycle detection below is the canonical example.

## Know your linked list libraries

Many interview problems require a hand-rolled list class, but know the standard library too. C++ offers `list` (doubly linked) and `forward_list` (singly linked). A singly linked node is smaller — no prev pointer — at the price of forward-only iteration.

For `list` (doubly linked):
- Insertion/removal at the ends: `push_front(42)` / `emplace_front(42)`, `pop_front()`, `push_back(42)` / `emplace_back(42)`, `pop_back()`.
- `splice(L1.end(), L2)` transfers nodes between lists; `reverse()` flips the order; `sort()` sorts in place.

For `forward_list` (singly linked):
- `push_front(42)` / `emplace_front(42)`, `pop_front()`; position-relative editing goes through `insert_after(it, 42)` / `emplace_after(it, 42)` and `erase_after(it)` — "after" because a singly linked node cannot reach its predecessor.
- `splice_after(it, L2)` transfers elements; `reverse()` reverses.
- Use the member `sort()` rather than hand-rolling — it will save you real pain.

## 4.1 Test for cyclicity

**Problem.** A well-formed singly linked list ends in null, but nothing stops a next pointer from referencing an *earlier* node, creating a cycle. Given the head of a list of unknown length, return null if it is cycle-free; otherwise return the node where the cycle begins (the first node that is reachable twice).

*Hint: use two iterators — a slow one and a fast one.*

### Approach 1: hash table of visited nodes — O(n) time, O(n) space

Walk the list, inserting each node's address into a hash set. Either you reach null (no cycle) or you meet a node already in the set — and the *first* repeated node is precisely the cycle's entry, so this even answers the full question directly:

```cpp
shared_ptr<ListNode<int>> FindCycleStart(shared_ptr<ListNode<int>> head) {
  unordered_set<ListNode<int>*> seen;
  for (auto p = head; p; p = p->next) {
    if (!seen.insert(p.get()).second) {
      return p;  // first revisited node == start of cycle
    }
  }
  return nullptr;
}
```

Simple and linear, but O(n) space — the very thing list problems ask you to eliminate.

### Approach 2: nested traversal — O(n²) time, O(1) space

Without storage, count steps instead. The outer loop advances one node at a time; after its i-th step, an inner loop re-walks the first i nodes from the head and checks whether the outer node appears among them. A hit means the outer cursor has looped back over old ground — a cycle; if the outer loop ever reaches null, there is none. No extra memory, but the re-walking makes it quadratic.

### Approach 3: Floyd's slow/fast iterators — O(n) time, O(1) space

The optimal solution runs two cursors from the head: per iteration, *slow* advances one node and *fast* advances two. If the list terminates, fast hits null and there is no cycle. If there is a cycle, both cursors eventually orbit inside it, with fast gaining one position per step — and since the gap shrinks by exactly one each iteration, fast cannot leap over slow; they must land on the same node.

Meeting proves a cycle exists but the meeting point is *not* in general the cycle's start, so two more phases finish the job:

1. **Measure the cycle.** From the meeting node, walk until you return to it, counting steps — that count is the cycle length C.
2. **Find the entry.** Start two cursors at the head, advance one of them C steps, then move both one step at a time. When they meet, that node is the cycle's start. Why: the moment the lead cursor is C ahead, both cursors are C apart forever; the first node at which "C ahead" coincides with "same node" is the first node lying on the cycle.

```cpp
shared_ptr<ListNode<int>> HasCycle(const shared_ptr<ListNode<int>>& head) {
  auto slow = head, fast = head;
  while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
    if (slow == fast) {  // cycle detected
      // Phase 1: measure the cycle length C.
      int cycle_len = 0;
      do {
        ++cycle_len;
        fast = fast->next;
      } while (slow != fast);

      // Phase 2: advance a cursor C steps from the head...
      auto ahead = head;
      while (cycle_len--) {
        ahead = ahead->next;
      }
      // ...then move both cursors in tandem until they meet.
      auto iter = head;
      while (iter != ahead) {
        iter = iter->next;
        ahead = ahead->next;
      }
      return iter;  // the start of the cycle
    }
  }
  return nullptr;  // reached the end: no cycle
}
```

**Worked example.** Nodes n1 → n2 → n3 → n4 → n5, with n5->next = n3 (cycle {n3, n4, n5}, so C = 3 and the entry is n3).

| iteration | slow | fast |
|---|---|---|
| 1 | n2 | n3 |
| 2 | n3 | n5 |
| 3 | n4 | n4 — met |

Phase 1 from n4: n5, n3, n4 → C = 3. Phase 2: ahead starts 3 past the head at n4; tandem stepping gives (n1, n4) → (n2, n5) → (n3, n3) — they coincide at n3, the true cycle start.

**Complexity.** Let F be the number of nodes before the cycle and C the cycle length (F + C ≤ n). Detection costs O(F) for both cursors to enter the cycle plus O(C) for them to collide inside it; the two follow-up phases are O(C) and O(F + C). Total O(n) time, O(1) space.

**Variants:** a well-known shorter program claims to find the cycle start *without* measuring C: after slow and fast meet, reset slow to the head and advance both cursors one step at a time; return the node where they meet.

```cpp
shared_ptr<ListNode<int>> HasCycle(const shared_ptr<ListNode<int>>& head) {
  auto slow = head, fast = head;
  while (fast && fast->next && fast->next->next) {
    slow = slow->next;
    fast = fast->next->next;
    if (slow == fast) {
      slow = head;  // restart slow; advance both at equal speed
      while (slow != fast) {
        slow = slow->next;
        fast = fast->next;
      }
      return slow;  // claimed start of the cycle
    }
  }
  return nullptr;
}
```

The exercise: decide whether this program is correct, and justify your answer.
