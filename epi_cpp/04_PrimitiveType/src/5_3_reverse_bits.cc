#include <array>
#include "test_framework/generic_test.h"

using std::array;

// O(n) Time, O(1) Space
// n is bit width of x
unsigned long long reverse_bits1(unsigned long long x) {
  unsigned long long reversed = 0;
  int i = 64;
  while (--i) {
    reversed |= (x & 1);
    x >>= 1;
    reversed <<= 1;
  }
  return reversed;
}

// O(n) Time, O(1) Space
// n/2 iterations
unsigned long reverse_bits2(unsigned long long x, int n = 63) {
  int i = n, j = 0;
  while (i > j) {
    if (((x >> i) & 1) != ((x >> j) & 1)) {  // check if i and j bits are different
      x ^= (1 << i) | (1 << j);  // swap(i, j) bits
    }
    --i; ++j;
  }
  return x;
}

// O(1) Time, O(n) Space
// Using Hashing, Initial Cost: O(n) Time, O(n) Space
static array<unsigned long long, 1 << 16> precomputed_reverse;

void create_precomputed_table() {
  for (int i=0; i < (1 << 16); ++i) {
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

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  GenericTestMain(args, "reverse_bits.cc", "reverse_bits.tsv",
                  &reverse_bits1, DefaultComparator{}, param_names);
  create_precomputed_table();
  GenericTestMain(args, "reverse_bits.cc", "reverse_bits.tsv",
                  &ReverseBits, DefaultComparator{}, param_names);
}
