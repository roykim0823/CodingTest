// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#ifndef SOLUTIONS_PARITY_H_
#define SOLUTIONS_PARITY_H_

// O(n) time
short parity1(unsigned long x) {
  short result = 0;
  while (x) {
    result ^= (x & 1);
    x >>= 1;
  }
  return result;
}

// O(n) time, n is the number of 1 bits
short parity2(unsigned long x) {
  short result = 0;
  while (x) {
    result ^= 1;
    x &= (x - 1);  // drops the lowest set bit of x.
  }
  return result;
}

// Use precomputed hash table
static bool is_initialized = false;
short precomputed_parity[1 << 16];

void build_table() {
  if (!is_initialized) {
    for (int i = 0; i < (1 << 16); ++i) {
      precomputed_parity[i] = parity2(i);
    }
    is_initialized = true;
  }
}

short parity3(unsigned long x) {
  return precomputed_parity[x >> 48] ^
         precomputed_parity[(x >> 32) & 0b1111111111111111] ^
         precomputed_parity[(x >> 16) & 0b1111111111111111] ^
         precomputed_parity[x & 0b1111111111111111];
}

// Use 4 bit then compute the parity 
short four_bit_parity_lookup(int x);

// @include
short parity4(unsigned long x) {
  x ^= x >> 32;
  x ^= x >> 16;
  x ^= x >> 8;
  x ^= x >> 4;
  x &= 0xf; // only want the last 4 bits of x.
  // Return the LSB, which is the parity.
  return four_bit_parity_lookup(x) & 1;
}

// The LSB of kFourBitParityLookupTable is the parity of 0,
// next bit is parity of 1, followed by the parity of 2, etc.

const int kFourBitParityLookupTable = 0x6996; // = 0b0110100110010110.

short four_bit_parity_lookup(int x) {
  return kFourBitParityLookupTable >> x;
}
// @exclude
#endif  // SOLUTIONS_PARITY_H_
