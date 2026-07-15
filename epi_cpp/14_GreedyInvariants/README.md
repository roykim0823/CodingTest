# Greedy Algorithms and Invariants

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 14. See epilight_cpp_new.pdf for the full text.*

## Overview

A greedy algorithm assembles a solution one decision at a time, always taking the choice that looks best right now and never revisiting it. When a greedy rule can be proven optimal, the resulting algorithm is usually short, fast, and easy to implement — which is exactly why interviewers like these problems: the code is trivial, the insight is not. The chapter's second theme, **invariants**, is a complementary design tool: identify a condition that provably holds at every step of the algorithm (typically "the answer, if it exists, still lies inside this shrinking region"), and use it to discard candidates wholesale. The two ideas often appear together: a greedy pointer movement is justified by an invariant argument.

A cautionary example on greedy choice design. Suppose 2n depots sit on a line, n of type A and n of type B, and we must pair each A with a distinct B so that the total length of *road* needed to connect all pairs is minimized — where overlapping connections share pavement (the cost is the length of the union of the connecting intervals). The tempting rule "for each A, grab the nearest free B" fails: with A-depots at 0 and 4 and B-depots at 3 and 6, processing the A at 4 first pairs it with the B at 3, forcing the pair (0, 6); the union of intervals [3,4] and [0,6] has length 6. The crossing-free pairing (0,3) and (4,6) needs only length 5. A subtler rule *is* optimal: sweep all depots left to right, and pair each unpaired depot with the nearest unpaired depot of the opposite type. The leftmost depot's pairing can always be exchanged into an optimal solution without increasing cost, and induction on the remaining depots completes the proof. Moral: several greedy rules may suggest themselves; only some are correct, and the correct one is not always the obvious one.

## Boot camp

### Greedy: making change

With US coin denominations {100, 50, 25, 10, 5, 1}, the minimum-coin way to make any amount is to take as many of the largest coin as fits, then recurse on the remainder — and never reconsider. That irrevocability is the signature of a greedy algorithm.

```cpp
int MinCoins(int cents) {
  static const int kDenoms[] = {100, 50, 25, 10, 5, 1};
  int count = 0;
  for (int d : kDenoms) {
    count += cents / d;
    cents %= d;
  }
  return count;
}
```

Example: 73 cents → 50 + 10 + 10 + 1 + 1 + 1 = six coins. Six loop iterations of constant work each: **O(1) time**. (Note the optimality depends on the denomination set — with coins {1, 3, 4} and amount 6, greedy gives 4+1+1 but 3+3 is better; arbitrary denominations require DP.)

### Invariants

An invariant is a statement about program state that holds before and after every iteration. Classic illustrations:

- **Binary search** maintains: "the target, if present, lies within the current candidate range." Every halving step preserves it, so when the range empties, absence is proven.
- **Selection sort** maintains: "the prefix processed so far is sorted, each of its elements is ≤ every unprocessed element, and the whole array is still a permutation of the input." Termination of the loop instantly yields correctness.

### Invariants: two-sum in a sorted array

Given a sorted array and a target K, decide whether two entries sum to K. Example: in `[-3, 0, 2, 5, 9, 11]`, target 11 works (2 + 9) but target 1 does not.

- **Brute force:** double loop over all pairs — O(n^2) time, O(1) space.
- **Hash table:** insert all elements; for each `e` probe for `K - e` — O(n) time but O(n) extra space, and it ignores the sortedness we were given.
- **Invariant / two pointers:** maintain the invariant that *if any pair sums to K, both its elements lie in the current index window [i, j]*. Start with the full array. If `A[i] + A[j] < K`, then `A[i]` plus even the largest available partner is too small, so `A[i]` can never participate — shrink from the left. Symmetrically shrink from the right when the sum is too large.

```cpp
bool HasTwoSum(const std::vector<int>& A, int K) {
  int i = 0, j = (int)A.size() - 1;
  while (i <= j) {                 // i == j allowed: an entry may pair with itself
    int s = A[i] + A[j];
    if (s == K) return true;
    if (s < K) ++i; else --j;
  }
  return false;
}
```

**O(n) time, O(1) space.** Trace with target 11 on the example: (-3,11)→8 low, (0,11)→11 hit.

## Top tips

- Greedy is a natural fit for optimization problems presenting a clear menu of choices at each step; its hallmark is that decisions are final.
- Design the greedy strategy recursively ("choose, then solve the rest"), but implement it as a loop for performance.
- A greedy algorithm that is *not* optimal still earns its keep as a heuristic or as a stepping stone toward the real optimum.
- Do not trust the first greedy rule that comes to mind — the correct rule can be non-obvious, and small examples expose bad rules quickly.
- To find an invariant, experiment with small inputs by hand and hypothesize what stays true as the algorithm advances.
- The most common invariant in interview problems is geometric: a subarray/window of the input guaranteed to contain the solution, shrunk from the ends using order properties.

## 14.1 The 3-sum problem

Given an array of integers and a target t, determine whether some three entries — the *same entry may be reused* — sum exactly to t. Fresh example: for `A = [4, 9, 1, 6, 3]` and t = 19, yes (4 + 6 + 9); for t = 2, no (the smallest achievable triple sum is 1 + 1 + 1 = 3).

*Hint: for each candidate entry a, the question becomes whether two entries sum to t - a.*

**Approach 1 — brute force.** Three nested loops over all index triples: **O(n^3) time, O(1) space**. Correct but wasteful — the inner two loops solve a two-sum instance from scratch with no structure.

**Approach 2 — hash-assisted.** Store all elements in a hash set; for every pair `(A[i], A[j])` check membership of `t - A[i] - A[j]`: **O(n^2) time** but **O(n) extra space**.

**Approach 3 — sort + two-pointer two-sum (optimal here).** Sort A once. For each element `a`, run the invariant-based two-sum scan of the boot camp against target `t - a`. Reuse of an entry is naturally allowed because the two-sum scan permits `i == j` and the outer element is not excluded from the scan. Since sorting is a one-time O(n log n) cost and each of the n scans is O(n), the total is **O(n^2) time, O(1) extra space** — matching Approach 2's time with no extra memory.

```cpp
bool HasTwoSumSorted(const std::vector<int>& A, int K) {
  for (int i = 0, j = (int)A.size() - 1; i <= j;) {
    int s = A[i] + A[j];
    if (s == K) return true;
    s < K ? ++i : --j;             // invariant: any solution stays in [i, j]
  }
  return false;
}

bool HasThreeSum(std::vector<int> A, int t) {
  std::sort(A.begin(), A.end());
  return std::any_of(A.begin(), A.end(),
                     [&](int a) { return HasTwoSumSorted(A, t - a); });
}
```

**Worked example.** `A = [4, 9, 1, 6, 3]`, t = 19. Sorted: `[1, 3, 4, 6, 9]`. Outer element 1 → need 18: scans (1,9)=10 low, (3,9)=12 low, (4,9)=13 low, (6,9)=15 low, (9,9)=18 — hit; report true. (The found triple is 1 + 9 + 9, reusing 9; 4 + 6 + 9 would also qualify.)

**Complexity summary:** O(n^3)/O(1) brute force → O(n^2)/O(n) hashing → O(n^2)/O(1) sort + two pointers.

**Variants:**
- Require the three chosen indices to be pairwise distinct (entries may still be equal in value if they occur at different positions).
- Generalize to k-sum, where the number of summands k is part of the input.
- Closest 3-sum: return a distinct-index triple whose sum minimizes the absolute difference from a target T.
- Count the number of index triples (p, q, r) with `A[p] <= A[q] <= A[r]` whose sum is at most T.
