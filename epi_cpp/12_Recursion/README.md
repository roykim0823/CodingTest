# Recursion

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 12. See epilight_cpp_new.pdf for the full text.*

## Overview

Recursion solves a problem by expressing its solution in terms of solutions to smaller instances of related problems. It is the natural tool whenever the *input itself* is defined by recursive rules — grammars, trees, nested structures — and more broadly for search, enumeration, divide-and-conquer, and any decomposition of a complex problem into similar smaller pieces. Backtracking and branch-and-bound, the two systematic search paradigms, are both most naturally written recursively.

A correct recursive function needs exactly two ingredients:

1. **Base cases** — the smallest instances, solved directly without further calls.
2. **Progress** — every recursive call must move measurably toward a base case, so the recursion converges.

Interview-wise, recursion questions test whether you can spot the right decomposition (which sub-instance to recurse on), manage shared state across calls (the backtracking push/recurse/pop pattern), and reason about the size of the recursion tree for complexity analysis. The sampler contains one problem, generating the power set (12.1), which exercises all three.

## Boot camp

### Euclid's GCD

The greatest common divisor of x and y is the classic first recursion. The insight: any common divisor of x and y (with y > x) also divides y − x, so GCD(x, y) = GCD(x, y − x); iterating that subtraction all the way gives GCD(x, y) = GCD(x, y mod x). For instance GCD(273, 63): 273 mod 63 = 21, then 63 mod 21 = 0, so the answer is 21 — two steps.

```cpp
long long Gcd(long long x, long long y) {
  return y == 0 ? x : Gcd(y, x % y);
}
```

Every step replaces the pair (x, y) by (y, x mod y), and one can show an argument at least halves every step, so the number of calls is O(log max(x, y)) — equivalently O(n) in the bit-length n of the inputs. The call stack makes the space complexity O(n) too, but the function is *tail-recursive*, so it converts mechanically into a loop with O(1) space:

```cpp
long long GcdIterative(long long x, long long y) {
  while (y != 0) {
    long long r = x % y;
    x = y;
    y = r;
  }
  return x;
}
```

### Divide-and-conquer: tiling the mutilated chessboard

A *triomino* is an L-shaped tile of three unit squares. A *mutilated board* of side n is an n × n grid with one corner square removed; the 8 × 8 version has 63 squares, so exactly 21 triominoes could cover it, provided none overlap or stick out. The task: compute such a placement.

This is a lesson in choosing the induction hypothesis. The tempting hypothesis — "a tiling of the n × n mutilated board gives a tiling of the (n+1) × (n+1) one" — leads nowhere: the added L-shaped rim of 2n+1 squares has no useful relation to triominoes. The hypothesis that works is **"n × n tileable ⇒ 2n × 2n tileable."** Split the 2n × 2n mutilated board into four n × n quadrants. The quadrant containing the missing corner is an n × n mutilated board already. For the other three, pretend the corner cell touching the center of the big board is missing — those three "missing" cells form an L around the center, covered by one real triomino. Now all four quadrants are n × n mutilated boards, tileable by hypothesis; the base case is the 2 × 2 mutilated board, which *is* a triomino. So a tiling exists for every power-of-two side, and the proof is itself the algorithm:

```cpp
// Fills board (side = 2^k) with triomino ids; the hole at (hole_r, hole_c)
// stays 0. Each recursive call tiles one sub-board with one designated hole.
void TileMutilatedBoard(std::vector<std::vector<int>>& board, int top,
                        int left, int side, int hole_r, int hole_c,
                        int& next_id) {
  if (side == 1) {
    return;  // single cell: it is the hole, nothing to place
  }
  const int id = next_id++;      // the triomino spanning the center
  const int half = side / 2;
  for (int qr = 0; qr < 2; ++qr) {
    for (int qc = 0; qc < 2; ++qc) {
      const int r0 = top + qr * half, c0 = left + qc * half;
      int hr, hc;
      if (hole_r >= r0 && hole_r < r0 + half &&
          hole_c >= c0 && hole_c < c0 + half) {
        hr = hole_r, hc = hole_c;  // quadrant with the real hole
      } else {
        // Corner of this quadrant nearest the board center: cover it with
        // the central triomino and treat it as this quadrant's hole.
        hr = r0 + (qr == 0 ? half - 1 : 0);
        hc = c0 + (qc == 0 ? half - 1 : 0);
        board[hr][hc] = id;
      }
      TileMutilatedBoard(board, r0, c0, half, hr, hc, next_id);
    }
  }
}
```

Each call does O(1) work per quadrant plus four half-size subproblems, giving O(n²) total for an n × n board — linear in the number of cells, which is optimal since every cell must be written.

## Top tips

