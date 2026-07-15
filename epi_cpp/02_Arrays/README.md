# Arrays

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 2. See epilight_cpp_new.pdf for the full text. Original exposition — explanations, code, and examples are my own.*

## Overview

The array is the simplest aggregate type — a contiguous block of equal-sized cells — and that physical simplicity is exactly what interview problems exploit. Indexing is O(1), and scanning is cache-friendly, but inserting or deleting in the middle costs O(n) unless you get clever about *where* the work happens. Most problems in this chapter share one theme: the obvious solution allocates a second array or does quadratic shifting, and the interview-quality solution operates **in place**, usually by maintaining a small set of index pointers whose invariants partition the array into zones (processed / unclassified / processed-differently). The chapter closes with a run of randomized algorithms (sampling, shuffling) and 2-D array manipulations (Sudoku, spirals, rotations) where index arithmetic does all the heavy lifting.

## Boot camp: even–odd partition

**Implementations:** [C++](cpp/even_odd_array.cc) · [Python](python/even_odd_array.py)

**Problem.** Rearrange an integer array so that all even entries come first, without allocating a second array.

**Idea.** Work from both ends with two pointers that pinch inward. Everything left of `next_even` is known even; everything right of `next_odd` is known odd; the middle is unclassified. Look at the element at `next_even`: if it is even it is already in the right zone, so advance; if it is odd, swap it to the shrinking odd zone at the back. Either way one unclassified element is classified per step, so the loop terminates after n steps.

***cpp/even_odd_array.cc***

```cpp
void EvenOdd(vector<int>* A_ptr) {
  vector<int>& A = *A_ptr;
  int next_even = 0, next_odd = size(A) - 1;
  while (next_even < next_odd) {
    if (A[next_even] % 2 == 0) {
      ++next_even;
    } else {
      swap(A[next_even], A[next_odd--]);  // Swap not even, only decrement next_odd
    }
  }
}
```

**Example.** `[3, 8, 5, 2]`: 3 is odd → swap with back → `[2, 8, 5, 3]`; 2 even → advance; 8 even → advance; pointers meet. Evens `2, 8` precede odds `5, 3`.

**Complexity.** O(n) time — each iteration classifies one element — and O(1) space. This "zone pointers + invariant" pattern is the workhorse of the whole chapter.

## Top tips

- **Prefer in-place index manipulation** to allocating a scratch array — most O(n)-space "obvious" answers here have an O(1)-space refinement.
- **Exploit both ends.** Filling or deleting from the back is free; the front costs O(n). Many tricks are just "do the work at the cheap end" (e.g. process digits from the least significant, write results from the back).
- **State your zone invariants.** When several pointers carve the array into regions, write the invariant down (even in an interview) — the code then writes itself and off-by-one errors become visible.
- **Don't be afraid to overwrite.** Deleting by overwriting forward with a write index beats shifting elements one at a time.
- **For 2-D arrays, think in layers and index formulas.** Spiral orders, rotations, and Sudoku regions all reduce to loops over an offset plus arithmetic on `n - 1 - i` style mirrored indices.
- **Know the standard randomized primitives** — Fisher–Yates shuffling and reservoir sampling — and why "obvious" alternatives (rand mod n, sorting by random keys) are biased or slow.

## Know your arrays

- `vector<int> A(n, 0)` constructs with a fill value; `A.emplace_back(x)` amortized O(1); `A.resize(k)` truncates or zero-extends.
- 2-D arrays are `vector<vector<int>>`; rows may be ragged — `size(A)` vs `size(A[i])`.
- `<algorithm>` does a lot of this chapter's work: `swap`, `reverse`, `sort`, `upper_bound` / `lower_bound` (binary search on sorted ranges), `is_sorted_until`, `find_if_not`, `partial_sum` (in `<numeric>`), `iota`.
- Reverse iterators (`rbegin`/`rend`) let forward algorithms scan from the back — see `next_permutation.cc` for `is_sorted_until(rbegin, rend)` finding the longest decreasing suffix.
- `deque<bool>` is a common stand-in for `vector<bool>` when you want ordinary element semantics rather than the packed-bit proxy of `vector<bool>`.
- `<random>`: `uniform_int_distribution<int>{a, b}` (inclusive), `generate_canonical<double, digits>` for a value in [0, 1), seeded from `random_device`.

## 2.1 The Dutch national flag problem

**Implementations:** [C++](cpp/dutch_national_flag.cc) · [Python](python/dutch_national_flag.py)

**Problem.** Given an array and a pivot index, rearrange the elements so that everything less than the pivot value comes first, then everything equal, then everything greater — the three bands of the Dutch flag. (This is exactly the partition step that makes quicksort behave on inputs with many duplicate keys.)

*Hint: what invariant can four indices maintain as you sweep once through the array?*

**Background.** The problem is Dijkstra's: he used it as a structured-programming exercise in deriving a program together with its correctness proof, which is why the invariant-first way of thinking below is the whole point. The "colour" is just any three-way predicate — less/equal/greater than a pivot, leading digit zero or not — which is what makes this the partition primitive of quicksort.

### Warm-up: two colours

With only two values the array needs three sections — zeros, unknown, ones — tracked by two indices that pinch the unknown middle: everything left of `lo` is 0, everything right of `hi` is 1. Look at `A[lo]`: if it is 0 it is already home, advance `lo`; otherwise swap it out to `A[hi]` and shrink `hi`. Each step classifies one element, so the loop is a single O(n) pass. The three-colour algorithm below is the same idea with one more boundary.

### Approach 1: two passes — O(n) time

First pass sweeps forward moving everything smaller than the pivot to a growing prefix; second pass sweeps backward moving everything larger to a growing suffix (stopping early once it crosses into the region that can no longer hold larger elements). Equals end up in the middle by elimination.

***cpp/dutch_national_flag.cc***

