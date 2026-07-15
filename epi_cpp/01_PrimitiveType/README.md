# Primitive Types

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 1. See epilight_cpp_new.pdf for the full text. Original exposition — explanations, code, and examples are my own.*

## Overview

Every program ultimately manipulates values of some *type*: a set of possible values together with the operations defined on them. Languages ship with primitive types for booleans, integers, characters, and floating-point numbers, usually in several widths and signednesses. The *width* of a type is how many bits a variable of that type occupies — and it is not portable knowledge: a C++ `int` is commonly 32 bits but may be 64 depending on the implementation, whereas Java pins `int` at exactly 32 bits.

In interviews, "primitive types" is shorthand for *bit manipulation*. These problems test whether you can reason at the level of individual bits: shifting, masking, exploiting two's-complement arithmetic, and recognizing algebraic properties (like associativity of XOR) that unlock large speedups. The payoff pattern repeats throughout the chapter: a per-bit loop is the obvious O(n) answer, and the interview-quality answer processes many bits per step.

## Boot camp: counting set bits

**Implementations:** [C++](cpp/count_set_bits.cc) · [Python](python/count_bits.py)

**Problem.** Count how many bits of an unsigned integer are 1 (the *population count*).

**Idea.** Inspect the least-significant bit with `x & 1`, add it to a counter, and shift the word right by one. The loop condition `while (x)` stops as soon as no set bits remain — and note there is no hard-coded word size anywhere, so the same code works for 32- or 64-bit words.

***cpp/count_set_bits.cc***

```cpp
short CountSetBits(unsigned int x) {
  short count = 0;
  while (x) {
    count += x & 1;
    x >>= 1;
  }
  return count;
}
```

**Example.** For `x = 0b10110` (22), the loop sees bits 0, 1, 1, 0, 1 from the right and returns 3.

**Complexity.** Each iteration does O(1) work and consumes one bit position, so the time is O(n) for an n-bit word. Remember that big-O describes the *worst-case* input — here the all-ones word. The best case (input 0) exits immediately in O(1). The parity problem below develops several techniques (lowest-set-bit erasure, lookup tables, word-level XOR) that make this kind of bit counting much faster.

## Top tips

- **Own the bitwise operators.** `&`, `|`, `^`, `~`, `<<`, `>>` should feel as natural as `+` and `*`. XOR deserves special attention: it is its own inverse, and a large fraction of bit tricks reduce to XOR algebra.
- **Build masks portably.** Construct masks with expressions like `(1UL << k) - 1` rather than constants tied to a particular word width, so the code survives a change of type.
- **Memorize the lowest-set-bit identities.** `x & (x - 1)` clears the lowest set bit; `x & ~(x - 1)` isolates it. From these you can derive relatives: setting the lowest 0-bit, finding its index, and so on. They power many O(k) algorithms where k is the number of set bits.
- **Respect signedness.** Right-shifting signed negative values (arithmetic shift) and unsigned values (logical shift) behave differently. Prefer unsigned types for bit fiddling.
- **Cache small subproblems.** When an operation runs over enormous input volumes, brute-force all possible small inputs once into a lookup table, then answer each query with table lookups.
- **Exploit commutativity and associativity.** If an operation can be regrouped and reordered freely — XOR is the canonical case — you can process many bits in parallel with a single word-level instruction.

## Know your primitive types

- Numeric bounds live in `<limits>`: `numeric_limits<int>::min()`, `numeric_limits<float>::max()`, `numeric_limits<double>::infinity()`.
- Never compare floating-point values with bare `==`; use an absolute or relative tolerance appropriate to the magnitudes involved.
- `<cmath>` workhorses: `abs`, `fabs`, `ceil`, `floor`, `min`, `max`, `pow`, `log`, `sqrt`.
- Conversions between numbers, characters, and strings: `c - '0'` turns a digit character into its value, `'0' + d` goes the other way, `to_string(123)` formats an integer, `stoi("42")` parses one.
- `<random>` for writing tests: `uniform_int_distribution<> dis(1, 6)` draws an integer in [1, 6]; `uniform_real_distribution<double> dis(1.3, 2.9)` draws a real in [1.3, 2.9]; `generate_canonical<double, 10>` yields a value in [0, 1).
- `swap(x, y)` exchanges two values of any swappable type, including containers like `vector`.

