# Binary Search Trees

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 11. See epilight_cpp_new.pdf for the full text.*

## Overview

The BST is the "do everything reasonably well" data structure: it can solve almost any data-structure problem with decent efficiency. Beyond plain key lookup, a BST supports operations a hash table fundamentally cannot: find the minimum or maximum, find the successor or predecessor of a query value (which need not itself be stored), and enumerate all keys in a range in sorted order. Think of it as a sorted array that also supports efficient insertion and deletion. This chapter covers the BST property and its subtleties, the cost model, the `std::set` / `std::map` APIs, and — in the sampler — one problem: verifying that a binary tree actually is a BST, developed through four progressively better approaches.

### Core facts

- **The BST property:** at every node, the stored key is greater than or equal to *every* key in the node's left subtree and less than or equal to *every* key in its right subtree. A BST is just a binary tree (Chapter 6) whose nodes hold comparable keys (integers, strings, ...) respecting this invariant.
- Lookup, insert, and delete each cost O(h), where h is the tree height. Naive insertion/deletion can degenerate the tree to a chain, making h — and thus every operation — O(n). Height-balanced implementations (red-black trees being the standard example, and what library containers use) maintain h = O(log n) at the price of extra bookkeeping data in each node.
- Both BSTs and hash tables consume O(n) space (the BST slightly more in practice, due to pointers and balance metadata). The trade: hash tables win on raw speed for point operations; BSTs add ordering operations, all at O(log n).
- **The mutable-key trap** (same as with hash tables): mutating an object while it sits in a BST breaks the invariant that located it, so later lookups fail even though the object is still in the tree. Remove first, then mutate, then re-insert — or better, keep keys immutable.

A minimal node prototype, using `unique_ptr` for ownership:

```cpp
template <typename T>
struct BstNode {
  T data;
  std::unique_ptr<BstNode<T>> left, right;
};
```

## Boot camp

Search is the most fundamental BST operation, and it shows off how naturally recursion fits tree structure: at each node, the BST property tells you which single subtree can possibly contain the query, so you descend one level per step.

```cpp
BstNode<int>* SearchBst(const std::unique_ptr<BstNode<int>>& node, int key) {
  if (node == nullptr) {
    return nullptr;                       // key absent
  }
  if (node->data == key) {
    return node.get();                    // found it
  }
  return key < node->data ? SearchBst(node->left, key)
                          : SearchBst(node->right, key);
}
```

Each call moves one level down and does O(1) work, so the time complexity is O(h) — which is O(log n) for a balanced tree and O(n) for a degenerate one. (Contrast with searching an arbitrary binary tree, where you might have to visit every node.)

## Top tips

- Sorted iteration is free: an inorder traversal of a BST yields the keys in sorted order in O(n) time, whether or not the tree is balanced.
- BST + hash table is a powerful combination. Example: students ordered by GPA in a BST, but updates arrive keyed by student name. The BST alone would need a full traversal to find a name; an auxiliary hash table mapping name → tree entry gives direct access, after which you can remove, update, and re-insert in the tree.
- The BST property is *global*, not local. A tree can satisfy "each node's key is greater than its left child's and less than its right child's" at every single node and still not be a BST — the violation can be between a node and a distant descendant. (This is exactly the trap in Problem 11.1.)

## Know your binary search tree libraries

- The BST-backed containers are `set` (keys) and `map` (key–value pairs) — the ordered counterparts of `unordered_set` / `unordered_map`, with the same core insert/erase/find/size API plus ordering operations.
- What `set` adds over `unordered_set` (and `map` over `unordered_map`, analogously):
  - `begin()` iterates keys in ascending order; `rbegin()` in descending order.
  - `*begin()` / `*rbegin()` are the smallest / largest keys.
  - `lower_bound(k)` returns the first element ≥ k; `upper_bound(k)` the first element > k.
  - `equal_range(k)` returns the pair of iterators bounding the elements equal to k.
- All of these run in O(log n) — they are backed by the underlying balanced tree, not by hashing.
- The constructors accept an explicit comparator object to define the key order; be comfortable writing one (same lambda/functor syntax as for `std::sort`).

## 11.1 Test if a binary tree satisfies the BST property

Given the root of an arbitrary binary tree with integer keys, determine whether it satisfies the BST property.

*Hint: is it enough to check, at each node, that the key is ≥ the left child's key and ≤ the right child's key?*