```cpp
// O(n) time, O(1) space, two passes (smaller forward, larger backward)
void DutchFlagPartition_two_pass(int pivot_index, vector<Color>* A_ptr) {
  vector<Color>& A = *A_ptr;
  Color pivot = A[pivot_index];

  // First pass: group elements smaller than pivot.
  int smaller = 0;
  for (int i = 0; i < A.size(); ++i) {
    if (A[i] < pivot) {
      swap(A[i], A[smaller++]);
    }
  }

  // Second pass: group elements larger than pivot.
  int larger = A.size() - 1;
  for (int i = A.size() - 1; i >= 0 && A[i] >= pivot; --i) {
    if (A[i] > pivot) {
      swap(A[i], A[larger--]);
    }
  }
}
```

**Complexity.** O(n) time, O(1) space, but two full sweeps.

### Approach 2: single pass with four zones — O(n) time

Maintain four zones and shrink the unclassified one from the left:

```
| < pivot | == pivot | unclassified | > pivot |
0        smaller    equal         larger     n
```

The next unclassified element is `A[equal]`. Three cases: smaller → swap into the bottom zone (both zone boundaries advance); equal → just advance `equal`; larger → swap to the front of the top zone (`--larger`), and do *not* advance `equal`, because the element that arrived from position `larger - 1` is itself unclassified.

***cpp/dutch_national_flag.cc***

```cpp
// O(n) time, O(1) space, single pass with four zones
void DutchFlagPartition_single_pass(int pivot_index, vector<Color>* A_ptr) {
  vector<Color>& A = *A_ptr;
  Color pivot = A[pivot_index];
  /**
   * Keep the following invariants during partitioning:
   * bottom group: A[0, smaller - 1].
   * middle group: A[smaller, equal - 1].
   * unclassified group: A[equal, larger - 1].
   * top group: A[larger, size(A) - 1].
   */
  int smaller = 0, equal = 0, larger = size(A);
  // Keep iterating as long as there is an unclassified element.
  while (equal < larger) {
    // A[equal] is the incoming unclassified element.
    if (A[equal] < pivot) {
      swap(A[smaller++], A[equal++]);
    } else if (A[equal] == pivot) {
      ++equal;
    } else {  // A[equal] > pivot.
      swap(A[equal], A[--larger]);
    }
  }
}
```

**Example.** Pivot 1 on `[0, 2, 1, 0]`: 0 < 1 → swap with itself, both advance; 2 > 1 → swap with back → `[0, 0, 1, 2]`, larger shrinks; 0 < 1 → swap into bottom; 1 == 1 → advance; done: `[0, 0, 1, 2]`.

**Complexity.** O(n) time, O(1) space, one pass. Every iteration either advances `equal` or shrinks `larger`, so exactly n classifications happen.

**Variants:** partition into three groups when keys take only three values (sort by key order); when they take four values; when entries are boolean with the false keys first — and the stable version of the boolean variant.

## 2.2 Increment an arbitrary-precision integer

**Implementations:** [C++](cpp/int_as_array_increment.cc) · [Python](python/int_as_array_increment.py)

**Problem.** Given a decimal number as an array of digits (most significant first), add one to it — e.g. `[1, 2, 9]` → `[1, 3, 0]`.

*Hint: where does a carry stop?*

### Brute force: convert, add, convert back

Turning the digits into an `int`, incrementing, and re-extracting digits fails the moment the number exceeds the machine word — which is the whole point of storing it as an array.

### Optimized: propagate the carry from the back — O(n)

Increment the last digit; while a digit hits 10, zero it and bump its left neighbor. The only special case is a carry out of the most significant digit (all-nines input): rather than inserting at the front (an O(n) shift), overwrite the leading 10 with 1 and append a 0 at the cheap end — same result, no shifting.

***cpp/int_as_array_increment.cc***

```cpp
// O(n) time, O(1) space beyond the result, n = number of digits
vector<int> PlusOne(vector<int> A) {
  ++A.back();
  for (int i = size(A) - 1; i > 0 && A[i] == 10; --i) {
    A[i] = 0, ++A[i - 1];
  }
  if (A[0] == 10) {
    // There is a carry-out, so we need one more digit to store the result.
    // A slick way to do this is to append a 0 at the end of the array,
    // and update the first entry to 1.
    A[0] = 1;
    A.emplace_back(0);

    // Original Code
    // A[0] = 0;
    // A.insert(A.begin(), 1);
  }
  return A;
}
```

**Example.** `[9, 9]` → increment → `[9, 10]` → carry → `[10, 0]` → leading fix → `[1, 0]` + append → `[1, 0, 0]`.

**Complexity.** O(n) time (the carry chain is at most the array length), O(1) extra space.

**Variants:** add two numbers given as binary strings; more generally, add two base-b numbers as digit arrays.

## 2.3 Multiply two arbitrary-precision integers

**Implementations:** [C++](cpp/int_as_array_multiply.cc) · [Python](python/int_as_array_multiply.py)

**Problem.** Multiply two numbers represented as digit arrays, where a negative number carries its sign on the leading digit — e.g. `[-1, 3] × [1, 2] = [-1, 5, 6]`.

*Hint: in grade-school multiplication, which result column does digit i × digit j land in?*

### Approach: grade-school digit products — O(nm)

The product of an n-digit and an m-digit number has at most n + m digits, so allocate that much and accumulate every pairwise digit product directly into its column: `num1[i] * num2[j]` lands at `result[i + j + 1]`, with the carry spilling into `result[i + j]`. Handling the carry immediately after each product keeps every cell below 10, so no second normalization pass is needed. The sign is computed up front (negative iff exactly one operand is negative) and re-applied to the leading digit at the end, after stripping leading zeros.

***cpp/int_as_array_multiply.cc***

```cpp
// O(nm) time, O(n + m) space, n/m = digit counts
vector<int> Multiply(vector<int> num1, vector<int> num2) {
  const int sign = (num1.front() < 0) ^ (num2.front() < 0) ? -1 : 1;
  num1.front() = abs(num1.front()), num2.front() = abs(num2.front());

  vector<int> result(size(num1) + size(num2), 0);
  for (int i = size(num1) - 1; i >= 0; --i) {
    for (int j = size(num2) - 1; j >= 0; --j) {
      result[i + j + 1] += num1[i] * num2[j];
      result[i + j] += result[i + j + 1] / 10;
      result[i + j + 1] %= 10;
    }
  }

  // Remove the leading zeroes.
  result = {
      find_if_not(begin(result), end(result), [](int a) { return a == 0; }),
      end(result)};
  if (empty(result)) {
    return {0};
  }
  result.front() *= sign;
  return result;
}
```

