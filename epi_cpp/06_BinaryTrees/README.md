# Binary Trees

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 6. See epilight_cpp_new.pdf for the full text.*

## Overview

A binary tree is defined recursively: it is either empty, or a root node with two disjoint binary trees hanging off it — the left and right subtrees. That recursive definition is the single most important fact in the chapter, because it means nearly every binary-tree algorithm is a short recursive function whose shape mirrors the definition itself.

Binary trees matter in interviews for two reasons. Practically, they underlie binary search trees (sorted key storage) and are the natural model for any hierarchical data. Pedagogically, they are the cleanest setting for testing whether a candidate can design recursions, reason about what information must flow up or down the tree, and analyze space that lives on the call stack rather than in explicit allocations.

A minimal node type:

```cpp
template <typename T>
struct BinaryTreeNode {
    T data;
    unique_ptr<BinaryTreeNode<T>> left, right;
    // Some designs also carry: BinaryTreeNode<T>* parent;
};
```

### Terminology

- **Parent / child:** if node `p`'s left (right) subtree is rooted at `c`, then `c` is the left (right) child of `p`, and `p` is `c`'s parent. Every node except the root has exactly one parent. Node objects often carry a `parent` pointer (null at the root), though not always.
- **Search path:** the unique chain of nodes from the root down to a given node, each the child of the one before.
- **Ancestor / descendant:** `a` is an ancestor of `d` when `a` lies on the root-to-`d` search path; `d` is then a descendant of `a`. By the usual convention a node is both an ancestor and a descendant of itself.
- **Leaf:** a node whose only descendant is itself, i.e. it has no children. Careful: a node with exactly *one* child is **not** a leaf.
- **Depth** of a node: how many ancestors it has other than itself (the root has depth 0). **Height** of a tree: the largest depth present. A **level** is the set of all nodes sharing one depth.

### Tree shapes

- **Full:** every non-leaf node has exactly two children. In a full tree, the number of internal nodes is exactly one less than the number of leaves (a quick induction).
- **Perfect:** full, and all leaves sit at the same depth. A perfect tree of height h has 2^(h+1) − 1 nodes, of which 2^h are leaves.
- **Complete:** every level is packed except possibly the deepest, whose nodes are flushed left. A complete tree with n nodes has height ⌊log₂ n⌋. (Beware: some texts swap the words "complete" and "perfect.")
- **Skewed:** every node has at most one child, all on the same side (left-skewed / right-skewed). A skewed tree is effectively a linked list: height n − 1 for n nodes. Skewed trees are the worst case for anything whose cost depends on height.

### Traversals

Visiting every node ("walking" the tree) comes in three depth-first flavors, named for where the root is processed relative to its subtrees:

- **Inorder** — left subtree, root, right subtree. (On a BST this yields sorted order.)
- **Preorder** — root, left subtree, right subtree. (Root first: good for copying/serializing.)
- **Postorder** — left subtree, right subtree, root. (Children first: good for computing values that depend on subtrees, or for deletion.)

Example — for the tree

```
        8
       / \
      4   12
     / \    \
    2   6    14
```

inorder gives 2, 4, 6, 8, 12, 14; preorder gives 8, 4, 2, 6, 12, 14; postorder gives 2, 6, 4, 14, 12, 8.

Implemented recursively, each traversal is O(n) time. The additional space is O(h) — h the height — because that is the deepest the call stack gets. So space is O(log n) for a balanced tree but O(n) for a skewed one. If nodes carry parent pointers, all three traversals can be done iteratively with O(1) additional space.

(The word "tree" is heavily overloaded in computer science — rooted vs. free trees, binary vs. n-ary, tries, etc. — so confirm which variant a problem means.)

## Boot camp

The fastest way to internalize binary trees is to write the three traversals yourself. A single recursive function can display all three orders at once, since they differ only in *when* the root is handled relative to the two recursive calls:

```cpp
void Walk(const unique_ptr<BinaryTreeNode<int>>& node) {
    if (!node) return;

    cout << "pre:   " << node->data << '\n';   // before either subtree
    Walk(node->left);
    cout << "in:    " << node->data << '\n';   // between the subtrees
    Walk(node->right);
    cout << "post:  " << node->data << '\n';   // after both subtrees
}
```

