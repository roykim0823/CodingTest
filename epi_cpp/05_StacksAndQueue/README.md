# Stacks and Queues

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 5. See epilight_cpp_new.pdf for the full text.*

## Overview

Stacks and queues are the two fundamental restricted-access sequences. A stack exposes only one end (last-in, first-out); a queue feeds in at one end and drains from the other (first-in, first-out). Their restricted APIs are exactly what makes them powerful: choosing one over the other encodes an ordering discipline directly into the data structure, so the algorithm built on top stays simple.

In interviews they appear in two ways. Most often they are internal machinery — a stack driving a parser or an iterative tree walk, a queue driving a breadth-first traversal. But they also make good standalone design problems, typically of the form "implement this container with an extra operation at better-than-obvious cost" (5.1 below is the canonical example). Both directions reward the same two skills: recognizing when LIFO or FIFO order is the natural fit, and knowing how to augment a basic container with cached metadata.

### Stack fundamentals

A stack supports `push` (insert) and `pop` (remove the most recently inserted element), often plus `peek`/`top` (inspect without removing). Popping an empty stack is an error condition — libraries variously throw, return a sentinel, or leave behavior undefined, so defensive `empty()` checks matter.

Cost model:

- Linked-list backing: `push` and `pop` are true O(1).
- Fixed-size array backing: O(1), but with a hard capacity.
- Dynamically resized array (what `std::stack` over `std::deque`/`std::vector` effectively gives you): amortized O(1) per operation, since occasional reallocations are paid for by the cheap operations between them.

### Queue and deque fundamentals

A queue supports `enqueue` (insert at the tail/back) and `dequeue` (remove from the head/front); the element that has waited longest leaves first. Common API extensions are non-destructive peeks at the front and at the back. With a linked list (or a circular buffer) both core operations are O(1).

A **deque** (double-ended queue) generalizes both structures: insertion and deletion are allowed at either end. Terminology varies by author — front-insert is sometimes called *push* and back-insert *inject*; front-delete *pop* and back-delete *eject* — but in C++ you will just use `push_front`/`push_back`/`pop_front`/`pop_back`. A deque can impersonate a stack (use one end) or a queue (use both ends in one direction).

## Boot camp

### Stacks: reverse iteration over a one-way sequence

A singly linked list can only be walked forward. To print it back-to-front without modifying it, exploit the stack's reversal property: push every value during one forward pass, then pop everything — values come back out in reverse order of insertion.

```cpp
void PrintReversed(shared_ptr<ListNode<int>> node) {
    stack<int> pending;
    for (; node; node = node->next) {
        pending.push(node->data);
    }
    while (!pending.empty()) {
        cout << pending.top() << '\n';
        pending.pop();
    }
}
```

**Complexity:** O(n) time and O(n) extra space for a list of n nodes. The general lesson: whenever a data source can only be consumed forward but must be processed backward, a stack converts one order into the other at the price of linear space.

### Queues: extending a library container by composition

Suppose you need the ordinary queue API plus a `Max()` query. Rather than writing a queue from scratch or inheriting from a container (fragile — standard containers are not designed as base classes), *compose*: keep a library container as a private member and forward the standard operations to it, adding the new query alongside.

```cpp
class QueueWithMax {
public:
    void Enqueue(int x) { items_.push_back(x); }

    int Dequeue() {
        if (items_.empty()) throw length_error("Dequeue on empty queue");
        int front = items_.front();
        items_.pop_front();
        return front;
    }

    int Max() const {
        if (items_.empty()) throw length_error("Max on empty queue");
        return *max_element(items_.begin(), items_.end());
    }

private:
    list<int> items_;
};
```

**Complexity:** `Enqueue`/`Dequeue` inherit the underlying container's O(1); this straightforward `Max` is O(n). (A cleverer design — a monotonic auxiliary deque — gets amortized O(1) for all three, using the same caching philosophy as problem 5.1; the point of the boot camp version is the composition pattern itself.)

## Top tips