## 1.1 Computing the parity of a word

**Implementations:** [C++](cpp/parity.cc) · [Python](python/parity.py)

**Problem.** The parity of a word is 1 when the number of set bits is odd and 0 when it is even — e.g., `0b1101` has parity 1 and `0b101101` has parity 0. Parity is the classic single-bit error-detection check in storage and communication. Computing it once for one word is easy; the task is to compute it for a *huge stream* of 64-bit words, so per-word cost matters enormously.

*Hint: precompute a lookup table — but obviously not one indexed by all 2⁶⁴ words.*

### Approach 1: bit-by-bit brute force — O(n)

Walk the word one bit at a time. Since only oddness matters, there is no need to count: XOR each bit into a 1-bit accumulator, which is exactly counting modulo 2.

***cpp/parity.cc***

```cpp
short Parity_brute_force(unsigned long long x) {
  short result = 0;
  while (x) {
    result ^= x & 1;
    x >>= 1;
  }
  return result;
}
```

Time O(n) for word size n; space O(1).

### Approach 2: iterate only over the set bits — O(k)

The first refinement improves the best and average cases by jumping directly from one set bit to the next. It rests on the identity `x & (x - 1)`, which erases the lowest set bit in a single step. Each iteration removes one set bit and flips the accumulator, so the loop runs exactly k times, where k is the population count.

***cpp/parity.cc***

```cpp
short Parity_iter(unsigned long long x) {
  short result = 0;
  while (x) {
    result ^= 1;
    x &= x - 1;  // erase the lowest set bit
  }
  return result;
}
```

**Why the identity works.** Take a nonzero x and consider its lowest set bit. Subtracting 1 turns that bit into 0 and every bit below it into 1, leaving the higher bits untouched (adding 1 back would restore the original — that is what "borrow" does). ANDing with the original x therefore zeroes exactly the lowest set bit and nothing else. Example with `x = 0b01011000`: `x - 1 = 0b01010111`, so `x & (x - 1) = 0b01010000`. And if x is 0, the subtraction wraps to all-ones and the AND still gives 0, so the identity is total.

The sibling identity `x & ~(x - 1)` *isolates* the lowest set bit instead of erasing it: continuing the example, `~(x - 1) = 0b10101000`, so `x & ~(x - 1) = 0b00001000` — a word whose single 1 sits exactly where x's lowest 1 was. Both identities are robust for unsigned and two's-complement values, and both are worth committing to memory.

Time O(k). On *sparse* words this is dramatically faster than Approach 1, and even on random inputs it wins measurably, since about half the bits are 0 on average.

### Approach 3: lookup table over fixed-width chunks — O(n/L)

When bit computations run at high volume, two levers dominate performance: processing multiple bits per operation, and caching results in an array-indexed table. Caching per 64-bit word is out of the question — 2⁶⁴ entries. But parity is *associative*: the bits can be grouped however we like without changing the answer. So split the 64-bit word into four non-overlapping 16-bit chunks, look up each chunk's parity in a precomputed table with 2¹⁶ = 65,536 entries, and XOR the four results. Sixteen is a sweet spot: the table stays small, and 16 divides 64 evenly, which keeps the code uniform.

A miniature version shows the mechanics. Let the table cover 2-bit chunks: it holds ⟨0, 1, 1, 0⟩ — the parities of `00`, `01`, `10`, `11`. To get the parity of `0b01100111`, split it into `01`, `10`, `01`, `11`; the lookups give 1, 1, 1, 0, and their XOR is 1 — correct, since the word has five set bits. One subtlety: after right-shifting a chunk into the low positions, the chunks *above* it are still present in the word. Using the shifted value directly as an index would run off the end of the table, so you must first AND with a chunk-sized mask (`0b11` in the miniature; `0xFFFF` for 16-bit chunks) to extract just the low chunk.

***cpp/parity.cc***