**Complexity:** O(n) time — each node is touched a constant number of times. Although the function allocates nothing, the recursion descends h + 1 frames deep, so the space complexity is O(h): between O(log n) (complete/balanced) and O(n) (skewed). Reporting the call-stack space, unprompted, is itself a small interview signal.

## Top tips

- Trees are the textbook home of recursion: express the answer for a node in terms of the answers for its children, and the code writes itself. (Brush up on recursion fundamentals if needed — see the recursion chapter.)
- Always count implicit call-stack space in your space analysis; "no allocations" does not mean O(1) space.
- Many tree problems have an obvious solution using O(n) auxiliary storage and a subtler one that threads through the existing nodes with O(1) extra space — ask yourself whether the scratch structure is truly necessary.
- Sanity-check complexity claims against skewed trees: O(h) means O(log n) only when the tree is balanced; on a skewed tree it degrades to O(n).
- If the node type includes a parent pointer, use it — it frequently removes the need for an explicit stack or for passing state down the recursion, simplifying code and cost.
- A recurring off-by-one trap: treating a node with a single child as if it were a leaf. Test both child pointers.

## 6.1 Test if a binary tree is height-balanced

A tree is height-balanced when, at *every* node, the heights of the left and right subtrees differ by at most 1. Write a function that takes the root and decides whether the tree is height-balanced. (Perfect and complete trees qualify automatically, but so do many irregular trees — balance is a local condition at each node, not a statement about overall shape.)

*Hint: a classic traversal, slightly enriched.*

**Approach 1 — compute all heights, then check.** Compute the height of every node (bottom-up), storing results in a hash table keyed by node, or in an extra field. Then verify at each node that the two child heights differ by at most 1. This works and is O(n) time, but it retains every node's height simultaneously — O(n) auxiliary storage — which turns out to be unnecessary.

**Approach 2 — one postorder pass, constant data per frame.** The key observation: once a subtree has been fully examined, the only facts its parent needs are (a) whether it was balanced and (b) its height. Nothing about its interior matters anymore. So run a postorder traversal in which every call returns exactly that pair; each node combines its children's pairs in O(1). Two bonuses: the moment any subtree reports "unbalanced," the whole computation can short-circuit without visiting the remaining tree, and no per-node storage survives the visit.

```cpp
struct BalanceInfo {
    bool balanced;
    int height;   // meaningful only when balanced
};

BalanceInfo CheckBalance(const unique_ptr<BinaryTreeNode<int>>& node) {
    if (!node) return {true, -1};                     // empty tree: height -1

    BalanceInfo left = CheckBalance(node->left);
    if (!left.balanced) return {false, 0};            // prune: skip right subtree

    BalanceInfo right = CheckBalance(node->right);
    if (!right.balanced) return {false, 0};

    return {abs(left.height - right.height) <= 1,
            max(left.height, right.height) + 1};
}

bool IsHeightBalanced(const unique_ptr<BinaryTreeNode<int>>& root) {
    return CheckBalance(root).balanced;
}
```

**Worked example.** Consider:

```
        a
       / \
      b   c
     /
    d
   /
  e
```

Postorder reaches `e` first: both children empty (height −1 each), so `e` is balanced with height 0. At `d`: left height 0, right height −1 — difference 1, balanced, height 1. At `b`: left height 1, right height −1 — difference 2, so `b` reports *unbalanced*, and the answer `false` propagates straight up through `a` without `c` ever being visited.

**Complexity:** O(n) time — it is a postorder traversal, possibly cut short by early termination. Space is O(h) for the recursion stack (the frames alive at any moment correspond to one root-to-node path), and each frame holds only a constant-size pair. No hash table, no node mutation.

**Variants:**
- Return the size (node count) of the largest subtree that is *complete*.
- Call a node k-balanced when the node counts of its left and right subtrees differ by at most k. Given a tree and k, find a node that is **not** k-balanced although every one of its descendants is. (Same skeleton: postorder, but each call returns subtree *size* instead of height, and the first node found violating the bound is recorded.)
