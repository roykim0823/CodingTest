# Searching

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 8. See epilight_cpp_new.pdf for the full text.*

## Overview

Before picking a search algorithm, classify the problem along a few axes: Is the collection **static or dynamic** (do inserts/deletes interleave with queries)? Is it worth **preprocessing** the data to accelerate later queries? Are there **statistical properties** of the data to exploit (e.g. near-uniform key distribution)? And should you search the data directly, or **transform** it first?

This chapter concentrates on the simplest and most common cell of that grid: static data held in a sorted array. Structures for dynamic data — heaps, hash tables, balanced BSTs — are treated in their own chapters. The material splits into binary-search problems (variations on halving a sorted candidate set) and general search problems (cleverness without necessarily sorting).

Binary search is interview gold for a reason: every candidate claims to know it, it fits in a dozen lines, and yet the edge cases (empty ranges, one-element ranges, duplicates, overflow, off-by-one boundary updates) trip up a remarkable share of implementations. There is well-known folklore evidence: a survey once found the algorithm implemented correctly in only a small fraction of textbooks, and Jon Bentley observed that roughly 90% of professional programmers he assigned it to got it wrong with unlimited time — his own published version harbored an overflow bug that went unnoticed for about twenty years.

### Binary search fundamentals

Against unsorted keys, the only sure way to find something is to look at everything: O(n). Sorting buys eliminability: compare the target with the middle element of the current range, and one entire half provably cannot contain the target, so discard it. Iterating halves the range each step.

A correct iterative implementation over a sorted `vector<int>`:

```cpp
int BinarySearch(int target, const vector<int>& A) {
    int lo = 0, hi = (int)A.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;      // NOT (lo + hi) / 2 — see below
        if (A[mid] < target) {
            lo = mid + 1;
        } else if (A[mid] == target) {
            return mid;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}
```

The classic bug lives in the midpoint computation: `(lo + hi) / 2` can overflow `int` when both indices are large, producing a negative index. `lo + (hi - lo) / 2` is algebraically identical and overflow-safe. (This is exactly the defect that survived undetected for decades in a published version.)

**Cost:** the work satisfies T(n) = T(n/2) + O(1), which solves to O(log n). The precondition is a sorted array — an O(n log n) investment — so binary search pays off when the data is already sorted or when one sort is amortized over many queries. Many interview problems are small twists on this loop, and each twist reopens every boundary-condition trap, so re-derive the invariant ("the answer, if it exists, lies in [lo, hi]") every time.

## Boot camp

Library binary search "just works" on built-in types because the language knows how to compare them. For user-defined types in sorted collections you must supply the comparison yourself, and it must be a *strict weak ordering* (irreflexive, transitive, consistent) — a sloppy comparator makes lookups fail silently even for items genuinely present, because the search eliminates the half that actually contains them.

Example: an array of students sorted by descending GPA, ties broken alphabetically by name. To search it, hand the library the exact comparator that defines that order:

```cpp
struct Student {
    string name;
    double gpa;
};

bool OrderByGpaDesc(const Student& a, const Student& b) {
    if (a.gpa != b.gpa) return a.gpa > b.gpa;   // higher GPA first
    return a.name < b.name;                     // then A→Z by name
}

bool ContainsStudent(const vector<Student>& roster, const Student& who) {
    return binary_search(roster.begin(), roster.end(), who, OrderByGpaDesc);
}
```

**Complexity:** O(log n) comparisons, assuming O(1) random access into the sequence. The takeaway is a discipline, not a trick: the comparator passed to the search must be *the same ordering* the data was sorted with.

## Top tips

- Binary search is not just "find a key in a sorted array." Any monotonic predicate over an interval — of array indices, integers, or even real numbers — supports the same halving strategy ("binary search the answer").
- If your algorithm sorts first and then does cheaper-than-sort work (O(n) or below), the sort dominates the running time. That's a cue to look for an approach that avoids the full sort — partial sorting, heaps, or selection.
- Weigh time/space trade-offs explicitly: sometimes a second pass over the data eliminates the need for auxiliary memory, and vice versa.

## Know your searching libraries

All in `<algorithm>`, all requiring a sorted range (except `find`) and an optional custom comparator:

- `find(first, last, v)` — linear scan of any range, sorted or not; returns an iterator to the first match or `last`. O(n).
- `binary_search(first, last, v[, comp])` — returns a `bool`: is an equivalent element present? It does **not** report the position. O(log n).
- `lower_bound(first, last, v[, comp])` — iterator to the first element **not less than** `v` (i.e. `>= v`); this is where you learn positions. O(log n).
- `upper_bound(first, last, v[, comp])` — iterator to the first element **strictly greater than** `v`. O(log n). (`equal_range` returns both bounds at once; the two iterators bracket all occurrences of `v`.)

In an interview, unless asked to hand-roll binary search, prefer these functions — they are correct, and using them correctly is itself signal.

## 8.1 Search a sorted array for first occurrence of k

Standard binary search returns the index of *some* element equal to the key. Here duplicates matter: given a sorted array and a key `k`, return the index of the **first** (leftmost) occurrence of `k`, or −1 if `k` is absent. E.g., in `A = [2, 5, 5, 5, 9, 11]`, the answer for `k = 5` is 1 (not 2 or 3), and for `k = 9` it is 4.

*Hint: imagine the whole array equals k — finding one match must not end the search.*

**Approach 1 — find any match, then walk left.** Run ordinary binary search to land on some occurrence of `k`, then step backwards while the preceding element still equals `k`. Simple, and O(log n) when duplicates are rare — but the walk is linear in the number of duplicates, so an array made almost entirely of `k`s degrades it to O(n). The lesson: bolting a linear fix-up onto a logarithmic search forfeits the logarithm.

**Approach 2 — fold the requirement into the elimination rule.** Binary search's real invariant is a candidate interval that provably contains the answer if one exists. Reinterpret the three-way comparison for *this* problem:

- `A[mid] < k`: the first occurrence is right of `mid` → `lo = mid + 1`.
- `A[mid] > k`: it's left of `mid` → `hi = mid - 1`.
- `A[mid] == k`: `mid` is a *candidate answer*, but an earlier occurrence might exist — record `mid` and keep searching **left**: `hi = mid - 1`.

Every branch halves the interval, so the logarithmic bound is preserved; the recorded candidate is only ever improved (moved left), and it ends as the leftmost match.

```cpp
int FirstOccurrence(const vector<int>& A, int k) {
    int lo = 0, hi = (int)A.size() - 1;
    int result = -1;
    while (lo <= hi) {                    // invariant: answer (if any) in [lo, hi] or == result
        int mid = lo + (hi - lo) / 2;
        if (A[mid] < k) {
            lo = mid + 1;
        } else if (A[mid] > k) {
            hi = mid - 1;
        } else {                          // A[mid] == k
            result = mid;                 // best so far
            hi = mid - 1;                 // nothing right of mid can be first
        }
    }
    return result;
}
```

**Worked example.** `A = [2, 5, 5, 5, 9, 11]`, `k = 5`:

| lo | hi | mid | A[mid] | action | result |
|---|---|---|---|---|---|
| 0 | 5 | 2 | 5 | match → search left | 2 |
| 0 | 1 | 0 | 2 | too small → go right | 2 |
| 1 | 1 | 1 | 5 | match → search left | 1 |
| 1 | 0 | — | — | interval empty, stop | **1** |

**Complexity:** O(log n) time — each iteration halves the candidate interval regardless of which branch fires — and O(1) space. (Equivalently: `lower_bound` and then check that the found element equals `k`.)

**Variants:**
- Return the index of the first element **strictly greater** than the key (this is precisely `upper_bound`, and the same record-and-continue skeleton solves it).
- An unsorted array A satisfies A[0] ≥ A[1] and A[n−2] ≤ A[n−1]; an index is a *local minimum* if its value is ≤ both neighbors. Find one efficiently — binary search on the direction of descent: the boundary conditions guarantee a minimum exists in whichever half slopes downward.
- Return the pair [L, U] where L and U are the first and last occurrence of `k` (the interval enclosing `k`), or [−1, −1] if absent. E.g. for A = ⟨1, 2, 2, 4, 4, 4, 7, 11, 11, 13⟩ and k = 11, answer [7, 8]. Run the first-occurrence search and its mirrored last-occurrence twin.
- Given a sorted array of strings, test whether some string in it has `p` as a prefix.