```cpp
short Parity_lookup(unsigned long long x) {
  constexpr int kChunkBits = 16;
  constexpr unsigned kChunkMask = 0xFFFF;
  return kParityTable[(x >> (3 * kChunkBits)) & kChunkMask] ^
         kParityTable[(x >> (2 * kChunkBits)) & kChunkMask] ^
         kParityTable[(x >> kChunkBits) & kChunkMask] ^
         kParityTable[x & kChunkMask];
}
```

The table itself is filled once at startup, e.g. `kParityTable[i] = kParityTable[i >> 1] ^ (i & 1)`.

Time O(n/L) per query for chunk width L, treating shifts and lookups as O(1); the one-time table construction is not counted. Space O(2^L).

### Approach 4: XOR folding — O(log n)

The final idea attacks the worst case directly. The XOR of two bits is 0 when they are equal and 1 otherwise; it is associative and commutative; and the XOR of a group of bits *is* their parity. Therefore the parity of a 64-bit word equals the parity of (top half) XOR (bottom half) — and that XOR of two 32-bit values costs one shift plus one word-level XOR instruction, replacing 32 bit-level steps. Repeat at widths 32, 16, 8, 4, 2, 1; after the final fold, the answer sits in the least-significant bit. The higher bits accumulate meaningless junk along the way, so the closing `& 1` extraction is mandatory.

Tracing an 8-bit example, `x = 0b10110010` (four set bits, so parity 0):

| fold | XOR performed | surviving low bits |
|------|---------------|--------------------|
| by 4 | `1011 ^ 0010`  | `1001` |
| by 2 | `10 ^ 01`      | `11`   |
| by 1 | `1 ^ 1`        | `0`    |

***cpp/parity.cc***

```cpp
short Parity_xor(unsigned long long x) {
  x ^= x >> 32;
  x ^= x >> 16;
  x ^= x >> 8;
  x ^= x >> 4;
  x ^= x >> 2;
  x ^= x >> 1;
  return x & 0x1;
}
```

Time O(log n); space O(1).

### Combining and comparing the approaches

The techniques compose: fold from 64 bits down to 16, then finish with a single table lookup instead of four more folds. Real-world throughput depends on the data — the O(k) refinement is superb on sparse words — but on random inputs the rough ordering is: Approach 2 beats brute force by around 20%, the lookup table is several times faster again, and word-level folding buys roughly another factor of two.

**Variants:** using only bitwise operators, equality checks, and boolean operators, solve each in O(1):
- Right-propagate the rightmost set bit — e.g., `0b01010000` becomes `0b01011111`.
- Compute x mod a power of two without division — e.g., 77 mod 64 = 13.
- Test whether x is a power of two (true for 1, 2, 4, 8, ...; false for everything else).

## 1.2 Swap bits

**Implementations:** [C++](cpp/swap_bits.cc) · [Python](python/swap_bits.py)

**Problem.** Given a 64-bit integer x and two bit indices i and j, return x with the bits at positions i and j exchanged.

*Hint: when does the swap actually change anything?*

### Brute force: extract, clear, rewrite

Save bit i and bit j (`(x >> i) & 1`), clear both positions with AND masks, then OR each saved bit into the other position. Correct, but it takes a handful of mask operations and hides the real structure of the problem.

### Optimized: flip both bits, and only when they differ

A swap is visible only when the two bits differ — swapping two equal bits is a no-op. And when they *do* differ, swapping them is the same as flipping both, which is a single XOR against a two-bit mask:

```
index :  5 4 3 2 1 0
x     =  1 0 0 1 1 0        swap i = 0, j = 5  →  bits differ, flip both
mask  =  1 0 0 0 0 1
x^mask=  0 0 0 1 1 1
```

***cpp/swap_bits.cc***

```cpp
long long swap_bits(long long x, int i, int j) {
  // Extract the i-th and j-th bits, and see if they differ.
  if (((x >> i) & 1) != ((x >> j) & 1)) {
    // i-th and j-th bits differ. We will swap them by flipping their values.
    // Select the bits to flip with bit_mask. Since
    // x^1 = 0 when x = 1
    // x^1 = 1 when x = 0
    // we can perform the flip by XOR.
    unsigned long long bit_mask = (1L << i) | (1L << j);
    x ^= bit_mask;
  }
  return x;
}
```