**Example.** `[1, 3] × [2, 4]`: the four digit products accumulate column by column into `[0, 3, 1, 2]` → strip the leading zero → `[3, 1, 2]` = 312 = 13 × 24 ✓.

**Complexity.** O(nm) digit multiplications; O(n + m) space for the result.

## 2.4 Advancing through an array

**Implementations:** [C++](cpp/advance_by_offsets.cc) · [Python](python/advance_by_offsets.py)

**Problem.** Each entry is the maximum number of steps you may advance from that position. Starting at index 0, can you reach the last index? E.g. `[3, 3, 1, 0, 2, 0, 1]` — yes; `[3, 2, 0, 0, 2, 0, 1]` — no (the zeros at indices 2–3 form a wall).

*Hint: you don't need to know the path, only how far is reachable.*

### Approach: greedy furthest-reach sweep — O(n)

Track one number: the furthest index reachable so far. Walk i across the array, but only while `i <= furthest_reach_so_far` — an index beyond that is unreachable by definition. At each reachable i, the candidate new frontier is `i + A[i]`. If the frontier ever reaches the last index, done; if i outruns the frontier first, you are stuck.

***cpp/advance_by_offsets.cc***

```cpp
// O(n) time, O(1) space, n = array length
bool CanReachEnd(const vector<int>& max_advance_steps) {
  int furthest_reach_so_far = 0, last_index = size(max_advance_steps) - 1;
  for (int i = 0;
       i <= furthest_reach_so_far && furthest_reach_so_far < last_index; ++i) {
    furthest_reach_so_far =
        max(furthest_reach_so_far, max_advance_steps[i] + i);
  }
  return furthest_reach_so_far >= last_index;
}
```

**Complexity.** O(n) time, O(1) space.

**Variants:** compute the *minimum* number of steps needed to reach the end.

## 2.5 Delete duplicates from a sorted array

**Implementations:** [C++](cpp/sorted_array_remove_dups.cc) · [Python](python/sorted_array_remove_dups.py)

**Problem.** Remove duplicates from a sorted array in place and return the count of remaining valid entries — `[2, 3, 5, 5, 7, 11, 11, 13]` → first 6 entries become `[2, 3, 5, 7, 11, 13]`.

*Hint: sortedness means every duplicate is adjacent to its original.*

### Brute force: shift on every duplicate

Delete each duplicate by shifting everything after it left — O(n) per delete, O(n²) total. (A hash set detects duplicates in an unsorted array, but here sortedness gives detection for free.)

### Optimized: write index — O(n)

Because the array is sorted, keeping an element is a purely local decision: keep `A[i]` iff it differs from the last *kept* element. A write index marks the end of the deduplicated prefix; every kept element is copied there. Elements are only ever copied leftward onto positions already examined, so nothing is lost.

***cpp/sorted_array_remove_dups.cc***

```cpp
// O(n) time, O(1) space, n = array length
int DeleteDuplicates(vector<int>* A_ptr) {
  vector<int>& A = *A_ptr;
  if (empty(A)) {
    return 0;
  }

  int write_index = 1;
  for (int i = 1; i < size(A); ++i) {
    if (A[write_index - 1] != A[i]) {
      A[write_index++] = A[i];
    }
  }
  return write_index;
}
```

**Example.** `[2, 3, 3, 5]`: i=1 keeps 3 (write index → 2); i=2, 3 equals last kept → skip; i=3 keeps 5 → prefix `[2, 3, 5]`, return 3.

**Complexity.** O(n) time, O(1) space.

**Variants:** remove every occurrence of a given key in place; update the array so each distinct element appears at most twice.

## 2.6 Buy and sell a stock once

**Implementations:** [C++](cpp/buy_and_sell_stock.cc) · [Python](python/buy_and_sell_stock.py)

**Problem.** Given daily prices, find the maximum profit from one buy followed by one later sell. For `[310, 315, 275, 295, 260, 270, 290, 230, 255, 250]` the answer is 30 (buy 260, sell 290) — note it is *not* max − min, because the global minimum (230) comes after the best selling day.

*Hint: if you sell on day i, when should you have bought?*

### Brute force: all pairs — O(n²)

Try every buy/sell pair with the buy before the sell.

### Optimized: minimum-so-far sweep — O(n)

Selling on day i is optimal against the *minimum price seen before i*. So one forward pass suffices: maintain the running minimum, and at each price evaluate the profit of selling today against it.

***cpp/buy_and_sell_stock.cc***

```cpp
// O(n) time, O(1) space, n = number of days
double BuyAndSellStockOnce(const vector<double>& prices) {
  double min_price_so_far = numeric_limits<double>::infinity(), max_profit = 0;
  for (double price : prices) {
    double max_profit_sell_today = price - min_price_so_far;
    max_profit = max(max_profit, max_profit_sell_today);
    min_price_so_far = min(min_price_so_far, price);
  }
  return max_profit;
}
```

**Complexity.** O(n) time, O(1) space. (This is also a disguised maximum-subarray problem on the day-to-day price differences.)

**Variants:** longest run of equal entries; maximum profit with unlimited transactions.

## 2.7 Buy and sell a stock twice

**Implementations:** [C++](cpp/buy_and_sell_stock_twice.cc) · [Python](python/buy_and_sell_stock_twice.py)

**Problem.** Same setup, but at most **two** non-overlapping buy–sell transactions (the second buy must be on or after the first sell).

*Hint: a second transaction starting on day i only cares about the best single transaction ending before i.*

### Approach 1: forward pass + backward pass — O(n) time, O(n) space

Split the timeline at every day i: the best double transaction whose second buy is on day i is (best single transaction within days [0, i−1]) + (best sale after buying on day i). The forward pass records, for each day, the best single-transaction profit achievable by that day. The backward pass tracks the maximum future price and combines the two.

