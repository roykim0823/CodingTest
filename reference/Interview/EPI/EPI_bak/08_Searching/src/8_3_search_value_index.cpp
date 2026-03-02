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
int search_value_index(const vector<T> &A) {
	int l=0, r=A.size()-1, res=-1; 	

	while(l <= r) {
		int m = (l + r) >> 1 ; 	// m(iddle) = (l+r)/2;
		int value = A[m]-m;
		if( value==0 )
			return m;
		else if(value > 0) {
			r=m-1;	
		} else 	// value > 0
			l=m+1;
	}
	return res;
}



int main(int argc, char* argv[]) {
	vector<int> a = {-1, 1, 2, 4, 5, 6, 7, 8, 9, 10};
	vector<int> b = {-2, -1, 0, 3, 4, 5, 6};

	cout << search_value_index(a) << endl; 	
	cout << search_value_index(a) << endl;  
	cout << search_value_index(a) << endl;  
	cout << search_value_index(a) << endl;  
	cout << search_value_index(b) << endl; 	
	cout << search_value_index(b) << endl;  
	cout << search_value_index(b) << endl;  
	cout << search_value_index(b) << endl;  
	cout << search_value_index(b) << endl;  
	cout << search_value_index(b) << endl;  
	cout << search_value_index(b) << endl;  
	cout << search_value_index(b) << endl;  

  return 0;
}