**Complexity.** O(1) time, O(1) space — independent of the word size.

## 1.3 Reverse bits

**Implementations:** [C++](cpp/reverse_bits.cc) · [Python](python/reverse_bits.py)

**Problem.** Return the 64-bit word whose bits are those of x in reverse order: bit 0 trades places with bit 63, bit 1 with bit 62, and so on. As with parity, assume this runs over a huge stream of words, so per-word cost dominates.

*Hint: the parity problem's lookup-table machinery transfers directly.*

### Approach 1: bit-by-bit — O(n)

Peel bits off the low end of x and push them into the low end of the result; after n rounds the first bit peeled has migrated to the top.

***cpp/reverse_bits.cc***

```cpp
unsigned long long reverse_bits1(unsigned long long x) {
  unsigned long long reversed = 0;
  for (int i = 0; i < 64; ++i) {
    reversed = (reversed << 1) | (x & 1);
    x >>= 1;
  }
  return reversed;
}
```

Time O(n), space O(1). Note the loop must run exactly 64 times and shift *before* ORing — an off-by-one here silently drops bit 63.

### Approach 2: swap symmetric pairs in place — n/2 iterations

Reversal is a sequence of independent swaps: bit i with bit n−1−i for every i in the lower half. Each swap is exactly the 1.2 primitive — test whether the pair differs, and XOR-flip both when it does.

***cpp/reverse_bits.cc***

```cpp
unsigned long reverse_bits2(unsigned long long x, int n = 63) {
  int i = n, j = 0;
  while (i > j) {
    if (((x >> i) & 1) != ((x >> j) & 1)) {  // check if i and j bits are different
      x ^= (1ULL << i) | (1ULL << j);  // swap(i, j) bits
    }
    --i; ++j;
  }
  return x;
}
```

Still O(n) time, but half the iterations of Approach 1, and the parameter n lets the same routine reverse narrower words — which Approach 3 exploits.

### Approach 3: 16-bit chunk lookup table — O(n/L)

Split the word into four 16-bit chunks. Reversing the whole word means reversing each chunk *and* mirroring the chunk order:

```
x          = [  A   |  B   |  C   |  D   ]      16-bit chunks, A = bits 63..48
reverse(x) = [ rev(D) | rev(C) | rev(B) | rev(A) ]
```

Precompute a 2¹⁶-entry table of every 16-bit value's reversal (the table is built with `reverse_bits2(i, 15)`), then answer each query with four lookups, three shifts, and three ORs.

***cpp/reverse_bits.cc***

```cpp
static array<unsigned long long, 1 << 16> precomputed_reverse;

void create_precomputed_table() {
  for (int i = 0; i < (1 << 16); ++i) {
    precomputed_reverse[i] = reverse_bits2(i, 15);
  }
}

unsigned long long ReverseBits(unsigned long long x) {
  const int kMaskSize = 16;
  const int kBitMask = 0xFFFF;
  return precomputed_reverse[x & kBitMask] << (3 * kMaskSize) |
         precomputed_reverse[(x >> kMaskSize) & kBitMask] << (2 * kMaskSize) |
         precomputed_reverse[(x >> (2 * kMaskSize)) & kBitMask] << kMaskSize |
         precomputed_reverse[(x >> (3 * kMaskSize)) & kBitMask];
}
```

**Complexity.** O(n/L) per query for chunk width L; O(2^L) space and a one-time O(2^L) build. Exactly the parity Approach 3 recipe — the only twist is that the chunks land in mirrored positions.

**Python note.** Python integers are arbitrary-precision, so there is no implicit 64-bit word to reverse within — the width has to be pinned explicitly. [python/reverse_bits.py](python/reverse_bits.py) uses the same 16-bit lookup-table recipe, with the table stored as a precomputed literal.

## 1.4 Find a closest integer with the same weight

**Implementations:** [C++](cpp/closest_int_same_weight.cc) · [Python](python/closest_int_same_weight.py)

**Problem.** The *weight* of a nonnegative integer is its number of set bits. Given x (neither 0 nor all 1s), find a y ≠ x with the same weight that minimizes |x − y|.

