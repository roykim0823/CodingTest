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
using std::swap;
using std::uniform_int_distribution;
using std::vector;

// @include
// One execution of qsort
void dutch_flag_partition(vector<int> &A, int pivot_index) {
  int pivot = A[pivot_index];
  /**
   * Keep the following invariants during partitioning:
   * bottom group: 			A[0 		: smaller-1].
   * middle group: 			A[smaller 	: equal-1].
   * unclassified group: 	A[equal 	: larger ].
   * top group:	 			A[larger+1 	: A->size()-1].
   */
  int smaller = 0, equal = 0, larger = A.size() - 1;
  // When there is any unclassified element.
  while (equal <= larger) {
    // A[equal] is the incoming unclassified element.
    if (A[equal] < pivot) {
      swap(A[smaller++], A[equal++]);
    } else if (A[equal] == pivot) {
      ++equal;
    } else {  // A[equal] > pivot.
      swap(A[equal], A[larger--]);
    }
  }
}
// @exclude

vector<int> rand_vector(int len) {
  default_random_engine gen((random_device())());
  vector<int> ret;
  while (len--) {
//    uniform_int_distribution<int> dis(0, 2);
    uniform_int_distribution<int> dis(0, 9);
    ret.emplace_back(dis(gen));
  }
  return ret;
}
void print(vector<int> &vec) {
	for(auto v : vec) {
		cout << v << " ";
	}
	cout << endl;
}

int main(int argc, char* argv[]) {
  default_random_engine gen((random_device())());
  //for (int times = 0; times < 1000; ++times) {
  for (int times = 0; times < 10; ++times) {
    int n;
    if (argc == 2) {
      n = atoi(argv[1]);
    } else {
      uniform_int_distribution<int> dis(1, 20);
      n = dis(gen);
    }
    vector<int> A(rand_vector(n));
    uniform_int_distribution<int> dis(0, A.size() - 1);
    int pivot_index = dis(gen);
	cout << "Pivot index= " << pivot_index << " , Pivot value= " << A[pivot_index] << endl;
    cout << "Before: "; print(A);
    dutch_flag_partition(A, pivot_index);
    cout << "After:  "; print(A);
  }
  return 0;
}