***cpp/buy_and_sell_stock_twice.cc***

```cpp
// O(n) time, O(n) space, n = number of days
double BuyAndSellStockTwice_two_pass(const vector<double>& prices) {
  double max_total_profit = 0;
  vector<double> first_buy_sell_profits(size(prices), 0);
  double min_price_so_far = numeric_limits<double>::infinity();

  // Forward phase. For each day, we record maximum profit if we
  // sell on that day.
  for (int i = 0; i < size(prices); ++i) {
    min_price_so_far = min(min_price_so_far, prices[i]);
    max_total_profit = max(max_total_profit, prices[i] - min_price_so_far);
    first_buy_sell_profits[i] = max_total_profit;
  }

  // Backward phase. For each day, find the maximum profit if we make
  // the second buy on that day.
  double max_price_so_far = numeric_limits<double>::lowest();
  for (int i = size(prices) - 1; i > 0; --i) {
    max_price_so_far = max(max_price_so_far, prices[i]);
    max_total_profit = max(max_total_profit, max_price_so_far - prices[i] +
                                                 first_buy_sell_profits[i]);
  }
  return max_total_profit;
}
```

### Approach 2: state machine — O(n) time, O(1) space

Track, per transaction number, the best "effective buy price" and best profit so far. The trick that makes the space constant: the second buy's effective price is discounted by the profit already banked from the first sale (`price - max_profits[i - 1]`), so a single sweep updates both transactions.

***cpp/buy_and_sell_stock_twice.cc***

```cpp
// O(n) time, O(1) space, n = number of days
double BuyAndSellStockTwice_constant_space(const vector<double>& prices) {
  array<double, 2> min_prices = {numeric_limits<double>::infinity(),
                                 numeric_limits<double>::infinity()},
                   max_profits = {0.0, 0.0};
  for (double price : prices) {
    for (int i = 1; i >= 0; --i) {
      max_profits[i] = max(max_profits[i], price - min_prices[i]);
      min_prices[i] =
          min(min_prices[i], price - (i - 1 >= 0 ? max_profits[i - 1] : 0.0));
    }
  }
  return max_profits[1];
}
```

**Complexity.** Both O(n) time; O(n) vs O(1) space. The state-machine form also generalizes to k transactions with two length-k arrays.

## Computing an alternation

**Implementations:** [C++](cpp/alternating_array.cc) · [Python](python/alternating_array.py)

*(Kept unnumbered to match the chapter index.)*

**Problem.** Rearrange the array into the zigzag pattern A[0] ≤ A[1] ≥ A[2] ≤ A[3] ≥ …

*Hint: does position i really need to know about anything beyond its neighbor?*

### Brute force: sort, then interleave

Sort, then swap each adjacent pair `(A[1], A[2]), (A[3], A[4]), …` — correct but O(n log n), and it establishes far more order than the output requires. (A median-based refinement gets to O(n), but is still more machinery than needed.)

### Optimized: local greedy repair — O(n)

The condition at position i only constrains the pair (A[i−1], A[i]). Sweep once; whenever a pair violates its required direction (should be ascending into an odd index, descending into an even one), swap the two. A swap never breaks the pair to the left: if i is odd and A[i−1] > A[i], the swap moves a *smaller* value into position i−1, which can only help the ≥ constraint arriving from i−2 — and symmetrically for even i.

***cpp/alternating_array.cc***

```cpp
// O(n) time, O(1) space, n = array length
void Rearrange(vector<int>* A_ptr) {
  vector<int>& A = *A_ptr;
  for (size_t i = 1; i < size(A); ++i) {
    if ((!(i % 2) && A[i - 1] < A[i]) || ((i % 2) && A[i - 1] > A[i])) {
      swap(A[i - 1], A[i]);
    }
  }
}
```

**Example.** `[5, 2, 8, 1]`: i=1 (want ≤): 5 > 2 → swap → `[2, 5, 8, 1]`; i=2 (want ≥): 5 < 8 → swap → `[2, 8, 5, 1]`; i=3 (want ≤): 5 > 1 → swap → `[2, 8, 1, 5]` ✓.

**Complexity.** O(n) time, O(1) space.

## 2.8 Enumerate all primes to n

**Implementations:** [C++](cpp/prime_sieve.cc) · [Python](python/prime_sieve.py)

**Problem.** Return every prime ≤ n, e.g. n = 18 → `[2, 3, 5, 7, 11, 13, 17]`.

*Hint: don't test numbers for primality — eliminate composites wholesale.*

### Approach 1: basic sieve of Eratosthenes — O(n log log n)

Trial-dividing each candidate costs O(n^1.5) overall. The sieve inverts the work: each discovered prime p crosses off its multiples starting at p² (smaller multiples were already crossed off by smaller primes). The outer loop only needs to run while p² ≤ n.

***cpp/prime_sieve.cc***

```cpp
// O(n log log n) time, O(n) space — basic sieve of Eratosthenes.
vector<int> GeneratePrimes_basic(int n) {
    if (n < 2) {
      return {};
    }

    // Create a boolean array "prime[0..n]" and initialize
    // all entries it as true. A value in prime[i] will
    // finally be false if i is Not a prime, else true.
    vector<bool> prime(n+1, true);
    //memset(prime, true, sizeof(prime));

    prime[0]=prime[1]=false;
    for (int p=2; p*p<=n; p++) {
        // If prime[p] is not changed, then it is a prime
        if (prime[p] == true) {
            // Update all multiples of p greater than or
            // equal to the square of it
            // numbers which are multiple of p and are
            // less than p^2 are already been marked.
            for (int i=p*p; i<=n; i += p)
                prime[i] = false;
        }
    }
    
    vector<int> primes;
    for (int i=0; i<prime.size(); i++) {
      if (prime[i]) {
        primes.push_back(i);
      }
    }
    return primes;
}
```

### Approach 2: sieve odd candidates only — half the space and work