*Hint: which two bits can you flip while changing the value as little as possible?*

### Brute force: search outward

Test x−1, x+1, x−2, x+2, … until a value with equal weight appears. Simple, but the search radius is unbounded in general — for x = 2^k the nearest same-weight neighbor is 2^(k−1), a distance of 2^(k−1) away — so this can take time exponential in the word size.

### Optimized: swap the lowest differing adjacent pair — O(n)

Preserving the weight means any change must flip one bit from 1 to 0 and another from 0 to 1. Flipping positions k₁ > k₂ changes the value by 2^k₁ − 2^k₂, so the difference is minimized by making the two positions as low and as close together as possible: adjacent, and as far right as we can find a `10` or `01` pattern. Scan from the LSB for the first i where bit i differs from bit i+1, and swap that pair with an XOR:

```
x      =  0 1 1 0   (6)     lowest adjacent differing pair: bits 0 and 1
                            swap them (XOR with 0b0011)
result =  0 1 0 1   (5)     weight preserved, |6 − 5| = 1
```

***cpp/closest_int_same_weight.cc***

```cpp
unsigned long long ClosestIntSameBitCount(unsigned long long x) {
  const static int kNumUnsignedBits = 64;
  for (int i = 0; i < kNumUnsignedBits - 1; ++i) {
    if (((x >> i) & 1) != ((x >> (i + 1)) & 1)) {
      x ^= (1UL << i) | (1UL << (i + 1));  // Swaps bit-i and bit-(i + 1).
      return x;
    }
  }

  // Throw error if all bits of x are 0 or 1.
  throw invalid_argument("All bits are 0 or 1");
}
```

**Complexity.** O(n) time, O(1) space. (An O(1)-time variant exists: isolate the lowest set bit and the lowest unset bit with the 1.1 identities and manipulate them directly.)

## 1.5 Compute x × y without arithmetical operators

**Implementations:** [C++](cpp/primitive_multiply.cc) · [Python](python/primitive_multiply.py)

**Problem.** Multiply two nonnegative integers using only assignment, bitwise operators, shifts, and equality tests — no `+`, no `*`.

*Hint: do grade-school multiplication in base 2, then build addition itself out of XOR and carries.*

### Brute force: repeated addition

Initialize the result to 0 and add y to it x times. Even granting an Add primitive, that is 2^n additions in the worst case — hopeless for 64-bit inputs.

### Optimized: shift-and-add with a bitwise adder — O(n²)

Two layers, each a classic:

1. **Shift-and-add.** Write x in binary: x·y = Σ over set bits k of (y << k). Walk the bits of x, and whenever the current bit is 1, add the appropriately shifted y into the running sum — at most n additions instead of 2^n.
2. **Addition from bitwise operations.** `a ^ b` is the sum ignoring carries, and `(a & b) << 1` is exactly the carries (a carry is generated where both bits are 1, and lands one position left). Loop until the carry word is 0 — a ripple-carry adder in software:

```
Add(7, 6):
  a = 0111, b = 0110
  a ^ b            = 0001      (sum without carries)
  (a & b) << 1     = 1100      (carries)
  next: a = 0001, b = 1100  →  a ^ b = 1101, (a & b) << 1 = 0000
  done: 1101 = 13
```

***cpp/primitive_multiply.cc***

```cpp
// O(n^2) since Add is O(n) time, O(1) space
unsigned long long Multiply(unsigned long long x, unsigned long long y) {
  unsigned long long sum = 0;
  while (x) {
    // Examines each bit of x.
    if (x & 1) {
      sum = Add(sum, y);
    }
    x >>= 1, y <<= 1;
  }
  return sum;
}

// O(n) time, O(1) space
unsigned long long Add(unsigned long long a, unsigned long long b) {
  while (b) {
    unsigned long long carry = a & b;
    a = a ^ b;
    b = carry << 1;
  }
  return a;
}
```

**Example.** 5 × 3: x = 0b101, so the sum collects `3 << 0` = 3 and `3 << 2` = 12, giving 15.

**Complexity.** Each Add is O(n) (a carry can ripple the full width), and up to n adds run, so O(n²) overall; space O(1).

