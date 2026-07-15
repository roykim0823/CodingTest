# Sorting

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 10. See epilight_cpp_new.pdf for the full text.*

## Overview

Sorting — arranging a collection into increasing or decreasing order — shows up in interviews in two distinct roles. Sometimes sorting *is* the problem (design a custom sort, or sort under unusual constraints); far more often it is a preprocessing step that turns a hard problem into an easy one, because sorted data supports binary search, two-pointer scans, and adjacency arguments ("equal items are now next to each other"). This chapter reviews the classic algorithms and their trade-offs, when an O(n) specialized sort applies, and how to drive `std::sort` with custom comparators. The sampler contains two problems: intersecting sorted arrays (10.1) and the interval-overlap sweep (10.2).

### Algorithm landscape

- Naive sorts (bubble, selection, insertion as usually written) run in O(n²). The serious general-purpose algorithms run in O(n log n), and the three to know are:
  - **Heapsort** — in-place (O(1) extra space) but not stable.
  - **Merge sort** — stable but not in-place (needs an auxiliary buffer).
  - **Quicksort** — fastest in practice, but O(n²) in the worst case and not stable.
  A well-tuned quicksort is the default best choice; the others win under specific constraints.
- Definitions worth having crisp: an **in-place** sort uses O(1) extra space; a **stable** sort preserves the original relative order of equal elements.
- Specialized situations beat O(n log n):
  - **Tiny inputs** (around ten elements): insertion sort is simpler *and* faster than the asymptotically superior algorithms — which is why library sorts switch to it for small subranges.
  - **Almost-sorted input**, where every element is at most k slots from its final position: push elements through a min-heap of size k for O(n log k).
  - **Small key universe** (e.g., bytes in [0..255]): counting sort — tally how many elements are smaller than each value — runs in O(n). Keep the tallies in an array when the max key is comparable to n, or in a BST keyed by value with frequency counts otherwise.
  - **Many duplicate keys**: insert into a BST keyed by value, chaining equal elements in a list per node; an inorder walk then emits the sorted result.
- Stability isn't free: most sorts (and most library sorts) are unstable. If you need stability, use merge sort carefully implemented, or augment each key with its original index as a tiebreaker.
- Most sorting is comparison-based, but when keys have numeric structure you can sort on the digits directly — **radix sort**.
- Heaps are frequent companions to sorting problems: a max-heap (min-heap) supports O(log n) insertion, O(1) access to the maximum (minimum), and O(log n) deletion of it.

## Boot camp

Being fluent with the library sort — especially custom comparators — is table stakes. Consider a student record with a name and a GPA, where the "natural" ordering compares names via `operator<`. Sorting by name then needs no comparator at all; sorting by GPA requires passing one explicitly:

```cpp
struct Student {
  std::string name;
  double gpa;

  bool operator<(const Student& other) const { return name < other.name; }
};

void SortByName(std::vector<Student>* students) {
  std::sort(students->begin(), students->end());  // uses Student::operator<
}

void SortByDescendingGpa(std::vector<Student>* students) {
  std::sort(students->begin(), students->end(),
            [](const Student& a, const Student& b) { return a.gpa > b.gpa; });
}
```

(Note the comparator uses strict `>`, never `>=` — `std::sort` requires a strict weak ordering, and a comparator that returns true for equal elements is undefined behavior.)

Any reasonable library sort runs in O(n log n) on an n-element array. Most are quicksort variants, which allocate no auxiliary buffer — the space cost is the O(log n) recursion stack.

## Top tips

- Sorting problems split into two flavors: (1) sort to make a later step easy — reach for the library sort, possibly with a custom comparator; (2) design a bespoke sorting routine — reach for a BST, a heap, or an array indexed by key value.
- Many problems become both easier to *understand* and easier to *solve* once the input is sorted; the most natural trigger is input with an inherent ordering, sorted as a preprocessing step to accelerate searching.
- Watch for specialized input — a small range of key values, or few distinct keys — where an O(n) sort replaces the generic O(n log n) one.
- Sorting frequently achieves the same result as a brute-force approach in *less space*, e.g., replacing a hash-based dedup/join with an in-place sort plus linear scan.
- What to sort *on* can be the whole problem: given intervals, should you order by start point or end point? (Problem 10.2 turns on exactly this choice.)

## Know your sorting libraries