Even numbers other than 2 are never prime, so sieve only odds: entry i of the table represents the number 2i + 3. Emit 2 manually; a prime p = 2i + 3 starts crossing off at p², whose *index* works out to 2i² + 6i + 3 — index arithmetic replaces value arithmetic, halving both storage and marking work. (The `long long j` guards against p² overflowing `int`.)

***cpp/prime_sieve.cc***

```cpp
// O(n log log n) time, O(n) space — sieve over odd candidates only,
// starting each round of crossing-off at p^2.
vector<int> GeneratePrimes_odd_only(int n) {
  if (n < 2) {
    return {};
  }
  const int size = floor(0.5 * (n - 3)) + 1;  // size ~= sqrt(n)
  vector<int> primes;
  primes.emplace_back(2);
  // is_prime[i] represents whether (2i + 3) is prime or not.
  // Initially, set each to true. Then use sieving to eliminate nonprimes.
  deque<bool> is_prime(size, true);
  for (int i = 0; i < size; ++i) {
    if (is_prime[i]) {
      int p = (i * 2) + 3;
      primes.emplace_back(p);
      // Sieving from p^2, whose value is (4i^2 + 12i + 9). The index in
      // is_prime is (2i^2 + 6i + 3) because is_prime[i] represents 2i + 3.
      //
      // Note that we need to use long long for j because p^2 might overflow.
      for (long long j = 2LL * i * i + 6 * i + 3; j < size; j += p) {
        is_prime[j] = false;
      }
    }
  }
  return primes;
}
```

**Complexity.** Both O(n log log n) time and O(n) space; the odd-only version halves the constants.

## 2.9 Permute the elements of an array

**Implementations:** [C++](cpp/apply_permutation.cc) · [Python](python/apply_permutation.py)

**Problem.** Given a permutation P and an array A, rearrange A so the element at position i moves to position P[i] — in place.

*Hint: every permutation decomposes into independent cycles.*

### Brute force: second array

`B[P[i]] = A[i]`, then copy back — O(n) time but O(n) space.

### Optimized: settle each position with local swaps — O(n)

At position i, repeatedly swap `A[i]` with `A[perm[i]]` — sending the current occupant directly to its destination — and swap the corresponding `perm` entries in tandem so the bookkeeping follows the data. Each swap parks at least one element in its final home and records that in `perm` (`perm[i]` becomes i there), so at most n swaps happen across the entire run — amortized O(n), not O(n²). When the sweep finishes, `perm` has been sorted back to the identity: the "tandem swap" is what makes the marking self-cleaning without extra space.

***cpp/apply_permutation.cc***

```cpp
// O(n) time, O(1) space (perm is restored via sign-free cycle walk), n = array length
void ApplyPermutation(vector<int> perm, vector<int>* A_ptr) {
  vector<int>& A = *A_ptr;
  for (int i = 0; i < size(A); ++i) {
    while (perm[i] != i) {
      swap(A[i], A[perm[i]]);
      swap(perm[i], perm[perm[i]]);
    }
  }
}
```

**Complexity.** O(n) time. The classic alternative marks visited entries by subtracting n from `perm[i]` and restoring afterward; this version instead re-sorts `perm` to the identity as it goes.

**Variants:** apply the *inverse* permutation without materializing it; invert a permutation in place.

## 2.10 Compute the next permutation

**Implementations:** [C++](cpp/next_permutation.cc) · [Python](python/next_permutation.py)

**Problem.** Given a permutation, produce the next one in lexicographic order (or the empty array if it is the last). E.g. `[6, 2, 1, 5, 4, 3, 0]` → `[6, 2, 3, 0, 1, 4, 5]`.

*Hint: which suffix is already as large as it can possibly be?*

### Approach: inversion point + swap + reverse — O(n)

```
6 2 1 5 4 3 0
    ^ └─────┘  longest decreasing suffix (already maximal)
    inversion point: 1 (first entry from the right smaller than its successor)

swap 1 with 3 (smallest suffix entry > 1):  6 2 3 5 4 1 0
reverse the suffix (make it minimal):       6 2 3 0 1 4 5
```

A decreasing suffix cannot be advanced by rearranging itself — it is its own maximum. So the leftmost digit that changes must be the entry just before that suffix (the *inversion point*). For the smallest possible increase, replace it with the least suffix entry that exceeds it, then make the new suffix minimal — and since the suffix is still decreasing after the swap, "sort ascending" is just "reverse".

***cpp/next_permutation.cc***

```cpp
// O(n) time, O(1) space, n = array length
vector<int> NextPermutation(vector<int> perm) {
  // Find the first entry from the right that is smaller than the entry
  // immediately after it.
  auto inversion_point = is_sorted_until(rbegin(perm), rend(perm));
  if (inversion_point == rend(perm)) {
    // perm is sorted in decreasing order, so it's the last permutation.
    return {};
  }

  // Swap the entry referenced by inversion_point with smallest entry
  // appearing after inversion_point that is greater than the entry referenced
  // by inversion_point:
  //
  // 1.) Find the smallest entry after inversion_point that's greater than the
  //     entry referenced by inversion_point. Since perm must be sorted in
  //     decreasing order after inversion_point, we can use a fast algorithm
  //     to find this entry.
  auto least_upper_bound =
      upper_bound(rbegin(perm), inversion_point, *inversion_point);

  // 2.) Perform the swap.
  iter_swap(inversion_point, least_upper_bound);

  // Reverse the subarray that follows inversion_point.
  reverse(rbegin(perm), inversion_point);
  return perm;
}
```

**Complexity.** O(n) time, O(1) space.

**Python note.** The C++ leans on reverse-iterator algorithms — `is_sorted_until(rbegin, rend)` finds the inversion point and `upper_bound` binary-searches the decreasing suffix. [python/next_permutation.py](python/next_permutation.py) spells both scans out as explicit index loops, which is the clearer form if the iterator gymnastics feel opaque.

**Variants:** the k-th permutation directly (factorial number system); the *previous* permutation.

## 2.11 Sample offline data

**Implementations:** [C++](cpp/offline_sampling.cc) · [Python](python/offline_sampling.py)

**Problem.** Produce a uniformly random size-k subset of an n-element array, using O(1) extra space.

