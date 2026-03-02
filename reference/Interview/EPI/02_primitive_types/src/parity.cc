#include "test_framework/generic_test.h"
#include <vector>

// O(n) Time, O(1) Space
short parity(unsigned long long x) {
  short result = 0;
  while (x) {
    result ^= (x & 1);
    x >>= 1;
  }
  return result;
}

// O(n*) Time, O(1) Space
// n* is the number of '1' bits
short parity2(unsigned long long x) {
  short result = 0;
  while (x) {
    result ^= 1;
    x &= (x - 1);  // drops the lowest '1' bit of x.
  }
  return result;
}

// O(1) Time, O(n/4) space
// Use precomputed hash table
std::vector<short> precomputed_parity;

void build_precomputed_parity() {
  precomputed_parity.reserve(1<<16);
  for (int i=0; i < (1 << 16); ++i) {
    precomputed_parity.push_back(parity2(i));
  }
}

short parity3(unsigned long long x) {
  const unsigned short mask = 0b1111111111111111;
  return precomputed_parity[x >> 48] ^  // x[63:48]
         precomputed_parity[(x >> 32) & mask] ^  // x[47:32]
         precomputed_parity[(x >> 16) & mask] ^  // x[31:16]
         precomputed_parity[x & mask];  // x[15:0]
}

// O(1) time, O(1) space
// LSB of kFourBitParityLookupTable is the parity of 0.
// next bit is parity of 1, followed by the parity of 2, etc.
short four_bit_parity_lookup(short x) {
  const unsigned int kFourBitParityLookupTable = 0x6996;  // 0b0110100110010110
  return kFourBitParityLookupTable >> x;
}
short parity4(unsigned long long x) {
  x ^= x >> 32;
  x ^= x >> 16;
  x ^= x >> 8;
  x ^= x >> 4;
  x &= 0xf;  // only want the last four bits of x.
  // return the LSB which is the parity
  return four_bit_parity_lookup(x) & 1;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  GenericTestMain(args, "parity.cc", "parity.tsv", &parity,
                         DefaultComparator{}, param_names);
  GenericTestMain(args, "parity.cc", "parity.tsv", &parity2,
                         DefaultComparator{}, param_names);
  build_precomputed_parity();
  GenericTestMain(args, "parity.cc", "parity.tsv", &parity3,
                         DefaultComparator{}, param_names);
  GenericTestMain(args, "parity.cc", "parity.tsv", &parity4,
                         DefaultComparator{}, param_names);
}
