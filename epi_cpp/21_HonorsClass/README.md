# Honors Class

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 21. See epilight_cpp_new.pdf for the full text. All exposition below is original commentary on the chapter's topics.*

## Overview

This chapter — Part III of the book — collects problems that are markedly harder than everything before them. Many of them *do* appear in real interviews, but usually with the tacit understanding that the candidate will not reach the optimal solution; getting partway with a sound approach is already a good outcome.

Why study problems above the bar? Four reasons:

1. **Training effect.** Mastery of these problems is not required to pass interviews, but if you can solve them, ordinary questions will feel easy — like racing after training with ankle weights.
2. **Outsized payoff.** If one of these (or a variant) does come up and you derive the best solution, the impression you make goes beyond getting an offer — it can move your level, salary, and standing.
3. **Specialized roles.** Some of these problems are standard fare for positions in areas like large-scale distributed systems optimization or machine learning for computational finance, and they are commonly posed to candidates with advanced degrees.
4. **Sport.** If you enjoy a genuine challenge, this is where the book keeps them.

A mindset note: a hard question is a compliment. It means the interviewer's expectations of you are high, and it hands you an opportunity to distinguish yourself that a routine "check if this string is a palindrome" question never could.

## Intractability

Interview coding problems almost always admit an efficient solution — say O(n) time. Occasionally, though, you will be handed a problem that is computationally intractable: no efficient algorithm for it is known (or even possible). Real-world engineering meets such problems constantly, so knowing the standard escape hatches is itself interview material:

- **Brute force, accepted knowingly.** Exponential-time algorithms — including exponential dynamic programming — are perfectly reasonable when instances are small, or when the exponent attaches to a parameter that stays small in practice even as the input grows.
- **Pruned search.** Backtracking, branch-and-bound, and hill climbing all explore the same space as brute force but cut away large portions of it — via early infeasibility detection, bounding functions, or local improvement.
- **Approximation algorithms,** which trade exactness for speed but come with a *proof* that the answer is within a known factor of optimal.
- **Heuristics** — solutions engineered from problem insight, analysis of the common case, and careful tuning. No guarantees, but often excellent behavior on the instances that actually occur.
- **Parallelism.** When a problem decomposes, many machines attacking subparts simultaneously can push otherwise-infeasible instance sizes into reach.

## 21.1 Compute the greatest common divisor

**Problem.** The GCD of nonnegative integers x and y is the largest d dividing both evenly (with GCD(0, 0) defined as 0). Design an efficient algorithm for GCD **without using multiplication, division, or the modulus operator.**

*Hint: split into cases by parity — both even, both odd, one of each.*

### Development

**Attempt 1 — subtraction only.** From the identity GCD(x, y) = GCD(x − y, y) for x > y (any common divisor of x and y also divides x − y, and conversely), plus the base case GCD(x, x) = x, we get a recursion using only subtraction. It is correct — but catastrophically slow on skewed inputs: for x = 2ⁿ and y = 2 it subtracts 2 from a huge number over and over, making on the order of 2ⁿ⁻¹ calls. The running time is proportional to the *value* of the inputs, i.e., exponential in their bit length.

**Attempt 2 — collapse the subtractions.** Repeated subtraction of y from x is exactly division: the chain terminates at x mod y. Euclid's recursion GCD(x, y) = GCD(y, x mod y) is fast — but it uses the modulus operator, which the problem forbids. Dead end, by rule.

**The fast solution — binary GCD.** Keep the recursion, but replace general division with the one arithmetic the machine gives us for free: halving and doubling by bit shifts. Case analysis on parity:

- **Both even:** 2 divides both, so GCD(x, y) = 2 · GCD(x/2, y/2). Halve both (right shift), recurse, double the result (left shift).
- **Exactly one even:** the factor 2 in the even number cannot be part of the GCD, since it does not divide the odd number. Halve the even one and recurse: GCD(x, y) = GCD(x/2, y) when x is even, y odd.
- **Both odd:** fall back on subtraction, GCD(x, y) = GCD(y − x, x) for y > x. Crucially, odd minus odd is *even*, so the very next step halves.
- **Base cases:** equal arguments — the answer is that value (GCD(12, 12) = 12); one argument zero — the answer is the other (GCD(0, 6) = 6).

**Worked trace, GCD(24, 300):** both even → 2 · GCD(12, 150); both even again → 4 · GCD(6, 75); 6 even, 75 odd → 4 · GCD(3, 75); both odd → 4 · GCD(3, 72); then the even side halves repeatedly — 36, 18, 9 (odd: subtract → 6), 3 — until GCD(3, 3) = 3. Result: 4 · 3 = **12**.

```cpp
long long GCD(long long x, long long y) {
  if (x > y) {
    return GCD(y, x);            // enforce x <= y
  }
  if (x == y) {
    return x;                    // GCD(a, a) = a
  }
  if (x == 0) {
    return y;                    // GCD(0, a) = a
  }
  bool x_even = !(x & 1), y_even = !(y & 1);
  if (x_even && y_even) {
    return GCD(x >> 1, y >> 1) << 1;   // both even: factor out a 2
  } else if (x_even) {
    return GCD(x >> 1, y);             // only x even: its 2 is useless
  } else if (y_even) {
    return GCD(x, y >> 1);             // only y even: same
  }
  return GCD(x, y - x);                // both odd: difference is even
}
```

Only comparisons, subtraction, bit tests, and shifts are used — multiplication by 2 is a left shift, division by 2 a right shift.

**Complexity.** In the even cases, one argument loses a bit immediately. In the odd-odd case, the subtraction produces an even number, so the *next* call is guaranteed to shed a bit. Hence every two calls reduce the combined bit length of the arguments by at least one, and the number of calls — and total time — is O(log x + log y), i.e., linear in the input size in bits.