- Recursion is especially natural when the input is defined by recursive rules, such as a grammar.
- Search, enumeration, and divide-and-conquer are recursion's home turf.
- Use recursion in place of deeply nested loops when the nesting depth isn't fixed — e.g., splitting a string into k pieces for arbitrary k: k nested loops can't be written, but a recursion parameterized on k can.
- Asked to *remove* recursion? Simulate the call stack explicitly with a stack data structure.
- Tail-recursive functions need no stack at all — they convert directly to a while-loop (optimizing compilers do this automatically).
- If the same function can be invoked with the same arguments more than once, cache the results — that memoization step is the gateway to dynamic programming (Chapter 13).

## 12.1 Generate the power set

The power set of a set S is the collection of *all* its subsets, from the empty set up to S itself — 2^n of them for |S| = n, since each element is independently in or out. Write a function returning the power set of an input set (given as a vector of distinct elements).

*Hint: there are 2^n subsets of an n-element set, and 2^k k-bit words.*

**Approach 1 — recursive include/exclude.** Pick any element e. Every subset either contains e or doesn't, so the power set is (all subsets of S − {e}, each with e added) ∪ (all subsets of S − {e}). Recurse with a shared "selected so far" vector: at position i, first take `S[i]` (push, recurse on i+1, pop), then skip it (recurse on i+1 again); when i reaches n, the selection is one complete subset — emit it. This push/recurse/pop discipline is the backtracking template worth internalizing.

```cpp
void EnumerateSubsets(const std::vector<int>& s, size_t i,
                      std::vector<int>* selected,
                      std::vector<std::vector<int>>* out) {
  if (i == s.size()) {
    out->push_back(*selected);  // one complete subset
    return;
  }
  selected->push_back(s[i]);           // subsets containing s[i]
  EnumerateSubsets(s, i + 1, selected, out);
  selected->pop_back();                // subsets not containing s[i]
  EnumerateSubsets(s, i + 1, selected, out);
}

std::vector<std::vector<int>> GeneratePowerSet(const std::vector<int>& s) {
  std::vector<std::vector<int>> power_set;
  std::vector<int> selected;
  EnumerateSubsets(s, 0, &selected, &power_set);
  return power_set;
}
```

**Worked example.** S = {4, 7}. The call tree: include 4 → include 7 → emit {4, 7}; back up, exclude 7 → emit {4}; back to the top, exclude 4 → include 7 → emit {7}; exclude 7 → emit {}. Result: {4, 7}, {4}, {7}, {} — all 2² subsets, each exactly once.

**Complexity.** The number of calls obeys C(n) = 2·C(n−1), i.e., O(2^n) calls, and each emitted subset costs up to O(n) to copy, giving O(n·2^n) time. Returning everything costs O(n·2^n) space too (2^n subsets of average size n/2); if the subsets only need to be printed or streamed, the live state is just the recursion stack and the current selection — O(n).

**Approach 2 — count in binary.** Fix an ordering of S's elements. Then n-bit words correspond one-to-one with subsets: bit i set means element i is in. (E.g., for S = {p, q, r, s}, the word 1011₂ — bits 0, 1, and 3 — denotes {p, q, s}.) So when n doesn't exceed the machine word width, just count from 0 to 2^n − 1 and decode each integer's set bits. Extract the lowest set bit with `w & ~(w − 1)`, convert it to an index via log₂, and clear it with `w &= w − 1`.

```cpp
std::vector<std::vector<int>> GeneratePowerSet(const std::vector<int>& s) {
  std::vector<std::vector<int>> power_set;
  for (int word = 0; word < (1 << s.size()); ++word) {
    std::vector<int> subset;
    for (int w = word; w != 0; w &= w - 1) {         // clear lowest set bit
      int index = static_cast<int>(std::log2(w & ~(w - 1)));
      subset.push_back(s[index]);
    }
    power_set.push_back(std::move(subset));
  }
  return power_set;
}
```

**Worked example.** S = {4, 7}: word 00 → {}, word 01 → {4}, word 10 → {7}, word 11 → {4, 7}.

**Complexity.** Still O(n·2^n) time — O(n) per subset — but very fast in practice (no function-call overhead), and only O(n) space when subsets are consumed one at a time rather than collected.

**Variants:**
- The input may contain duplicates (a multiset); enumerate each distinct multiset-subset exactly once. E.g., for ⟨1, 2, 3, 2⟩ the answer has 12 subsets — ⟨⟩, ⟨1⟩, ⟨2⟩, ⟨3⟩, ⟨1,2⟩, ⟨1,3⟩, ⟨2,2⟩, ⟨2,3⟩, ⟨1,2,2⟩, ⟨1,2,3⟩, ⟨2,2,3⟩, ⟨1,2,2,3⟩ — not 2⁴ = 16.