## 1.6 Compute x / y using only addition, subtraction, and shifts

**Implementations:** [C++](cpp/primitive_divide.cc) · [Python](python/primitive_divide.py)

**Problem.** Compute the quotient ⌊x / y⌋ for nonnegative integers with y > 0, without using division or multiplication.

*Hint: subtracting y once per loop is far too slow — subtract the largest 2^k·y you can.*

### Brute force: repeated subtraction

Count how many times y can be subtracted from x. That is x/y iterations — exponential in the bit width when y is small.

### Optimized: peel off the largest 2^k·y — O(n)

Find the largest k with 2^k·y ≤ x; that contributes 2^k to the quotient in one step, leaving a remainder smaller than 2^k·y. Because the usable k only shrinks as the remainder shrinks, start from the top and only ever shift down — the shifting work across the whole run totals O(n):

```
11 / 3:
  3·2² = 12 > 11   → shift down
  3·2¹ =  6 ≤ 11   → quotient += 2¹ = 2,  remainder = 5
  3·2¹ =  6 >  5   → shift down
  3·2⁰ =  3 ≤  5   → quotient += 2⁰ = 1,  remainder = 2 < 3 → stop
  quotient = 3
```

***cpp/primitive_divide.cc***

```cpp
int Divide(int x, int y) {
  int result = 0;
  int power = 32;
  unsigned long long y_power = static_cast<unsigned long long>(y) << power;
  while (x >= y) {
    while (y_power > x) {
      y_power >>= 1;
      --power;
    }

    result += 1 << power;
    x -= y_power;
  }
  return result;
}
```

**Complexity.** Each outer iteration commits one quotient bit and the shift-down is monotone, so O(n) time, O(1) space.

### Alternative: locate each power by binary search

Instead of descending linearly to the right k, binary-search the exponent range for it (guarding against `y << k` overflowing), commit that power, and recurse on the remainder:

***cpp/primitive_divide.cc***

```cpp
unsigned divide_bsearch(unsigned x, unsigned y) {
  if (x < y) {
    return 0;
  }

  int power_left = 0;
  int power_right = sizeof(unsigned) << 3;
  int power_mid = -1;
  while (power_left < power_right) {
    int tmp = power_mid;
    power_mid = power_left + ((power_right - power_left) >> 1);
    if (tmp == power_mid) {
      break;
    }
    unsigned yshift = y << power_mid;
    if ((yshift >> power_mid) != y) {
      // yshift overflowed, use a smaller shift.
      power_right = power_mid;
      continue;
    }
    if ((y << power_mid) > x) {
      power_right = power_mid;
    } else if ((y << power_mid) < x) {
      power_left = power_mid;
    } else {
      return (1U << power_mid);
    }
  }
  unsigned part = 1U << power_left;
  return part | divide_bsearch(x - (y << power_left), y);
}
```

O(log n) per located power, with at most one location pass per set bit of the quotient. The linear-descent version is simpler and already optimal overall; this one mainly demonstrates the technique.

## 1.7 Compute x^y

**Implementations:** [C++](cpp/power_x_y.cc) · [Python](python/power_x_y.py)

**Problem.** Compute x raised to the integer power y, where x is a double and y may be negative.

*Hint: x^(a+b) = x^a · x^b — read y in binary.*

### Brute force: repeated multiplication

Multiply x into an accumulator |y| − 1 times: O(2^n) multiplications for an n-bit exponent.

### Optimized: repeated squaring — O(log y)

Write y in binary: x^y is the product of x^(2^k) over y's set bits. Squaring x repeatedly produces exactly those factors — x, x², x⁴, x⁸, … — so walk the exponent's bits from the bottom, multiplying the current square into the result whenever the bit is 1. A negative exponent reduces to the positive case via x → 1/x, y → −y.

Trace for x = 2, y = 13 (binary 1101):

| step | power (binary) | low bit | x holds | result |
|------|----------------|---------|---------|--------|
| 1    | 1101           | 1       | 2 → 4   | 2      |
| 2    | 110            | 0       | 4 → 16  | 2      |
| 3    | 11             | 1       | 16 → 256| 32     |
| 4    | 1              | 1       | —       | 8192   |

