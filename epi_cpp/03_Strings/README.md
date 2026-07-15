# Strings

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 3. See epilight_cpp_new.pdf for the full text. Original exposition — explanations, code, and examples are my own.*

## Overview

Strings show up everywhere — scripting, web work, bioinformatics, log processing — which is why interviewers love them. Structurally a string is just an array whose elements happen to be characters, but it deserves separate study because a whole family of operations is meaningful only for strings: lexicographic comparison, concatenation, splitting on delimiters, substring search, find-and-replace, parsing. You should understand how strings are laid out in memory and be quick with the basics — comparing, copying, joining, splitting, matching.

The heavyweight string algorithms (suffix structures, pattern matching, edit distance) lean on hash tables and dynamic programming and belong to later chapters; the problems here are solvable with elementary technique plus care. The recurring interview themes are the same ones arrays have — avoid extra allocation, mind the direction you build in — plus one string-specific staple: digit-character arithmetic for parsing and formatting numbers.

## Boot camp: palindrome check

**Problem.** Decide whether a string reads identically forward and backward.

**Idea.** The tempting solution — build the reversed string and compare — costs O(n) extra space. Entirely unnecessary: run two indices toward each other, one from each end, comparing the characters they pass over. A mismatch anywhere means not a palindrome; if the indices cross without a mismatch, it is one. The `i < j` loop condition transparently handles both parities of length — for odd length the middle character is never compared (it cannot disqualify anything), and for even length every character is checked exactly once.

```cpp
bool IsPalindromic(const string& s) {
  for (int i = 0, j = s.size() - 1; i < j; ++i, --j) {
    if (s[i] != s[j]) {
      return false;
    }
  }
  return true;
}
```

**Example.** "noon": compare `n`/`n`, then `o`/`o`, indices cross → palindrome. "north": compare `n`/`h` → mismatch at the first step, return false immediately.

**Complexity.** O(n) time, O(1) space — and it can quit early on the first mismatch.

## Top tips

- **In-place solutions exist here too.** Like arrays, string problems usually admit a quick O(n)-space brute force; the more impressive solution operates on the string itself and cuts extra space to O(1).
- **Never build a mutable string from the front.** Every front insertion shifts the whole tail. If output naturally emerges front-first, see whether you can instead produce it back-first — or append everything and reverse once at the end.

## Know your string libraries

- The `string` class methods to know cold: `append("Gauss")`, `push_back('c')`, `pop_back()`, `insert(s.begin() + shift, "Gauss")`, `substr(pos, len)`, `compare("Gauss")`.
- A `string` is organized like a dynamic array: operations at the back (`push_back`, `pop_back`) are cheap, while insertion in the middle (`insert(s.begin() + middle, "Gauss")`) shifts everything after the insertion point and is slow.
- The relational operators `<`, `<=`, `>`, `>=`, `==` all work on strings, and `==` compares *contents* (logical equality), not pointer identity.

## 3.1 Interconvert strings and integers

**Problem.** Implement both directions of decimal conversion yourself: a function turning an integer (possibly negative) into its string representation, and one turning such a string back into an integer. Library shortcuts — `stoi` in C++, `parseInt` in Java, `int()` in Python — are off limits.

*Hint: build the result one digit at a time.*

### Integer → string

A single-digit number is immediate: emit the one character for it. The general case falls to digit peeling: for positive x, `x % 10` is the least-significant decimal digit and `x / 10` is the number formed by the remaining digits. Repeat until nothing is left.

The catch is *order*: peeling produces digits least-significant first. Converting 583 yields 3 (leaving 58), then 8 (leaving 5), then 5 — i.e., "385" if appended naively. Prepending each digit would fix the order but costs O(length) per digit, since a front insertion shifts the whole string. The efficient pattern: **append** each digit as it appears, then **reverse once** at the end — total O(n) instead of O(n²).

Two edge cases round it out. For negative x, remember the sign, work with absolute digit values, and tack `'-'` on before the final reverse (so it lands at the front). For x = 0, a `while (x)` loop would emit nothing at all; a do-while guarantees at least one digit, so 0 correctly becomes "0".

```cpp
string IntToString(int x) {
  bool is_negative = x < 0;
  string s;
  do {
    s += '0' + abs(x % 10);
    x /= 10;
  } while (x != 0);
  if (is_negative) {
    s += '-';
  }
  return {s.rbegin(), s.rend()};  // reverse into the final result
}
```

