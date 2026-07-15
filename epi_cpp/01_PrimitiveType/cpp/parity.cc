#include "test_framework/generic_test.h"
#include <vector>

// Approach 1: bit-by-bit brute force.
// O(n) time, O(1) space, n = word size
short Parity_brute_force(unsigned long long x) {
  short result = 0;
  while (x) {
    result ^= x & 1;
    x >>= 1;
  }
  return result;
}

// Approach 2: iterate only over the set bits.
// O(k) time, O(1) space, k = set bits
short Parity_iter(unsigned long long x) {
  short result = 0;
  while (x) {
    result ^= 1;
    x &= x - 1;  // erase the lowest set bit
  }
  return result;
}

// Approach 3: lookup table over fixed-width chunks.
// O(n/L) time, O(2^L) space, L = 16 (chunk width)
std::vector<short> kParityTable;

void BuildParityTable() {
  kParityTable.assign(1 << 16, 0);
  for (int i = 1; i < (1 << 16); ++i) {
    kParityTable[i] = kParityTable[i >> 1] ^ (i & 1);
  }
}

short Parity_lookup(unsigned long long x) {
  constexpr int kChunkBits = 16;
  constexpr unsigned kChunkMask = 0xFFFF;
  return kParityTable[(x >> (3 * kChunkBits)) & kChunkMask] ^
         kParityTable[(x >> (2 * kChunkBits)) & kChunkMask] ^
         kParityTable[(x >> kChunkBits) & kChunkMask] ^
         kParityTable[x & kChunkMask];
}

// Approach 4: XOR folding.
// O(log n) time, O(1) space
short Parity_xor(unsigned long long x) {
  x ^= x >> 32;
  x ^= x >> 16;
  x ^= x >> 8;
  x ^= x >> 4;
  x ^= x >> 2;
  x ^= x >> 1;
  return x & 0x1;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  GenericTestMain(args, "parity.cc", "parity.tsv", &Parity_brute_force,
                  DefaultComparator{}, param_names);
  GenericTestMain(args, "parity.cc", "parity.tsv", &Parity_iter,
                  DefaultComparator{}, param_names);
  BuildParityTable();
  GenericTestMain(args, "parity.cc", "parity.tsv", &Parity_lookup,
                  DefaultComparator{}, param_names);
  GenericTestMain(args, "parity.cc", "parity.tsv", &Parity_xor,
                  DefaultComparator{}, param_names);
}
