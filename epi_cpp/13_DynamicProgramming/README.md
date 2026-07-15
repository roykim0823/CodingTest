# Dynamic Programming

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 13. See epilight_cpp_new.pdf for the full text.*

## Overview

Dynamic programming is the technique of choice when a problem — optimization, counting, or search/decision — can be expressed in terms of answers to smaller instances of itself. The defining trait that separates DP from ordinary divide-and-conquer is *overlap*: the same subinstance shows up again and again down different branches of the recursion, so a naive recursive solution repeats work exponentially, while caching each subresult the first time it is computed collapses the running time to (typically) the number of distinct subproblems times the work per subproblem.

DP problems are a staple of hard interview rounds precisely because the recurrence is the creative step: once you can say "the answer for this instance is built from the answers for these smaller instances," the implementation is mechanical. Two skills matter in practice:

1. **Finding the right subproblem.** It is often *not* the obvious one. The decomposition must satisfy two conditions: the full answer is cheap to assemble once subanswers are known, and the subanswers are cacheable (finitely indexable).
2. **Managing the cache.** Top-down memoization uses a map keyed by the recursion arguments; bottom-up iteration uses an array filled in dependency order — and iteration frequently lets you *recycle* storage, because old entries stop being needed.

## Boot camp

### Fibonacci numbers

Definition: `F(0) = 0`, `F(1) = 1`, `F(n) = F(n-1) + F(n-2)`. The sequence begins 0, 1, 1, 2, 3, 5, 8, 13, 21, ... and appears in surprisingly many settings (population models, data-structure analysis, scheduling).

**Naive recursion — exponential.** Directly coding the recurrence re-derives the same values enormously many times: computing `F(5)` calls `F(3)` twice and `F(2)` three times, and the duplication compounds at every level, giving roughly `O(1.618^n)` calls.

**Memoized recursion — O(n) time, O(n) space.** Cache each `F(i)` on first computation; every value is then computed exactly once.

```cpp
long long FibMemo(int n, std::unordered_map<int, long long>& memo) {
  if (n <= 1) return n;
  auto it = memo.find(n);
  if (it != memo.end()) return it->second;
  return memo[n] = FibMemo(n - 1, memo) + FibMemo(n - 2, memo);
}
```

**Bottom-up — O(n) time, O(1) space.** Filling values in increasing order of `n` exposes the space optimization: only the previous two values are ever read, so the whole cache shrinks to two variables. Minimizing cache footprint like this is a recurring DP theme.

```cpp
long long Fib(int n) {
  long long prev = 0, curr = (n > 0);      // F(0), F(1)
  for (int i = 2; i <= n; ++i) {
    long long next = prev + curr;
    prev = curr;
    curr = next;
  }
  return n == 0 ? 0 : curr;
}
```

### Maximum subarray sum

Given an integer array, find the largest sum attainable by any contiguous subarray. Example (fresh values): for `A = [3, -7, 5, 2, -1, 4, -9]`, the best subarray is `[5, 2, -1, 4]` with sum 10.

A ladder of solutions:

- **Brute force, O(n^3):** try all `n(n+1)/2` subarrays and sum each one from scratch.
- **Prefix sums, O(n^2) time / O(n) space:** precompute `S[k] = A[0] + ... + A[k]`; then any subarray sum `A[i..j]` is `S[j] - S[i-1]` in O(1), leaving only the O(n^2) pair enumeration.
- **Divide and conquer, O(n log n):** split at the midpoint, solve both halves, and combine — the best subarray either lies wholly in one half or straddles the middle, in which case it is (best suffix of the left half) + (best prefix of the right half). Correct, but fiddly to code without off-by-one bugs.
- **DP, O(n) time / O(1) space.** The key insight: the naive subproblem "best subarray of `A[0..n-2]`" does *not* extend to `A[0..n-1]`, because the optimal subarray may end exactly at the new element. The subproblem that works is *the best subarray ending at index j*, which equals `S[j]` minus the smallest prefix sum over indices before `j`. Sweep once, maintaining the running sum and the minimum prefix sum seen so far:

```cpp
int MaxSubarraySum(const std::vector<int>& A) {
  int running = 0, min_prefix = 0, best = 0;
  for (int a : A) {
    running += a;
    min_prefix = std::min(min_prefix, running);
    best = std::max(best, running - min_prefix);
  }
  return best;  // 0 for empty array / all-negative arrays (empty subarray)
}
```

Walkthrough on `A = [3, -7, 5, 2, -1, 4, -9]`: running sums are 3, -4, 1, 3, 2, 6, -3; the minimum prefix before index 5 is -4 (after `A[1]`), so the best value is `6 - (-4) = 10`, found at index 5. Empty and all-negative inputs are handled without special cases.