- Train yourself to spot LIFO structure in a problem: nested or recursive input (expressions, brackets, directory paths), "undo the most recent thing first," and reverse-order processing all point at a stack. Parsing problems in particular almost always want one.
- Train yourself to spot FIFO structure: whenever elements must be served in arrival order — buffering, scheduling, breadth-first exploration — a queue is the natural fit.
- Don't stop at the basic API. Stacks and queues are easy to *augment*: attaching a little extra state per element (or a small auxiliary stack/deque) can turn an O(n) query like "current maximum" into O(1).

## Know your stack and queue libraries

`std::stack` (header `<stack>`) — an adaptor over a sequence container:

- `push(e)` / `emplace(args...)` — insert at the top.
- `top()` — read the top element **without** removing it.
- `pop()` — remove the top element; note it returns `void`, so read `top()` first if you need the value.
- `empty()`, `size()` — always guard `top()`/`pop()` with `empty()`; calling them on an empty stack is invalid.

`std::queue` (header `<queue>`):

- `push(e)` / `emplace(args...)` — insert at the back.
- `front()` / `back()` — read (not remove) the head / tail element.
- `pop()` — remove the front element; again returns `void`.
- Same rule: `front()`, `back()`, `pop()` must not be called on an empty queue.

`std::deque` (header `<deque>`) — the double-ended workhorse, and itself a full random-access container:

- `push_front(e)` / `emplace_front(args...)` and `push_back(e)` / `emplace_back(args...)`.
- `pop_front()` and `pop_back()`.
- `front()` and `back()`.

## 5.1 Implement a stack with max API

Design a stack supporting `Push`, `Pop`, and additionally `Max`, which reports the largest value currently stored anywhere in the stack.

*Hint: spend extra memory to remember maximum information.*

**Approach 1 — scan on demand.** Keep an ordinary stack; when `Max` is called, walk all elements and take the largest. No extra state, but `Max` costs O(n) time (O(1) extra space). Fine if `Max` is rare; bad if it's frequent.

**Approach 2 — auxiliary ordered structure.** Maintain, alongside the stack, a BST or heap of the live values (plus a hash table to locate entries for deletion on pop). `Max` drops to O(log n) — or O(1) for the peek itself — but every push/pop now pays O(log n), the space is O(n), and the bookkeeping is genuinely fiddly. Worth knowing as a pattern, rarely worth writing.

**Approach 3 — one running maximum? Broken.** A single variable `M` updated as `M = max(M, x)` on push handles pushes in O(1), but the scheme collapses on pop: the moment the element equal to `M` leaves, nothing tells you the second-largest, and you're back to scanning. The failure is instructive — one summary value can't be "unwound."

**Approach 4 — cache the max per element.** The unwinding problem disappears if every stack entry remembers the maximum of the stack *up to and including itself*. Pushing computes the new cached max in O(1) from the top entry's cache; popping simply discards the top pair, and the exposed entry's cache is automatically correct.

```cpp
class MaxStack {
public:
    bool Empty() const { return entries_.empty(); }

    int Max() const {
        if (Empty()) throw length_error("Max on empty stack");
        return entries_.top().running_max;
    }

    int Pop() {
        if (Empty()) throw length_error("Pop on empty stack");
        int value = entries_.top().value;
        entries_.pop();
        return value;
    }

    void Push(int x) {
        entries_.push({x, Empty() ? x : max(x, entries_.top().running_max)});
    }

private:
    struct Entry { int value, running_max; };
    stack<Entry> entries_;
};
```

**Complexity:** all three operations O(1); extra space O(n) unconditionally (one cached int per element).

**Approach 5 — auxiliary stack of (max, count).** Approach 4 wastes memory when the max rarely changes: long runs of elements share the same cached max. Observe that a newly pushed element smaller than the current max can never *become* the max, so it needs no record at all. Keep a second stack containing only the strictly increasing sequence of maxima, each paired with a multiplicity count (the count is what makes duplicates of the max safe).

- `Push(x)`: if `x` exceeds the top max, push `{x, 1}`; if equal, increment its count; if smaller, do nothing to the auxiliary stack.
- `Pop()`: if the popped value equals the top max, decrement its count, and pop the auxiliary entry when the count hits zero.
- `Max()`: read the top of the auxiliary stack.