2¹³ = 8192 ✓

***cpp/power_x_y.cc***

```cpp
double Power(double x, int y) {
  double result = 1.0;
  long long power = y;
  if (y < 0) {
    power = -power, x = 1.0 / x;
  }
  while (power) {
    if (power & 1) {
      result *= x;
    }
    x *= x, power >>= 1;
  }
  return result;
}
```

**Complexity.** One squaring per exponent bit and at most one extra multiply each: O(log y) multiplications, O(1) space. (Note `power` is a `long long` so that negating `INT_MIN` is safe.)

## 1.8 Reverse digits

**Implementations:** [C++](cpp/reverse_digits.cc) · [Python](python/reverse_digits.py)

**Problem.** Reverse a decimal integer's digits, keeping the sign: 42 → 24, −314 → −413.

*Hint: you never need the string form — `% 10` hands you the digits.*

### Brute force: via a string

Format x as a string, reverse it (minding the '−'), and parse it back. Works, but allocates and does three passes.

### Optimized: arithmetic peel — O(d)

Peel the least-significant digit with `x % 10` and append it to the result with `result * 10 + digit`; drop the digit with `x /= 10`. In C++ the `%` operator preserves the dividend's sign (−256 % 10 = −6), so negative inputs flow through the same loop with no special-casing — every appended digit is negative, building a negative result.

```
x = −256:   result = 0 → −6 → −65 → −652
```

***cpp/reverse_digits.cc***

```cpp
long long Reverse(int x) {
  long long result = 0;
  while (x) {
    // If x is an negative, x % 10 is the negative of least significant digit.
    // For example, -256 % 10 = -6.
    result = result * 10 + x % 10;
    x /= 10;
  }
  return result;
}
```

**Complexity.** O(d) for d digits (d ≈ log₁₀ x); O(1) space. The `long long` return type matters: reversing a valid `int` like 2,147,483,647 exceeds `int` range.

**Python note.** C++'s `%` keeps the dividend's sign, which is what lets the loop handle negatives directly. Python's `%` always returns a non-negative value, so [python/reverse_digits.py](python/reverse_digits.py) loops on `abs(x)` instead and restores the sign at the end.

## 1.9 Check if a decimal integer is a palindrome

**Implementations:** [C++](cpp/is_number_palindromic.cc) · [Python](python/is_number_palindromic.py)

