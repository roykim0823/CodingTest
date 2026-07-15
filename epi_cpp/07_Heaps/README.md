# Heaps

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 7. See epilight_cpp_new.pdf for the full text.*

## Overview

A heap is a binary tree with two constraints. Structurally it is *complete* — every level full except possibly the last, which is packed to the left. Order-wise it satisfies the **heap property**: in a max-heap, each node's key is greater than or equal to both children's keys (a min-heap flips the inequality). Neither sibling order nor cross-subtree order is constrained; the only global guarantee is that the extremum sits at the root.

Completeness makes an array representation possible with no pointers at all: store the nodes level by level, and the children of the node at index `i` land at indices `2i + 1` and `2i + 2` (parent at `⌊(i − 1)/2⌋`). For example, the max-heap

```
          90
         /  \
       55    70
      /  \   /
    20   41 68
```

is just the array `[90, 55, 70, 20, 41, 68]`.

Operation costs for a max-heap of n elements:

- **Insert:** O(log n) — append at the next free slot, then "sift up," swapping with the parent while the heap property is violated.
- **Peek max:** O(1) — it's the root.
- **Extract max (delete-and-return the root):** O(log n) — overwrite the root with the last element, shrink the array, then "sift down," repeatedly swapping with the larger child until the property holds.
- **Search for an arbitrary key:** O(n). The heap property gives no guidance for general lookup — this is the deliberate trade-off, and the reason a heap is the wrong tool when you need `contains` or arbitrary deletion.

A heap is also called a **priority queue**: it behaves like a queue whose dequeue always removes the highest-priority element rather than the oldest. The min-heap is the mirror image, giving O(1) access to the *smallest* element.

## Boot camp

**Problem: the k longest strings in a stream.** Strings arrive one at a time and cannot be revisited; at the end, output the k longest seen (any order). Memory should not grow with the stream.

The working set is "the k best so far." When a new string arrives and the set is full, the element that should be discarded is the *shortest of the current winners* — so the structure must efficiently expose its minimum. That means a **min**-heap, even though the problem says "longest": the heap's top is the weakest member, i.e. the eviction candidate. This inversion (k largest → min-heap, k smallest → max-heap) is the single most useful heap idiom.

Order the heap by string length via a custom comparator:

```cpp
vector<string> KLongestStrings(int k, istringstream* stream) {
    auto shorter = [](const string& a, const string& b) {
        return a.size() > b.size();   // "greater" comparator => min-heap by length
    };
    priority_queue<string, vector<string>, decltype(shorter)> best(shorter);

    string s;
    while (*stream >> s) {
        best.push(move(s));
        if ((int)best.size() > k) {
            best.pop();               // drop the shortest of the k+1
        }
    }

    vector<string> result;
    while (!best.empty()) {
        result.push_back(best.top());
        best.pop();
    }
    return result;
}
```

**Complexity:** the heap never exceeds k + 1 entries, so each arriving string costs O(log k) for push/pop; n strings cost O(n log k) total, with O(k) space regardless of stream length. A best-case refinement: before inserting, peek at `top()` (O(1)) and skip the push entirely when the newcomer is no longer than the current shortest winner — with many short strings this avoids most heap operations.

## Top tips

- Choose a heap when the *only* queries are about the largest or smallest element (peek/extract/insert). If you also need fast membership tests, arbitrary deletes, or general search, a heap is the wrong structure — consider a BST or hash table.
- For "k largest out of many/streaming," maintain a size-k **min**-heap; for "k smallest," a size-k **max**-heap. The heap's root is always the next element to evict, not the answer itself.

## Know your heap library

C++ heaps live in `std::priority_queue` (header `<queue>`):

- By default it is a **max**-heap (`std::less` comparator, largest on top). For a min-heap, instantiate with `std::greater<>` or a custom comparator.
- `push(e)` / `emplace(args...)` — insert, O(log n).
- `top()` — read the extremum without removing it, O(1).
- `pop()` — remove the extremum; returns `void`, O(log n).
- `empty()`, `size()` — `top()` and `pop()` must never be called on an empty priority queue.
- A custom comparator is supplied as the third template parameter and passed to the constructor (see the boot camp code). Lambdas need `decltype`; a `std::function` or a functor struct also works.
- Related, lower-level tools: `make_heap` / `push_heap` / `pop_heap` from `<algorithm>` maintain the heap invariant directly over your own `vector`.

## 7.1 Merge sorted files

Motivation: ~500 files each hold one day of trade records, one per line, of the form `timestamp,symbol,shares,price`, already sorted by timestamp within each file. The files total several gigabytes; produce a single output file with *all* trades sorted by timestamp. Abstractly:

Given a collection of sorted sequences, compute their union as one sorted sequence. E.g., merging ⟨1, 8, 10⟩, ⟨2, 9⟩, and ⟨2, 4⟩ must yield ⟨1, 2, 2, 4, 8, 9, 10⟩.

*Hint: at any moment, how many elements actually matter?*

**Approach 1 — concatenate and sort.** Dump everything into one array and sort: O(n log n) time for n total elements, O(n) memory. Correct, but it throws away the given sortedness — and for the file scenario it requires holding all the data at once, which may not fit in RAM.

**Approach 2 — k-way merge with a min-heap.** Since each input is sorted, the globally smallest *remaining* element must be the current head of one of the k sequences. So only k elements matter at a time. Keep those k heads in a min-heap; loop: extract the minimum, append it to the output, and insert the successor from whichever sequence supplied it (if any remains). Heap entries must remember their origin — for in-memory vectors, a (current, end) iterator pair does it; for files, the file handle itself tracks the read position.

```cpp
struct Cursor {
    vector<int>::const_iterator pos, end;
    bool operator>(const Cursor& other) const { return *pos > *other.pos; }
};

vector<int> MergeSorted(const vector<vector<int>>& seqs) {
    priority_queue<Cursor, vector<Cursor>, greater<>> heads;
    for (const auto& s : seqs) {
        if (!s.empty()) heads.push({s.cbegin(), s.cend()});
    }

    vector<int> merged;
    while (!heads.empty()) {
        Cursor c = heads.top();
        heads.pop();
        merged.push_back(*c.pos);
        if (++c.pos != c.end) heads.push(c);
    }
    return merged;
}
```

**Worked example.** Merging ⟨1, 8, 10⟩, ⟨2, 9⟩, ⟨2, 4⟩:

| heap contents | extract | output so far | pushed next |
|---|---|---|---|
| {1, 2, 2} | 1 | 1 | 8 |
| {8, 2, 2} | 2 | 1 2 | 9 |
| {8, 9, 2} | 2 | 1 2 2 | 4 |
| {8, 9, 4} | 4 | 1 2 2 4 | — (seq 3 done) |
| {8, 9} | 8 | 1 2 2 4 8 | 10 |
| {10, 9} | 9 | 1 2 2 4 8 9 | — |
| {10} | 10 | 1 2 2 4 8 9 10 | — |

**Complexity:** with k input sequences the heap holds at most k entries, so each of the n output elements costs O(log k) — O(n log k) total. Auxiliary space is O(k) beyond the output itself; crucially, when inputs and output are files streamed line by line, O(k) is the *entire* memory footprint, which is what makes multi-gigabyte merges feasible.

**Approach 3 — repeated two-way merges.** Alternatively, merge the sequences pairwise (the merge step of merge sort): k sequences become k/2, then k/4, …, over log k rounds, each round touching all n elements — O(n log k) time, matching the heap. But any natural implementation materializes O(n) intermediate data per round, so its working memory is O(n): far worse than the heap's O(k) when k is small relative to n, which is precisely the file-merging regime.
