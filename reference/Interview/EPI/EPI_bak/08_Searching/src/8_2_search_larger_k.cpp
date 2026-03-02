// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

using std::cout;
using std::default_random_engine;
using std::endl;
using std::random_device;
using std::uniform_int_distribution;
using std::vector;

template <typename T>
int search_larger_k(const vector<T> &A, const T &k) {
	int l=0, r=A.size()-1, res=-1; 	

	while(l <= r) {
		int m = (l + r) >> 1 ; 	// m(iddle) = (l+r)/2;
		if( A[m] > k) {
			// Record the solution and keep searching the left part
			res = m;
			r = m-1;
		} else 	// A[m] <= k
			l = m+1;
	}
	return res;
}



int main(int argc, char* argv[]) {
	vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	vector<int> b = {1, 1, 1, 2, 2, 2, 3, 4, 5, 6, 7, 7, 7, 8, 8, 8};

	cout << search_larger_k(a, 0) << endl; 		// 0
	cout << search_larger_k(a, 11) << endl;   	// -1
	cout << search_larger_k(a, 3) << endl;   	// 3
	cout << search_larger_k(a, 8) << endl;   	// 8
	cout << search_larger_k(b, 0) << endl; 		// 0
	cout << search_larger_k(b, 1) << endl;   	// 3
	cout << search_larger_k(b, 2) << endl;   	// 6
	cout << search_larger_k(b, 7) << endl;   	// 13
	cout << search_larger_k(b, 3) << endl;   	// 7

  return 0;
}