**Problem.** Decide whether a decimal integer reads the same forwards and backwards: 121, 1551, 7 → yes; 10, −121 → no (a negative number's leading '−' breaks symmetry by definition).

*Hint: compare one digit from each end, then peel both off.*

### Brute force: via a string

Format x and walk two pointers inward — O(d) time but O(d) extra space.

### Optimized: numeric two-pointer — O(d) time, O(1) space

The number has d = ⌊log₁₀ x⌋ + 1 digits. The most significant digit is `x / 10^(d−1)` and the least significant is `x % 10`. Compare them; if equal, strip *both* — `x %= msd_mask` removes the top digit, `x /= 10` removes the bottom one — and shrink the mask by two orders of magnitude. Repeat d/2 times:

```
x = 157751,  msd_mask = 100000
  [1] 5775 [1]   1 == 1  → strip both → x = 5775, mask = 1000
  [5]  77  [5]   5 == 5  → strip both → x =   77, mask = 10
  [7]      [7]   7 == 7  → palindrome ✓
```

***cpp/is_number_palindromic.cc***

```cpp
bool IsPalindromeNumber(int x) {
  if (x <= 0) {
    return x == 0;
  }

  const int num_digits = static_cast<int>(floor(log10(x))) + 1;
  int msd_mask = static_cast<int>(pow(10, num_digits - 1));
  for (int i = 0; i < (num_digits / 2); ++i) {
    if (x / msd_mask != x % 10) {
      return false;
    }
    x %= msd_mask;  // Remove the most significant digit of x.
    x /= 10;        // Remove the least significant digit of x.
    msd_mask /= 100;
  }
  return true;
}
```

**Complexity.** O(d) time, O(1) space.

## 1.10 Generate uniform random numbers

**Implementations:** [C++](cpp/uniform_random_number.cc) · [Python](python/uniform_random_number.py)

**Problem.** Using only a random bit generator (a fair coin, `ZeroOneRandom()`), produce an integer uniformly distributed in [lower_bound, upper_bound].

*Hint: how do you roll a fair die with coin flips?*

### Why the obvious approach is biased

There are t = upper − lower + 1 outcomes. Generating some bits and taking the value mod t is *not* uniform unless t is a power of two: with 3 bits and t = 6, the values 0 and 1 each arise from two bit patterns while 2–5 arise from one — a 2:1 bias.

### Rejection sampling — expected O(log t)

Concatenate k = ⌈log₂ t⌉ random bits into a number uniform over [0, 2^k). If it lands in [0, t), accept it (every value in that range is exactly equally likely); otherwise throw it away and redraw:

```
[lower, upper] = [1, 6],  t = 6,  k = 3 bits → raw value in 0..7
  000..101  (0..5) → accept → add lower → result in 1..6, each p = 1/6
  110, 111  (6, 7) → reject → redraw all 3 bits
```

Since 2^k < 2t, the acceptance probability exceeds 1/2, so the expected number of attempts is under 2 and the probability of needing many retries decays geometrically.

***cpp/uniform_random_number.cc***

```cpp
int UniformRandom(int lower_bound, int upper_bound) {
  int number_of_outcomes = upper_bound - lower_bound + 1, result;
  do {
    result = 0;
    for (int i = 0; (1 << i) < number_of_outcomes; ++i) {
      // ZeroOneRandom() is the provided random number generator.
      result = (result << 1) | ZeroOneRandom();
    }
  } while (result >= number_of_outcomes);
  return result + lower_bound;
}
```

**Complexity.** Each attempt costs O(log t) coin flips; expected total O(log t). There is no deterministic worst-case bound — only a probabilistic one.

## 1.11 Rectangle intersection

**Implementations:** [C++](cpp/rectangle_intersection.cc) · [Python](python/rectangle_intersection.py)

**Problem.** Two rectangles are XY-aligned (sides parallel to the axes), each given as (x, y, width, height) with (x, y) the lower-left corner. Return their intersection rectangle, or a sentinel (0, 0, −1, −1) if they do not intersect.

*Hint: a 2-D intersection test is just two 1-D interval-overlap tests.*

### Idea: decompose by axis

Two rectangles intersect exactly when their x-projections overlap *and* their y-projections overlap. Two intervals [a₁, b₁] and [a₂, b₂] overlap iff each one starts no later than the other ends: `a₁ ≤ b₂ && a₂ ≤ b₁`. When both axes overlap, the intersection is the box from the larger of the starts to the smaller of the ends on each axis:

```
 y ▲
   │            ┌───────────────┐
   │            │ R2            │
   │    ┌───────┼─────┐         │
   │    │  R1   │▒▒▒▒▒│         │
   │    │       └─────┼─────────┘
   │    └─────────────┘
   └────────────────────────────▶ x
        ▒ = intersection: [max x, min x+w] × [max y, min y+h]
```

***cpp/rectangle_intersection.cc***

```cpp
struct Rect {
  int x, y, width, height;
};

Rect IntersectRectangle(const Rect& r1, const Rect& r2) {
  if (!IsIntersect(r1, r2)) {
    return {0, 0, -1, -1};  // No intersection.
  }
  return {max(r1.x, r2.x), max(r1.y, r2.y),
          min(r1.x + r1.width, r2.x + r2.width) - max(r1.x, r2.x),
          min(r1.y + r1.height, r2.y + r2.height) - max(r1.y, r2.y)};
}

bool IsIntersect(const Rect& r1, const Rect& r2) {
  return r1.x <= r2.x + r2.width && r1.x + r1.width >= r2.x &&
         r1.y <= r2.y + r2.height && r1.y + r1.height >= r2.y;
}
```

**Complexity.** O(1) time and space. Note the comparisons are inclusive (`<=`/`>=`), so rectangles that merely share an edge or a corner count as intersecting and yield a zero-width or zero-height rectangle.