- Arrays/vectors: `std::sort` from `<algorithm>`. Linked lists: the member function `std::list::sort` (node-splicing; iterator-based `std::sort` doesn't work on lists).
- Time complexity is O(n log n). The standard makes no space guarantee; typical implementations are quicksort/introsort variants that allocate nothing and use O(log n) call-stack space.
- Both `sort()` and `list::sort()` work out of the box on element types providing `operator<`.
- Both also accept an explicit comparator object (function, functor, or lambda) — see the boot camp for the syntax.

## 10.1 Compute the intersection of two sorted arrays

Search engines keep an *inverted index*: each word maps to the sorted array of document IDs containing it. A multi-word query is answered by intersecting the per-word arrays, and that intersection is the computational bottleneck. So: given two sorted integer arrays, possibly containing duplicates, return a duplicate-free array of the values present in both. For example, A = ⟨1, 2, 2, 4, 6⟩ and B = ⟨2, 2, 4, 5⟩ should yield ⟨2, 4⟩.

*Hint: solve it first when one array is orders of magnitude shorter than the other; then when they are about the same length.*

**Approach 1 — loop join, O(mn).** For each element of A (skipping any element equal to its predecessor, which handles output dedup), linearly scan B for it.

```cpp
std::vector<int> IntersectSortedArrays(const std::vector<int>& A,
                                       const std::vector<int>& B) {
  std::vector<int> result;
  for (size_t i = 0; i < A.size(); ++i) {
    if ((i == 0 || A[i] != A[i - 1]) &&
        std::find(B.begin(), B.end(), A[i]) != B.end()) {
      result.push_back(A[i]);
    }
  }
  return result;
}
```

Time O(mn) for lengths m and n — the sortedness is completely wasted.

**Approach 2 — binary search the longer array, O(m log n).** Same outer loop, but since B is sorted, membership testing is a binary search:

```cpp
std::vector<int> IntersectSortedArrays(const std::vector<int>& A,
                                       const std::vector<int>& B) {
  std::vector<int> result;
  for (size_t i = 0; i < A.size(); ++i) {
    if ((i == 0 || A[i] != A[i - 1]) &&
        std::binary_search(B.begin(), B.end(), A[i])) {
      result.push_back(A[i]);
    }
  }
  return result;
}
```

Time O(m log n) with m the outer array's length — so **iterate over the shorter array**: when n ≪ m, n log m is far smaller than m log n. This is the best choice when the sizes are wildly lopsided (one common query word, one rare one). It still ignores the sortedness of the outer array, though, so it is not optimal when the lengths are comparable.

**Approach 3 — advance two pointers in lockstep, O(m + n).** Keep an index into each array. If the current elements differ, the smaller one cannot appear in the intersection (everything remaining in the other array is at least as large), so advance past it. If they are equal, record the value — unless it equals its predecessor in A, which means it was already recorded — and advance both.

```cpp
std::vector<int> IntersectSortedArrays(const std::vector<int>& A,
                                       const std::vector<int>& B) {
  std::vector<int> result;
  size_t i = 0, j = 0;
  while (i < A.size() && j < B.size()) {
    if (A[i] == B[j]) {
      if (i == 0 || A[i] != A[i - 1]) {
        result.push_back(A[i]);
      }
      ++i, ++j;
    } else if (A[i] < B[j]) {
      ++i;
    } else {  // B[j] < A[i]
      ++j;
    }
  }
  return result;
}
```

**Worked example.** A = ⟨1, 2, 2, 4, 6⟩, B = ⟨2, 2, 4, 5⟩. Compare 1 vs 2: drop 1. Compare 2 vs 2: equal, first occurrence → emit 2, advance both. Compare A's second 2 vs B's second 2: equal, but A[i] equals its predecessor → skip, advance both. Compare 4 vs 4: emit 4, advance both. Compare 6 vs 5: drop 5; B is exhausted → return ⟨2, 4⟩.

**Complexity.** Each iteration advances at least one pointer and does O(1) work, so O(m + n) time and O(1) space beyond the output — optimal when the arrays have similar lengths.

## 10.2 Render a calendar

An online calendar renders each day's events as non-overlapping rectangles: the X-axis is time, an event spanning [b, e] must span exactly [b, e] horizontally, all rectangles get equal height, and everything must fit in a vertical band of height L. The maximum possible rectangle height is therefore L divided by the *maximum number of events that are ever simultaneously in progress*. So the computational core is: given a set of intervals (start, finish), find the maximum number that overlap at any single point in time.

*Hint: focus on endpoints.*

**Approach 1 — test every endpoint, O(n²).** The count of in-progress events only changes at some event's start or finish, so the maximum is achieved at one of the 2n endpoints. For each endpoint, count in O(n) how many intervals contain it: 2n endpoints × O(n) per check = O(n²). The waste: moving from one endpoint to the next recounts from scratch instead of updating incrementally.

**Approach 2 — sweep a counter over sorted endpoints, O(n log n).** Flatten every interval into two endpoint records and sort them by time, with one tie-break rule: **at equal times, a start endpoint precedes an end endpoint** (an event beginning at the instant another ends does overlap it, given inclusive endpoints; ties among two starts or two ends can be broken arbitrarily). Then sweep left to right keeping a running counter: +1 at each start, −1 at each end. The counter always equals the number of events in progress, so its running maximum is the answer.

```cpp
struct Event {
  int start, finish;
};

struct Endpoint {
  int time;
  bool is_start;

  bool operator<(const Endpoint& other) const {
    // Earlier time first; at the same time, starts come before ends.
    return time != other.time ? time < other.time
                              : (is_start && !other.is_start);
  }
};

int MaxSimultaneousEvents(const std::vector<Event>& events) {
  std::vector<Endpoint> endpoints;
  endpoints.reserve(2 * events.size());
  for (const Event& e : events) {
    endpoints.push_back({e.start, true});
    endpoints.push_back({e.finish, false});
  }
  std::sort(endpoints.begin(), endpoints.end());

  int in_progress = 0, best = 0;
  for (const Endpoint& p : endpoints) {
    if (p.is_start) {
      best = std::max(best, ++in_progress);
    } else {
      --in_progress;
    }
  }
  return best;
}
```

**Worked example.** Events (0, 3), (1, 5), (4, 6), (5, 8). Sorted endpoints: 0s, 1s, 3e, 4s, 5s, 5e, 6e, 8e — note the tie at time 5 puts the start of (5, 8) before the end of (1, 5). Counter trace: 1, 2, 1, 2, **3**, 2, 1, 0. The maximum is 3, witnessed at time 5, when (1, 5), (4, 6), and (5, 8) are all in progress.

**Complexity.** Sorting the 2n endpoints costs O(n log n); the sweep is O(n). Total O(n log n) time and O(n) space for the endpoint array.

**Variants:**
- n users share an internet connection; user i consumes bandwidth b_i from time s_i to f_i inclusive. Compute the peak total bandwidth. (Same sweep with the counter incremented by ±b_i instead of ±1.)
