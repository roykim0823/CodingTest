#include "swap_bits.h"
#include <iostream>
#include <vector>

using namespace std;

// O(n): n is bit width of x
unsigned long reverse_bits1(unsigned long x) {
	unsigned long reversed=0;
	int i=63;
	while(i--) {
		reversed |= (x & 1);
		x >>= 1; 
		reversed <<=1;
	}
	return reversed;
}

// O(n): 2/n, 32 iterations in all cases, by swapping
unsigned long reverse_bits2(unsigned long x) {
	int i=63, j=0;
	while(i>j) {
	 	if( ((x >>i) & 1) != ((x >> j) &1)) { 	// check if i and j bits are different
			x^= (1L << i) | (1L << j);        	// swap(i, j) bits
			//cout << x << endl;
		}
		i--;j++;
	}
	return x;
}

long reverse_x(long x, int n=63) {  	// same as reverse_bits2
  for (int i = 0, j = n; i < j; ++i, --j) {
    x = swap_bits(x, i, j);
  }
  return x;
}


// O(n): 2/n, best O(1)
unsigned long reverse_bits3(unsigned long x) {
	int i=63, j=0;
	// first check: 32 left bits = 32 right bits
	unsigned long left_bits = x >> 32;
	unsigned long  right_bits = x & 0x00000000FFFFFFFF;
	if(left_bits==right_bits)  	// Nothing to swap
		return x;	                // O(1)

	while(i>j) {
	 	if( ((x >>i) & 1) != ((x >> j) &1)) { 	// check if i and j bits are different
			x^= (1L << i) | (1L << j);        	// swap(i, j) bits
			//cout << x << endl;
		}
		i--;j++;
	}
	return x;
}

vector<long> precomputed_reverse;
void create_precomputed_table() {
  for (int i = 0; i < (1 << 16); ++i) {
    precomputed_reverse.emplace_back(reverse_x(i, 15));
  }
}

long reverse_bits(long x) {
  return precomputed_reverse[(x >> 48) & 0b1111111111111111] |
         precomputed_reverse[(x >> 32) & 0b1111111111111111] << 16 |
         precomputed_reverse[(x >> 16) & 0b1111111111111111] << 32 |
         precomputed_reverse[x & 0b1111111111111111] << 48;
}
