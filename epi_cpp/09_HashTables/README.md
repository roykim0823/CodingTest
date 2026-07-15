# Hash Tables

*Study notes for Elements of Programming Interviews in C++ (sampler), Chapter 9. See epilight_cpp_new.pdf for the full text.*

## Overview

The hash table is arguably the single most useful data structure in interview problems: it stores keys (optionally with associated values) and supports insert, delete, and lookup in O(1) time on average. A huge fraction of "optimize this brute-force solution" questions reduce to replacing a linear scan or a nested loop with a hash-table lookup. This chapter covers how hash tables work internally, what makes a good hash function, how to make your own classes hashable in C++, and the `unordered_set` / `unordered_map` APIs. The sampler includes one problem (9.1), which is the canonical "counting with a hash map" pattern.

### How a hash table works

- Keys live in an array of slots. A **hash function** maps each key to an integer (its hash code), and the code modulo the array length picks the slot. If the function spreads keys evenly, every operation touches O(1) expected slots.
- Two keys landing in the same slot is a **collision**. The standard remedy is chaining: each slot holds a linked list of the entries that hashed there. With n entries in m slots, the expected cost of an operation is O(1 + n/m).
- When the **load factor** n/m gets large, the table is **rehashed**: allocate a bigger array and re-insert everything. A rehash costs O(n + m), but if triggered geometrically (say, on every doubling of size), its amortized cost per insert is O(1). This is why a *single* insert can occasionally be O(n) even though inserts are O(1) on average — worth remembering for latency-sensitive (real-time) systems, where the practical fix is to run rehashing on a separate thread.
- Contrast with the alternatives: a sorted array keeps keys in order but makes updates O(n); a balanced BST gives O(log n) everything plus ordered iteration. A hash table beats both on raw insert/delete/lookup speed but gives up ordering entirely, and it depends on having a sensible hash function (rarely a problem in practice).

### Hash function requirements

- **Hard requirement:** equal keys must produce equal hash codes. Sounds trivial, but it breaks whenever a hash function reads something outside the key's logical value — an object's address, profiling/timing data, uninitialized padding, etc.
- **Soft requirements:** codes should be spread uniformly over the slots, and the function should be cheap to evaluate.
- **The mutable-key trap:** if you modify an object while it is inside a hash table, it now sits in the slot determined by its *old* hash code, so a subsequent lookup fails even though the object is physically in the table. The discipline is: erase, mutate, re-insert. Better still, only use immutable objects as keys.

### Hashing strings

A reasonable string hash should read every character, produce a wide range of values, and not let any single character dominate or zero out the result (a plain product of character codes fails: one zero character kills it). The workhorse is the polynomial hash — treat the string as digits of a number in some base and reduce modulo a modulus:

```cpp
int PolynomialStringHash(const std::string& s, int modulus) {
  const int kBase = 997;
  long long code = 0;
  for (char c : s) {
    code = (code * kBase + c) % modulus;
  }
  return static_cast<int>(code);
}
```

This function is also a **rolling hash**, which matters for sliding-window problems: when the window slides one position (drop the front character, append one at the back), the new code is computable in O(1) — subtract the front character's contribution (its code times `kBase^(len-1)`), multiply by the base, add the new character.

For dictionary-style workloads (store a large set of strings, query membership or prefixes), also know the **trie**: a tree in which nodes do not store keys; instead, a node's position — the path from the root — encodes the key. Tries can beat hash tables when prefix structure matters.

## Boot camp

### Application: grouping anagrams

Two words are anagrams when one is a letter-rearrangement of the other ("stop" / "pots"). Task: given a list of words, output every group of two or more words that are mutual anagrams. For input `{"pots", "cheap", "stop", "cat", "peach", "tops"}` the answer is `{pots, stop, tops}` and `{cheap, peach}`; `"cat"` is dropped since it has no partner.

The key insight: anagram-ness ignores character order, so sorting a word's characters produces a **canonical representative** shared by exactly the members of its anagram class — `sort("stop")` and `sort("pots")` are both `"opst"`. Comparing every pair of words costs O(n²·m log m) for n words of length at most m; instead, make one pass, using the sorted form as a hash-map key and accumulating the original words as the value. Any time a problem says "group equivalent items," think *canonical form as hash key*.

```cpp
std::vector<std::vector<std::string>> GroupAnagrams(
    const std::vector<std::string>& words) {
  std::unordered_map<std::string, std::vector<std::string>> class_of;
  for (const std::string& word : words) {
    std::string canon = word;
    std::sort(canon.begin(), canon.end());
    class_of[canon].push_back(word);
  }

  std::vector<std::vector<std::string>> groups;
  for (auto& [canon, members] : class_of) {
    if (members.size() >= 2) {
      groups.push_back(std::move(members));
    }
  }
  return groups;
}
```

Complexity: the n sorts of length-m strings dominate — O(n·m log m) time; the hash inserts contribute O(n·m). Space is O(n·m) for the map.

### Designing a hashable class

Suppose a contact list must be stored as a `vector<string>` that may contain duplicates, and two contact lists count as equal when they contain the same *set* of names — order and multiplicity are irrelevant (three copies of a name mean the same as one). To deduplicate a collection of such lists with an `unordered_set`, we must supply equality and a hash that agree with each other.

- Equality: build a set from each list and compare the sets.
- Hash: it must depend only on the distinct names, not on their order or repetition. De-duplicate first, then combine the individual `std::hash<string>` codes with XOR — XOR is commutative and associative, so processing order cannot affect the result.