*Hint: build the sample inside the array's own prefix, one element at a time.*

### Approach: partial Fisher–Yates shuffle — O(k)

For i from 0 to k−1, pick a uniform index in [i, n−1] and swap it into position i. After k rounds the prefix A[0, k−1] is the sample. Uniformity follows by induction: the first pick is uniform over n choices; conditioned on it, the second is uniform over the remaining n−1; so every ordered k-sequence has probability 1/(n·(n−1)···(n−k+1)), hence every subset is equally likely.

***cpp/offline_sampling.cc***

```cpp
// O(k) time, O(1) space beyond the input array
void RandomSampling(int k, vector<int>* A_ptr) {
  vector<int>& A = *A_ptr;
  default_random_engine seed((random_device())());  // Random num generator.
  for (int i = 0; i < k; ++i) {
    // Generate a random index in [i, size(A) - 1].
    swap(A[i], A[uniform_int_distribution<int>{
                   i, static_cast<int>(A.size()) - 1}(seed)]);
  }
}
```

**Complexity.** O(k) time, O(1) space. When k > n/2, sample the n−k *excluded* elements instead.

**Python note.** [python/offline_sampling.py](python/offline_sampling.py) also carries a one-line `random_sampling_pythonic` variant — `A[:] = random.sample(A, k)` — the standard-library form of the same operation.

## 2.12 Sample online data

**Implementations:** [C++](cpp/online_sampling.cc) · [Python](python/online_sampling.py)

**Problem.** Maintain a uniform random size-k sample of a stream whose length is unknown in advance, using O(k) space.

*Hint: with what probability should element number t displace something in the sample?*

### Approach: reservoir sampling — O(n) time, O(k) space

Keep the first k elements. For element number t > k, admit it with probability k/t, evicting a uniformly random current occupant. Induction: assume after t−1 elements every element seen so far is in the sample with probability k/(t−1). The new element enters with probability k/t ✓; an old occupant survives with probability k/(t−1) × (1 − (k/t)·(1/k)) = k/(t−1) × (t−1)/t = k/t ✓. The code fuses "admit with probability k/t, then evict uniformly" into a single draw: pick a uniform index in [0, t−1] and replace only when it lands below k.

***cpp/online_sampling.cc***

```cpp
// O(n) time, O(k) space, n = stream length
vector<int> OnlineRandomSample(vector<int>::const_iterator stream_begin,
                               const vector<int>::const_iterator stream_end,
                               int k) {
  vector<int> running_sample;
  // Storesult the first k elements.
  for (int i = 0; i < k; ++i) {
    running_sample.emplace_back(*stream_begin++);
  }

  default_random_engine seed((random_device())());  // Random num generator.
  // Have read the first k elements.
  int num_seen_so_far = k;
  while (stream_begin != stream_end) {
    int x = *stream_begin++;
    ++num_seen_so_far;
    // Generate a random number in [0, num_seen_so_far - 1], and if this
    // number is in [0, k - 1], we replace that element from the sample with
    // x.
    if (const int idx_to_replace =
            uniform_int_distribution<int>{0, num_seen_so_far - 1}(seed);
        idx_to_replace < k) {
      running_sample[idx_to_replace] = x;
    }
  }
  return running_sample;
}
```

**Complexity.** O(n) time over the whole stream, O(k) space.

## 2.13 Compute a random permutation

**Implementations:** [C++](cpp/random_permutation.cc) · [Python](python/random_permutation.py)

**Problem.** Generate a uniformly random permutation of {0, …, n−1}.

*Hint: you already wrote the shuffle — 2.11 with k = n.*

### Approach: full Fisher–Yates — O(n)

Beware the tempting wrong answers: assigning each element `rand() % n` slots collides; sorting by random keys costs O(n log n) and is biased if keys repeat. The correct primitive is the 2.11 prefix shuffle run to completion — and the file literally reuses it via the include-with-renamed-main trick (`#define main _main` / `#include "offline_sampling.cc"`).

***cpp/random_permutation.cc***

```cpp
// O(n) time, O(1) space beyond the result
vector<int> ComputeRandomPermutation(int n) {
  vector<int> permutation(n);
  // Initializes permutation to 0, 1, 2, ..., n - 1.
  iota(begin(permutation), end(permutation), 0);
  RandomSampling(n, &permutation);
  return permutation;
}
```

**Complexity.** O(n) time; the n − 1 shrinking-range draws are exactly enough randomness to select uniformly among n! outcomes.

## 2.14 Compute a random subset

**Implementations:** [C++](cpp/random_subset.cc) · [Python](python/random_subset.py)

**Problem.** Return a uniformly random k-subset of {0, 1, …, n−1} where n is huge and k tiny — so materializing an n-element array is unacceptable.

*Hint: run 2.11's loop, but store only the entries that differ from the identity.*

### Approach: hash map as a virtual array — O(k) time and space

Conceptually run the Fisher–Yates prefix shuffle on the identity array A[i] = i, but represent that array as a hash map holding *only the entries that have been disturbed*; any index absent from the map still holds its identity value. Each round touches two indices (i and the random pick), giving four cases by which of the two is already in the map — each is O(1) map work.

***cpp/random_subset.cc***

```cpp
// O(k) time, O(k) space — simulates the k relevant entries of a length-n array
vector<int> RandomSubset(int n, int k) {
  unordered_map<int, int> changed_elements;
  default_random_engine seed((random_device())());  // Random num generator.
  for (int i = 0; i < k; ++i) {
    // Generate a random index in [i, n - 1].
    int rand_idx = uniform_int_distribution<int>{i, n - 1}(seed);
    if (auto ptr1 = changed_elements.find(rand_idx),
        ptr2 = changed_elements.find(i);
        ptr1 == end(changed_elements) && ptr2 == end(changed_elements)) {
      changed_elements[rand_idx] = i;
      changed_elements[i] = rand_idx;
    } else if (ptr1 == end(changed_elements) && ptr2 != end(changed_elements)) {
      changed_elements[rand_idx] = ptr2->second;
      ptr2->second = rand_idx;
    } else if (ptr1 != end(changed_elements) && ptr2 == end(changed_elements)) {
      changed_elements[i] = ptr1->second;
      ptr1->second = i;
    } else {
      int temp = ptr2->second;
      changed_elements[i] = ptr1->second;
      changed_elements[rand_idx] = temp;
    }
  }

  vector<int> result;
  for (int i = 0; i < k; ++i) {
    result.emplace_back(changed_elements[i]);
  }
  return result;
}
```

