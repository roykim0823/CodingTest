#include "test_framework/generic_test.h"
#include <iostream>
#include <vector>
using namespace std;
/*
// Bit facts and Tricks
x ^ 0s = x
x ^ 1s = ~x
x ^ x = 0
x ^ ~x = 1s
x & (x-1)		// equals x with the least significant bit cleared
x & !(x-1)   	// extracts the lowest set bit of x (all other bits are cleared)
x ^ (x >> 1) 	// standard (binary-refected) Grady code for x
x & (~0 << n) 	// clears the n rightmist bits of a
*/

/* Examples
0110 + 0110 = 0110 * 2 = 0110 << 1
0100 * 0011 = 4 * 0011 = 0011 << 2
1101 ^ (~1101) = 1111
1101 & ( ~0 << 2) = 1100
*/

// Common Bit tasks: get, set, clear, and update a bit
int getBit(int num, int i) 	{		// return ith bit
	int mask = 1 << i;
	if ( (num & mask) != 0)
		return 1;
	else
		return 0;
	//return ( (num & (1 << i) ) != 0 ) ;
}

int setBit(int num, int i)		{		// set ith bit
	return num | ( 1 << i);
}

int clearBit(int num, int i)	{   	// clear ith bit
	int mask = ~(1 << i);	// mask is the reverse of setBit
	return num & mask;
}

int clearBitMSBthroughI(int num, int i) {
	int mask = (1 << i)-1;
	return num & mask;
}

int clearBitIthrough0(int num, int i) {
	int mask = ~((1 << (i+1))-1);
	return num & mask;
}

// update num's i-th bit with v
int updateBit(int num, int i, int v) {
	int mask = ~(1 << i);
	return (num & mask) | (v << i);
}

// Given a 32 bit unsigned integer, write a function that returns a count of bits are "1."

// Boot camp version: bit-by-bit.
// O(n) time, O(1) space, n = word size
short CountSetBits(unsigned int x) {
  short count = 0;
  while (x) {
    count += x & 1;
    x >>= 1;
  }
  return count;
}

// Refinement: clear the lowest set bit each iteration.
// O(k) time, O(1) space, k = set bits
short CountSetBits_iter(unsigned int x) {
  short count = 0;
  while (x) {
    count++;
    x &= x - 1;  // clear the right most 1
  }
  return count;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  GenericTestMain(args, "count_bits.cc", "count_bits.tsv", &CountSetBits,
                  DefaultComparator{}, param_names);
  return GenericTestMain(args, "count_bits.cc", "count_bits.tsv",
                         &CountSetBits_iter, DefaultComparator{}, param_names);
}