(The hint's answer is no — that local check misses violations between a node and a deeper descendant, as the example below shows.)

**Running example.** Root 10; left child 5 with children 2 and 12; right child 15. Every parent–child pair looks fine locally (2 < 5 < 12, and 5 < 10 < 15), but 12 lives in 10's *left* subtree while exceeding 10 — not a BST.

**Approach 1 — recompute subtree extremes, O(n²).** Straight from the definition: at each node, compute the maximum key in its left subtree and the minimum key in its right subtree, check them against the node's key, and recurse into both children. Computing a subtree minimum must consider the root's key and both subtrees' minima (in a general binary tree the minimum can be anywhere). The flaw is repeated traversal of the same subtrees from every ancestor: on a left-skewed chain this hits O(n²). Caching each node's subtree min/max fixes the time back to O(n), but costs O(n) extra storage for the cache.

**Approach 2 — propagate valid ranges top-down, O(n) time, O(h) space.** Invert the direction of information flow: instead of asking each subtree for its extremes, tell each subtree what range it is allowed to occupy. The root may hold anything in [−∞, +∞]. If a subtree must lie within [lo, hi] and its root holds w (which must itself be in [lo, hi], else we have found a violation), then the left subtree must lie within [lo, w] and the right within [w, hi]. Each node is visited once with an O(1) check.

```cpp
bool KeysInRange(const std::unique_ptr<BstNode<int>>& node, int lo, int hi) {
  if (node == nullptr) {
    return true;
  }
  if (node->data < lo || node->data > hi) {
    return false;
  }
  return KeysInRange(node->left, lo, node->data) &&
         KeysInRange(node->right, node->data, hi);
}

bool IsBst(const std::unique_ptr<BstNode<int>>& root) {
  return KeysInRange(root, std::numeric_limits<int>::min(),
                     std::numeric_limits<int>::max());
}
```

On the running example: node 10 gets [−∞, ∞]; node 5 gets [−∞, 10]; node 12 gets [5, 10] and 12 > 10 → **false**. Time O(n); space O(h) for the recursion stack.

**Approach 3 — inorder traversal is sorted, O(n) time.** An inorder walk of a BST visits keys in nondecreasing order, and conversely a binary tree whose inorder sequence is sorted must be a BST (both directions follow directly from the definitions). So walk inorder, remembering the previously visited key, and fail the moment the previous key exceeds the current one.

```cpp
bool InorderIsSorted(const std::unique_ptr<BstNode<int>>& node,
                     const BstNode<int>** prev) {
  if (node == nullptr) {
    return true;
  }
  if (!InorderIsSorted(node->left, prev)) {
    return false;
  }
  if (*prev && (*prev)->data > node->data) {
    return false;  // predecessor exceeds current key
  }
  *prev = node.get();
  return InorderIsSorted(node->right, prev);
}

bool IsBst(const std::unique_ptr<BstNode<int>>& root) {
  const BstNode<int>* prev = nullptr;
  return InorderIsSorted(root, &prev);
}
```

On the running example the inorder sequence begins 2, 5, 12, 10 — the step 12 → 10 descends, so **false**. Time O(n); space O(h).

**Approach 4 — BFS with bounds, for early detection.** Approaches 2 and 3 both dive into the left subtree first, so even a violation sitting right next to the root (say, a right child smaller than the root) isn't noticed until the entire left subtree has been explored. Searching in breadth-first order fixes this: keep a queue of (node, lower bound, upper bound) entries, seeded with the root and [−∞, +∞]. Pop an entry, check its constraint (fail immediately on violation), and enqueue its children with tightened bounds — left child inherits [lo, node key], right child [node key, hi]. Because entries are processed by depth and each carries the tightest possible bounds, a violation among nodes at depth ≤ d is found without ever touching depth d+1.

```cpp
bool IsBst(const std::unique_ptr<BstNode<int>>& root) {
  struct Entry {
    const BstNode<int>* node;
    int lo, hi;
  };
  std::queue<Entry> q;
  q.push({root.get(), std::numeric_limits<int>::min(),
          std::numeric_limits<int>::max()});

  while (!q.empty()) {
    Entry e = q.front();
    q.pop();
    if (e.node == nullptr) {
      continue;
    }
    if (e.node->data < e.lo || e.node->data > e.hi) {
      return false;
    }
    q.push({e.node->left.get(), e.lo, e.node->data});
    q.push({e.node->right.get(), e.node->data, e.hi});
  }
  return true;
}
```

On the running example the queue evolves: (10, [−∞, ∞]) → (5, [−∞, 10]), (15, [10, ∞]) → after popping 5: (2, [−∞, 5]), (12, [5, 10]) → 15 and 2 pass → popping (12, [5, 10]) fails → **false**, having visited only depths 0–2. Time O(n) worst case; space O(n) for the queue (up to a whole level of the tree).

**Complexity summary.** Approach 1: O(n²) time (or O(n) time with O(n) cache). Approaches 2 and 3: O(n) time, O(h) space. Approach 4: O(n) time, O(n) space, but strictly better latency when a violation occurs at small depth.