**Example.** n = 100, k = 2: first draw picks, say, 41 → map {0↔41}; second picks 27 → map gains {1↔27}; result `[41, 27]`. Four map entries ever exist — never a 100-element array.

**Complexity.** O(k) time and space, independent of n.

## 2.15 Generate nonuniform random numbers

**Implementations:** [C++](cpp/nonuniform_random_number.cc) · [Python](python/nonuniform_random_number.py)

**Problem.** Given values v₀…vₙ₋₁ with probabilities p₀…pₙ₋₁ (summing to 1) and a uniform random generator, produce values with exactly those probabilities.

*Hint: chop [0, 1) into pieces whose widths are the pᵢ.*

### Approach: prefix sums + binary search — O(n) setup, O(log n) per draw

The prefix sums of the probabilities cut [0, 1) into n disjoint intervals, the i-th of width pᵢ:

```
p = 0.5, 0.3, 0.2
[0.0 ────────── 0.5 ────── 0.8 ──── 1.0)
       v0            v1        v2
```

Draw U uniform in [0, 1) and locate its interval with `upper_bound` — U lands in interval i with probability exactly pᵢ.

***cpp/nonuniform_random_number.cc***

```cpp
// O(n) time, O(n) space to build prefix sums; O(log n) per additional query
int NonuniformRandomNumberGeneration(const vector<int>& values,
                                     const vector<double>& probabilities) {
  vector<double> prefix_sums_of_probabilities;
  // Creating the endpoints for the intervals corresponding to the
  // probabilities.
  partial_sum(cbegin(probabilities), cend(probabilities),
              back_inserter(prefix_sums_of_probabilities));

  static default_random_engine seed((random_device())());
  const double uniform_0_1 =
      generate_canonical<double, numeric_limits<double>::digits>(seed);
  // Find the index of the interval that uniform_0_1 lies in, which is the
  // return value of upper_bound() minus 1.
  const int interval_idx =
      distance(cbegin(prefix_sums_of_probabilities),
               upper_bound(cbegin(prefix_sums_of_probabilities),
                           cend(prefix_sums_of_probabilities), uniform_0_1));
  return values[interval_idx];
}
```

**Complexity.** O(n) to build the table, O(log n) per draw once the prefix sums are cached. (The alias method reaches O(1) per draw with O(n) setup, at the price of a more intricate table.)

## 2.16 The Sudoku checker problem

**Implementations:** [C++](cpp/is_valid_sudoku.cc) · [Python](python/is_valid_sudoku.py)

**Problem.** Check whether a partially filled n×n Sudoku grid (0 = empty) violates any constraint: no duplicate among the filled cells of any row, column, or √n×√n region.

*Hint: one duplicate-detector, three kinds of rectangular slices.*

### Approach: one presence-table scan per constraint — O(n²)

All three constraint types ask the same question — "does this rectangular slice contain a repeated value in 1..n?" — so write that check once against a boolean presence table and call it for each of the n rows, n columns, and n regions. Every cell is examined exactly three times.

***cpp/is_valid_sudoku.cc***

```cpp
// O(n^2) time, O(n) space, n = grid dimension
bool IsValidSudoku(const vector<vector<int>>& partial_assignment) {
  // Check row constraints.
  for (int i = 0; i < size(partial_assignment); ++i) {
    if (HasDuplicate(partial_assignment, i, i + 1, 0,
                     size(partial_assignment))) {
      return false;
    }
  }

  // Check column constraints.
  for (int j = 0; j < size(partial_assignment); ++j) {
    if (HasDuplicate(partial_assignment, 0, size(partial_assignment), j,
                     j + 1)) {
      return false;
    }
  }

  // Check region constraints.
  int region_size = sqrt(size(partial_assignment));
  for (int I = 0; I < region_size; ++I) {
    for (int J = 0; J < region_size; ++J) {
      if (HasDuplicate(partial_assignment, region_size * I,
                       region_size * (I + 1), region_size * J,
                       region_size * (J + 1))) {
        return false;
      }
    }
  }
  return true;
}

// Return true if subarray partial_assignment[start_row, end_row -
// 1][start_col, end_col - 1] contains any duplicates in {1, 2, ...,
// size(partial_assignment)}; otherwise return false.
bool HasDuplicate(const vector<vector<int>>& partial_assignment, int start_row,
                  int end_row, int start_col, int end_col) {
  deque<bool> is_present(size(partial_assignment) + 1, false);
  for (int i = start_row; i < end_row; ++i) {
    for (int j = start_col; j < end_col; ++j) {
      if (partial_assignment[i][j] != 0 &&
          is_present[partial_assignment[i][j]]) {
        return true;
      }
      is_present[partial_assignment[i][j]] = true;
    }
  }
  return false;
}
```

**Complexity.** O(n²) time, O(n) space for the presence table.

**Variants:** *solve* the Sudoku (backtracking — a recursion-chapter problem); re-validate incrementally after one cell changes by checking just its row, column, and region.

## 2.17 Compute the spiral ordering of a 2D array

**Implementations:** [C++](cpp/spiral_ordering.cc) · [Python](python/spiral_ordering.py)

**Problem.** Read an n×n matrix in clockwise spiral order — for the 3×3 matrix of 1..9 that is `1 2 3 6 9 8 7 4 5`.

*Hint: the spiral is concentric rings; peel one ring at a time.*

### Approach: peel rings by offset — O(n²)

Ring `offset` is the square frame with corners `(offset, offset)` and `(n-1-offset, n-1-offset)`. Each ring is four sweeps, each deliberately stopping one element short of its corner so no element is emitted twice:

```
→ → → ·        top row      [offset .. n-2-offset]
·     ↓        right col    [offset .. n-2-offset]
↑     ↓
· ← ← ←        bottom row and left col: mirrored
```