## Top tips

- Think DP whenever solving the instance means *making choices*, and the value of a choice depends on solutions to smaller instances of the same problem.
- DP is not only for optimization: counting problems ("how many ways...") and decision problems ("is it possible...") fit whenever the answer recurses on smaller instances.
- Conceive the algorithm recursively, but consider building the cache bottom-up (iteratively) for efficiency — iteration avoids call-stack overhead and enables cache reuse.
- Cache choice follows implementation style: recursive memoization pairs naturally with a hash table or BST; iterative DP pairs with a 1-D or multi-D array.
- Reclaim cache space aggressively: once a set of entries can never be read again (e.g., the row before last in a grid DP), overwrite it.
- Resist the instinct to split the array into two halves as in quicksort/mergesort; for most DP problems, half-and-half subproblems don't carry enough information to reassemble the global answer. Peeling off one element or one row usually does.
- Verify that optimal substructure actually holds. Counterexample: the longest cycle-free path from city 1 to city 2 through city 3 does *not* decompose into longest 1→3 and 3→2 paths, since those pieces may reuse intermediate cities — DP is inapplicable there.

## 13.1 Count the number of ways to traverse a 2D array

Starting at the top-left cell of an n x m grid and moving only right or down, how many distinct paths reach the bottom-right cell? (For a 5 x 5 grid the answer is 70.)

*Hint: for i, j > 0, cell (i, j) is entered either from (i-1, j) or from (i, j-1).*

**Approach 1 — enumerate paths (brute force).** Recursively try both moves at every cell and count complete paths. The number of paths itself is exponential in the grid dimensions, so this blows up immediately; it also recomputes the count for the same cell across many different call chains.

**Approach 2 — memoized recurrence.** Let `W(i, j)` be the number of paths from the origin to `(i, j)`. Every path's final step arrives from above or from the left, and those two path sets are disjoint, so

```
W(i, j) = W(i-1, j) + W(i, j-1),   W(i, 0) = W(0, j) = 1.
```

Cache each `W(i, j)` in a matrix so it is computed once.

```cpp
int CountPaths(int i, int j, std::vector<std::vector<int>>& memo) {
  if (i == 0 && j == 0) return 1;
  if (memo[i][j] == 0) {
    int from_above = i > 0 ? CountPaths(i - 1, j, memo) : 0;
    int from_left  = j > 0 ? CountPaths(i, j - 1, memo) : 0;
    memo[i][j] = from_above + from_left;
  }
  return memo[i][j];
}

int NumWays(int n, int m) {
  std::vector<std::vector<int>> memo(n, std::vector<int>(m, 0));
  return CountPaths(n - 1, m - 1, memo);
}
```

**Complexity:** O(nm) time and O(nm) space — each cell is filled once at O(1) cost.

**Approach 3 — bottom-up with a rolling row.** Filling row by row, only the previous row is ever consulted, and in fact updating a single row in place works: before the update `row[j]` holds `W(i-1, j)` and `row[j-1]` already holds `W(i, j-1)`.

```cpp
int NumWays(int n, int m) {
  if (m < n) std::swap(n, m);              // keep the shorter dimension
  std::vector<long long> row(m, 1);        // top row: one way everywhere
  for (int i = 1; i < n; ++i)
    for (int j = 1; j < m; ++j)
      row[j] += row[j - 1];
  return (int)row[m - 1];
}
```

**Complexity:** O(nm) time, O(min(n, m)) space.

**Approach 4 — closed form.** Every path is a shuffle of exactly `n-1` down-moves and `m-1` right-moves, so the count is the binomial coefficient `C(n+m-2, n-1)`. Computing it takes O(n + m) multiplications (mind overflow).

**Worked example (4 x 3 grid):** the table of `W` values is

```
1  1  1
1  2  3
1  3  6
1  4 10
```

so there are 10 paths — matching `C(4+3-2, 3) = C(5, 3) = 10`.

**Variants:**
- Compute the count using only O(min(n, m)) space (Approach 3).
- Count paths when some cells are blocked, given a boolean obstacle matrix (blocked cells get `W = 0`).
- Fisherman's grid: each cell holds a fish value; maximize the total value collected on a top-left → bottom-right down/right path (`best(i,j) = value(i,j) + max(best(i-1,j), best(i,j-1))`).
- Same fisherman problem, but the start and end cells are unconstrained and values may be negative (a 2-D relative of maximum subarray sum).
- Count *monotone* decimal numbers of length k — k-digit sequences (leading digit nonzero) whose digits never decrease. DP over (position, last digit used).
- Count *strictly monotone* decimal numbers of length k (digits strictly increasing); equivalently, choose k distinct digits from 1-9, so the DP answer equals `C(9, k)`.