**Example.** x = −276: peel 6, 7, 2 giving `"672"`; append the sign → `"672-"`; reverse → `"-276"`.

### String → integer

Recall what positional notation means: the string d₂d₁d₀ encodes 10²·d₂ + 10¹·d₁ + d₀. The brute-force reading starts at the *rightmost* digit and accumulates dᵢ · 10ⁱ, maintaining the growing power of ten by one multiplication per step.

The cleaner formulation scans *left to right* with a single running value (Horner's rule): for each new digit, multiply the partial result by 10 and add the digit. Parsing "508": r = 5, then r = 5·10 + 0 = 50, then r = 50·10 + 8 = 508. No explicit powers of ten anywhere. A leading `'-'` is noted, skipped, and applied by negation at the end.

```cpp
int StringToInt(const string& s) {
  int result = 0;
  for (int i = (s[0] == '-' ? 1 : 0); i < (int)s.size(); ++i) {
    result = result * 10 + (s[i] - '0');
  }
  return s[0] == '-' ? -result : result;
}
```

**Complexity.** Both directions do constant work per digit: O(n) time in the digit count, O(1) space beyond the output.

## 3.2 Base conversion

**Problem.** Positional notation generalizes past base 10: in base b, the string "a₍ₖ₋₁₎ … a₁a₀" (each 0 ≤ aᵢ < b) encodes a₀·b⁰ + a₁·b¹ + ⋯ + a₍ₖ₋₁₎·b^(k−1). Write a converter that takes a string encoding an integer in base b₁ and two integers b₁, b₂ (both in [2, 16]) and returns the string encoding the same integer in base b₂, using 'A'–'F' for digit values 10–15. Negative inputs must work.

*Hint: which base can you convert to and from most easily?*

### Approach 1: through unary — impractical

In principle you could expand the number into unary (that many 1s) and re-group the 1s into powers of b₂. It "works," but implementing it is awkward and the time and space are exponential in the input length — a non-starter, mentioned only to be dismissed.

### Approach 2: through the machine's integer type

The key observation: every language already gives you one base converter for free — the native integer type, with its multiply, add, divide, and modulus. So *reduce* the problem to two conversions through that middleman:

1. **Read (base b₁ → int):** scan the string left to right, Horner-style: `value = value * b1 + digit`, mapping characters '0'–'9' to 0–9 and 'A'–'F' to 10–15.
2. **Write (int → base b₂):** peel digits with `value % b2` and `value / b2` until the value hits zero, mapping values ≥ 10 back to letters. As in Problem 3.1, digits emerge least-significant first — recursion (emit higher digits first, then this one) or append-and-reverse restores the order.

Handle a leading `'-'` by stripping it on the way in and re-attaching it on the way out, and special-case value 0, which the peeling loop would otherwise render as the empty string.

```cpp
string ConvertBase(const string& num_as_string, int b1, int b2) {
  bool is_negative = num_as_string.front() == '-';
  int value = 0;
  for (size_t i = is_negative ? 1 : 0; i < num_as_string.size(); ++i) {
    char c = num_as_string[i];
    value = value * b1 + (isdigit(c) ? c - '0' : c - 'A' + 10);
  }
  return (is_negative ? "-" : "") +
         (value == 0 ? "0" : BuildInBase(value, b2));
}

string BuildInBase(int value, int base) {  // value > 0
  if (value == 0) {
    return "";
  }
  int d = value % base;
  char digit_char = d >= 10 ? 'A' + d - 10 : '0' + d;
  return BuildInBase(value / base, base) + digit_char;
}
```

**Example.** Convert "134" from base 5 to base 12. Reading: v = 1, then 1·5 + 3 = 8, then 8·5 + 4 = 44. Writing in base 12: 44 mod 12 = 8, 44 / 12 = 3; 3 mod 12 = 3, 3 / 12 = 0 → digits 3, 8 → "38". A letter-digit case: "7B" in base 13 is 7·13 + 11 = 102; in base 8 that is 102 = 1·64 + 4·8 + 6 → "146".

**Complexity.** Reading performs n multiply-adds for an n-digit input. The value is at most b₁ⁿ, so writing performs about log₍b₂₎(b₁ⁿ) = n·log₍b₂₎ b₁ divide-mod steps. Total O(n(1 + log₍b₂₎ b₁)) time. The problem is also a textbook illustration of *reduction*: solve a new problem by transforming it into one you can already solve.
