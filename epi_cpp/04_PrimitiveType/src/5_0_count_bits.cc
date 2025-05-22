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

// 1. Naive version: 32 iterations
int cntBit1(unsigned i) {
	int count=0;
	while(i) {
		if( i & 1 )
			count++;
		i >>= 1;	// right shift by 1 bit
	}
	return count;
}

// 2. Naive version: n (number of 1 bits) iterations
int CountBits(unsigned i) {
	int count=0;
	while(i) {
		count++;
		i = i & (i-1);	// clear the right most 1
	}
	return count;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{argv + 1, argv + argc};
	std::vector<std::string> param_names{"x"};
	return GenericTestMain(args, "count_bits.cc", "count_bits.tsv", &CountBits,
						   DefaultComparator{}, param_names);
  }