## 21.2 Compute the maximum product of all entries but one

**Problem.** Given an array A of n integers — positive, negative, or zero — compute the largest product achievable by multiplying exactly n − 1 of the entries, each used at most once. **Division may not be used, explicitly or implicitly.**

Examples: for ⟨3, 2, 5, 4⟩ the answer is 3 · 5 · 4 = 60 (drop the 2); for ⟨3, 2, −1, 4⟩ it is 3 · 2 · 4 = 24 (drop the −1); for ⟨3, 2, −1, 4, −1, 6⟩ it is 3 · (−1) · 4 · (−1) · 6 = 72 (drop the 2, keep both negatives).

*Hint: combine products of the first i − 1 entries and the last n − i entries. Alternatively, count the negative and zero entries.*

### Why the obvious approaches are out

- **Divide it out:** form the total product P with n − 1 multiplications, then take the max of P/A[i] over all i (n divisions). Clean — but division is banned (the motivation given is finite-precision arithmetic, where division introduces error; it also collapses on A[i] = 0).
- **Brute force:** compute each of the n products of n − 1 entries independently; each costs n − 2 multiplications, for O(n²) total. Correct, division-free, too slow.

### Approach 1 — prefix and suffix products: O(n) time, O(n) space

The product of everything except A[i] factors as (product of A[0..i−1]) × (product of A[i+1..n−1]). Adjacent exclusions overlap almost entirely — the prefix for i + 1 is just the prefix for i times A[i] — so all prefixes and all suffixes can each be computed in one linear pass. Space can be trimmed to a single stored array: precompute suffix products right-to-left, then sweep left-to-right holding the running prefix in one scalar, combining as you go and tracking the maximum.

```cpp
int FindBiggestNMinusOneProduct(const vector<int>& A) {
  const int n = A.size();
  // suffix[i] = product of A[i..n-1]; suffix[n] = 1 (empty product).
  vector<int> suffix(n + 1, 1);
  for (int i = n - 1; i >= 0; --i) {
    suffix[i] = A[i] * suffix[i + 1];
  }

  int prefix = 1;                                  // product of A[0..i-1]
  int max_product = numeric_limits<int>::min();
  for (int i = 0; i < n; ++i) {
    max_product = max(max_product, prefix * suffix[i + 1]);
    prefix *= A[i];
  }
  return max_product;
}
```

Time O(n) — two linear passes; space O(n) for the suffix array.

### Approach 2 — sign analysis: O(n) time, O(1) space

We can do better on space by asking directly: *which single entry should be dropped?* The answer depends only on the signs.

- **No negatives (or ignore signs for a moment):** drop the smallest element — you keep the n − 1 largest factors. This stays correct whether the array holds zero, one, or many 0 entries.
- **Odd number of negatives:** the product of all kept entries can only be made nonnegative by dropping one negative, and to lose as little magnitude as possible we drop the **least negative** entry (the negative with the smallest absolute value). This holds regardless of how many zeros and positives are present.
- **Even number of negatives, at least one nonnegative entry:** the negatives pair up into a positive contribution, so keep them all and drop the **smallest nonnegative** entry. (Again correct even with zeros present.)
- **Even number of negatives, no nonnegative entries:** every entry is negative and we must keep an odd count of them, so the result is unavoidably negative. To make it *least* negative, minimize the magnitude kept — i.e., drop the entry with the **largest magnitude**, which is the **greatest negative** (most negative) entry.

This yields a two-stage algorithm: one pass to classify (count negatives; remember the indices of the least negative, greatest negative, and least nonnegative entries), a branch to select the index to skip, then one pass to multiply everything else.

```cpp
int FindBiggestNMinusOneProduct(const vector<int>& A) {
  int number_of_negatives = 0;
  int least_negative_idx = -1;      // negative entry closest to zero
  int greatest_negative_idx = -1;   // most negative entry
  int least_nonnegative_idx = -1;   // smallest entry >= 0

  for (int i = 0; i < static_cast<int>(A.size()); ++i) {
    if (A[i] < 0) {
      ++number_of_negatives;
      if (least_negative_idx == -1 || A[least_negative_idx] < A[i]) {
        least_negative_idx = i;
      }
      if (greatest_negative_idx == -1 || A[i] < A[greatest_negative_idx]) {
        greatest_negative_idx = i;
      }
    } else {
      if (least_nonnegative_idx == -1 || A[i] < A[least_nonnegative_idx]) {
        least_nonnegative_idx = i;
      }
    }
  }

  const int idx_to_skip =
      (number_of_negatives % 2)
          ? least_negative_idx
          : (least_nonnegative_idx != -1 ? least_nonnegative_idx
                                         : greatest_negative_idx);

  int product = 1;
  for (int i = 0; i < static_cast<int>(A.size()); ++i) {
    if (i != idx_to_skip) {
      product *= A[i];
    }
  }
  return product;
}
```

**Complexity.** One classification pass with O(1) work per element, a constant-time selection, and one multiplication pass: O(n) + O(1) + O(n) = **O(n) time**, and the handful of index/counter locals means **O(1) additional space** — strictly better than Approach 1 on space with the same time bound.

### Variants

- **Variant 1:** Compute the array B where B[i] is the product of all entries of A except A[i] — no division allowed, O(n) time, O(1) space beyond the output. (The prefix/suffix idea applies: write suffix products into B, then sweep forward with a running prefix.)
- **Variant 2:** Compute the maximum product over all triples of distinct entries of A. (Only the extremes matter: the answer is the best of "three largest" and "two smallest × largest.")