An odd-sized matrix leaves one center cell, handled as its own case.

***cpp/spiral_ordering.cc***

```cpp
// O(n^2) time, O(1) space beyond the result, n = matrix dimension
vector<int> MatrixInSpiralOrder(const vector<vector<int>>& square_matrix) {
  vector<int> spiral_ordering;
  for (int offset = 0; offset < ceil(0.5 * size(square_matrix)); ++offset) {
    MatrixLayerInClockwise(square_matrix, offset, &spiral_ordering);
  }
  return spiral_ordering;
}

void MatrixLayerInClockwise(const vector<vector<int>>& square_matrix,
                            int offset, vector<int>* spiral_ordering) {
  if (offset == size(square_matrix) - offset - 1) {
    // square_matrix has odd dimension, and we are at the center of
    // square_matrix.
    spiral_ordering->emplace_back(square_matrix[offset][offset]);
    return;
  }

  for (int j = offset; j < size(square_matrix) - offset - 1; ++j) {
    spiral_ordering->emplace_back(square_matrix[offset][j]);
  }
  for (int i = offset; i < size(square_matrix) - offset - 1; ++i) {
    spiral_ordering->emplace_back(
        square_matrix[i][size(square_matrix) - offset - 1]);
  }
  for (int j = size(square_matrix) - offset - 1; j > offset; --j) {
    spiral_ordering->emplace_back(
        square_matrix[size(square_matrix) - offset - 1][j]);
  }
  for (int i = size(square_matrix) - offset - 1; i > offset; --i) {
    spiral_ordering->emplace_back(square_matrix[i][offset]);
  }
}
```

**Complexity.** O(n²) time — each element emitted once — O(1) space beyond the output.

**Python note.** [python/spiral_ordering.py](python/spiral_ordering.py) expresses each sweep with list slicing (e.g. `square_matrix[offset][offset:-1 - offset]`), including negative end indices — compact, but mind that `-1 - offset` excludes the corner just as the C++ loop bounds do.

**Variants:** spiral order for an m×n matrix; *fill* a matrix with 1..n² in spiral order; coordinates of an outward spiral starting from the center.

## 2.18 Rotate a 2D array

**Implementations:** [C++](cpp/matrix_rotation.cc) · [Python](python/matrix_rotation.py)

**Problem.** Rotate an n×n matrix 90° clockwise, in place.

*Hint: rotation moves elements in cycles of four.*

### Brute force: rotate into a copy

`B[j][n-1-i] = A[i][j]` — O(n²) time but O(n²) extra space.

### Optimized: 4-way exchange per layer — O(n²) time, O(1) space

Under a 90° clockwise turn, four positions chase each other in a cycle, so they can be rotated with a single 4-way exchange and no full-matrix copy:

```
(i, j) ← (n-1-j, i) ← (n-1-i, n-1-j) ← (j, n-1-i) ← (i, j)

    ┌───────────┐      e.g. the four corners:
    │ A ····· B │      D→A, C→D, B→C, A→B
    │ ·       · │      in one exchange
    │ D ····· C │
    └───────────┘
```

Process the matrix ring by ring; within ring i, each j along the top edge drives one exchange, so every element moves exactly once.

***cpp/matrix_rotation.cc***

```cpp
// O(n^2) time, O(1) space — in-place 4-way exchange per layer
void RotateMatrix(vector<vector<int>>* square_matrix_ptr) {
  vector<vector<int>>& square_matrix = *square_matrix_ptr;
  const int matrix_size = size(square_matrix) - 1;
  for (int i = 0; i < (size(square_matrix) / 2); ++i) {
    for (int j = i; j < matrix_size - i; ++j) {
      // Perform a 4-way exchange.
      int temp1 = square_matrix[matrix_size - j][i];
      int temp2 = square_matrix[matrix_size - i][matrix_size - j];
      int temp3 = square_matrix[j][matrix_size - i];
      int temp4 = square_matrix[i][j];
      square_matrix[i][j] = temp1;
      square_matrix[matrix_size - j][i] = temp2;
      square_matrix[matrix_size - i][matrix_size - j] = temp3;
      square_matrix[j][matrix_size - i] = temp4;
    }
  }
}
```

**Complexity.** O(n²) time, O(1) space. (A constant-*time* "rotation" is also possible by wrapping the matrix in a view object that translates indices on every access.)

**Variants:** reflect the matrix about its horizontal, vertical, or diagonal axis in place — each is a coordinate remapping needing only 2-way swaps.

## 2.19 Compute rows in Pascal's Triangle

**Implementations:** [C++](cpp/pascal_triangle.cc) · [Python](python/pascal_triangle.py)

**Problem.** Return the first n rows of Pascal's triangle — each interior entry is the sum of the two entries above it.

```
        1
       1 1
      1 2 1
     1 3 3 1
    1 4 6 4 1
```

*Hint: left-justify the triangle and "the two above" become clean indices.*

### Approach: build each row from the previous — O(n²)

Left-justified, row i has i + 1 entries and entry (i, j) is `row[i-1][j-1] + row[i-1][j]` for interior j, with 1 at both edges. Each row is produced in one pass from the row just built.

***cpp/pascal_triangle.cc***

```cpp
// O(n^2) time, O(n^2) space, n = number of rows
vector<vector<int>> GeneratePascalTriangle(int num_rows) {
  vector<vector<int>> pascal_triangle;
  for (int i = 0; i < num_rows; ++i) {
    vector<int> curr_row;
    for (int j = 0; j <= i; ++j) {
      // Sets this entry to the sum of the two above adjacent entries if they
      // exist.
      curr_row.emplace_back(0 < j && j < i ? pascal_triangle.back()[j - 1] +
                                                 pascal_triangle.back()[j]
                                           : 1);
    }
    pascal_triangle.emplace_back(curr_row);
  }
  return pascal_triangle;
}
```

**Complexity.** O(1 + 2 + … + n) = O(n²) time and space.

**Variants:** return only the n-th row using O(n) space (update one row in place, right to left).