```cpp
class MaxStack2 {
public:
    bool Empty() const { return data_.empty(); }

    int Max() const {
        if (Empty()) throw length_error("Max on empty stack");
        return maxes_.top().value;
    }

    int Pop() {
        if (Empty()) throw length_error("Pop on empty stack");
        int value = data_.top();
        data_.pop();
        if (value == maxes_.top().value && --maxes_.top().count == 0) {
            maxes_.pop();
        }
        return value;
    }

    void Push(int x) {
        data_.push(x);
        if (maxes_.empty() || x > maxes_.top().value) {
            maxes_.push({x, 1});
        } else if (x == maxes_.top().value) {
            ++maxes_.top().count;
        }
    }

private:
    struct MaxCount { int value, count; };
    stack<int> data_;
    stack<MaxCount> maxes_;
};
```

**Worked example.** Push 7, 3, 7, 9, then pop twice:

| operation | data stack (top right) | aux stack (top right) | Max() |
|---|---|---|---|
| push 7 | 7 | (7,1) | 7 |
| push 3 | 7 3 | (7,1) | 7 |
| push 7 | 7 3 7 | (7,2) | 7 |
| push 9 | 7 3 7 9 | (7,2)(9,1) | 9 |
| pop → 9 | 7 3 7 | (7,2) | 7 |
| pop → 7 | 7 3 | (7,1) | 7 |

**Complexity:** operations remain O(1). Worst-case extra space is still O(n) (a strictly increasing push sequence forces an auxiliary entry per element), but when maxima change rarely or duplicate heavily, the auxiliary stack stays tiny — O(1) in the best case.

## 5.2 Compute binary tree nodes in order of increasing depth

Given a binary tree, return the keys grouped by depth: a list of levels, shallowest first, with each level's keys ordered left to right. (This is level-order, or breadth-first, traversal.)

*Hint: try it with two queues before optimizing.*

**Approach 1 — annotate and sort.** Do any depth-first traversal that tracks the current depth, recording `(depth, key)` pairs. A preorder walk visits left children before right children, so within one depth the pairs already appear left-to-right; a *stable* sort keyed on depth then produces the required grouping. Correct, but the sort makes it O(n log n) time with O(n) space — and it ignores how much order the tree already hands you.

**Approach 2 — process level by level with queues.** Breadth-first order *is* the required order, so no sorting or depth labels are needed. Hold the current level's nodes in a queue; as you drain it, collect their keys and enqueue their children into a second queue, which becomes the next level. (Equivalently: one queue, processed in batches whose size is recorded before each level starts.)

```cpp
vector<vector<int>> LevelOrder(const unique_ptr<BinaryTreeNode<int>>& root) {
    vector<vector<int>> levels;
    if (!root) return levels;

    queue<BinaryTreeNode<int>*> current;
    current.push(root.get());
    while (!current.empty()) {
        queue<BinaryTreeNode<int>*> next;
        vector<int> keys;
        while (!current.empty()) {
            BinaryTreeNode<int>* node = current.front();
            current.pop();
            keys.push_back(node->data);
            if (node->left)  next.push(node->left.get());
            if (node->right) next.push(node->right.get());
        }
        levels.push_back(move(keys));
        current = move(next);
    }
    return levels;
}
```

**Worked example.** For the tree below, the queue holds {8}, then {4, 12}, then {2, 6, 14}, producing `[[8], [4, 12], [2, 6, 14]]`:

```
        8
       / \
      4   12
     / \    \
    2   6    14
```

**Complexity:** every node is enqueued and dequeued exactly once, so O(n) time. Space is O(m) where m is the maximum number of nodes on any one level — up to about n/2 for a bushy tree, O(1) for a skewed one.

**Variants:**
- Zigzag level order: alternate left-to-right and right-to-left per level (reverse alternate levels, or append to a deque from alternating ends).
- Bottom-up level order: emit the deepest level first (build top-down, then reverse the outer list).
- Return the average of the keys at each level rather than the keys themselves.