```cpp
struct Contacts {
  std::vector<std::string> names;

  bool operator==(const Contacts& other) const {
    return std::unordered_set<std::string>(names.begin(), names.end()) ==
           std::unordered_set<std::string>(other.names.begin(),
                                           other.names.end());
  }
};

struct ContactsHasher {
  size_t operator()(const Contacts& c) const {
    std::unordered_set<std::string> distinct(c.names.begin(), c.names.end());
    size_t code = 0;
    for (const std::string& name : distinct) {
      code ^= std::hash<std::string>()(name);
    }
    return code;
  }
};

std::vector<Contacts> DeduplicateContacts(const std::vector<Contacts>& lists) {
  std::unordered_set<Contacts, ContactsHasher> unique(lists.begin(),
                                                      lists.end());
  return {unique.begin(), unique.end()};
}
```

Both the equality and the hash above rebuild a set on every call — O(n) per evaluation for n names — which is fine for an interview but wasteful in production. The practical fix is to cache the distinct-name set and the hash code inside the object, invalidating both whenever the list mutates. Caching hash codes is a common real-world optimization in general, always with that same caveat: the cache must be cleared when any field the hash reads is updated.

## Top tips

- Hash tables have the best theoretical *and* practical performance for lookup, insert, and delete — O(1) average for each. Keep the caveat in mind that one individual insert can cost O(n) when it triggers a resize.
- A hash code makes a cheap *signature*: compare hashes first to filter out non-matching candidates before doing an expensive full comparison.
- For fixed small mappings (character to value, character to character), a precomputed lookup table is cleaner and faster than a ladder of if/else code.
- When you define equality for a class that will live in a hash container, you must define a matching hash over exactly the fields equality inspects. If hash and equality disagree, logically equal objects land in different buckets and lookups silently return "not found" even when the item is present.
- Some problems want a multimap (several values per key) or a bidirectional map. If the library at hand lacks them, know the fallback: a map whose values are lists, or a third-party library.

## Know your hash table libraries

- The two workhorses are `unordered_set` (keys only) and `unordered_map` (key–value pairs). Neither admits duplicate keys — unlike, say, `list` or `priority_queue`.
- Essential `unordered_set` operations: `insert(v)` / `emplace(v)`, `erase(v)`, `find(v)`, `size()`.
  - `insert` returns a `pair<iterator, bool>`: the iterator points at the element (newly inserted or the pre-existing equivalent), and the bool tells you whether an insertion actually happened.
  - `find` returns an iterator to the element, or `end()` when absent — always compare against `end()`.
  - Iteration order from `begin()` is unspecified and can even change over time as the table rehashes.
- `unordered_map` mirrors the same API with `insert({k, v})` / `emplace`, `erase(k)`, `find(k)`, `size()`. Iterating yields `pair<key, value>` entries; iteration order is unspecified, but iterations over entries, keys, and values are mutually consistent.
- `std::hash` (header `<functional>`) provides ready-made hash functions for the built-in types — `int`, `bool`, `string`, `unique_ptr`, `shared_ptr`, and friends. For your own types, supply a hasher functor as the extra template argument (see the boot camp above).

## 9.1 Is an anonymous letter constructible?

You want to compose an anonymous letter by cutting characters out of a magazine. Given the letter's text and the magazine's text, determine whether the magazine contains enough of every character: each character must occur in the letter no more often than it occurs in the magazine.

*Hint: count the number of distinct characters appearing in the letter.*

**Approach 1 — count every character of the alphabet.** For each character in the character set, count its occurrences in the letter and in the magazine, and fail if the letter's count ever exceeds the magazine's. Correct, but slow in an avoidable way: it iterates over the whole alphabet including characters that appear in neither text, and it re-scans both strings once per alphabet character — as many passes as the character set has members.

**Approach 2 — one hash map, single pass over each text.** Scan the letter once, building a hash map from character to how many copies the letter needs. Then scan the magazine once: each time a magazine character appears in the map, decrement its count, erasing the entry when it reaches zero. If the map empties, every requirement is met and we can return true immediately — the rest of the magazine is irrelevant. If the magazine is exhausted while the map is nonempty, the leftover entries are exactly the characters the letter needs more copies of than the magazine supplies, so the answer is false.

```cpp
bool IsLetterConstructibleFromMagazine(const std::string& letter_text,
                                       const std::string& magazine_text) {
  std::unordered_map<char, int> deficit;  // char -> copies still needed
  for (char c : letter_text) {
    ++deficit[c];
  }

  for (char c : magazine_text) {
    if (deficit.empty()) {
      break;  // every letter character is already covered
    }
    auto it = deficit.find(c);
    if (it != deficit.end() && --it->second == 0) {
      deficit.erase(it);
    }
  }
  return deficit.empty();
}
```

**Worked example.** Letter `"atta"`, magazine `"a cat sat"`. After pass one, `deficit = {a:2, t:2}`. Scanning the magazine: `'a'` → `a:1`; `' '`, `'c'` → no-ops; `'a'` → `a` erased; `'t'` → `t:1`; `' '`, `'s'` → no-ops; `'a'` → not in map; `'t'` → `t` erased, map empty → **true**. Change the letter to `"attack"` and the entries `{c:1, k:1}` can never both drain (`k` never appears in the magazine), so the answer is **false**.

**Complexity.** O(m + n) time, where m and n are the letter and magazine lengths — the worst case (an unconstructible letter, or a magazine whose final character is the one that completes the letter) reads both strings fully. Space is O(L) for the L distinct characters of the letter. If the texts are ASCII, drop the hash map for a plain 256-entry `int` array indexed by character code — same algorithm, lower constant factor.
