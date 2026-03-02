#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>
#include <set>

using std::cout;
using std::default_random_engine;
using std::endl;
using std::random_device;
using std::uniform_int_distribution;
using std::vector;

// O(n^2) check of inversions or naive version
template <typename T>
int check_inversions(const vector<T>& A) {
  int inv_count = 0;
  for (size_t i = 0; i < A.size(); ++i) 
    for (size_t j = i + 1; j < A.size(); ++j) 
      if (A[i] > A[j]) 
        ++inv_count;
  return inv_count;
}


int count_inversions_helper(vector<int>& A, int start, int end);
int merge(vector<int>& A, int start, int mid, int end);

// argument is copied, so the A is not modified.
int count_inversions(vector<int> A) {
  return count_inversions_helper(A, 0, A.size());
}

int count_inversions_helper(vector<int>& A, int start, int end) {
  if (end - start <= 1) {
    return 0;
  }

  // divide the vector into two parts 
  int mid = start + ((end - start) >> 1);

  // inversion count will be sum of inversions in left-part, right-part 
  // and number of inversions in merging
  return count_inversions_helper(A, start, mid) +
         count_inversions_helper(A, mid, end) + merge(A, start, mid, end);
}

// Merge two sorted arrays and returns inversion count in the arrays
int merge(vector<int>& A, int start, int mid, int end) {

  vector<int> sorted_A;
  int left_start = start, right_start = mid, inver_count = 0;

  while (left_start < mid && right_start < end) {
    if (A[left_start] <= A[right_start]) {
      sorted_A.emplace_back(A[left_start++]);
    } else {
      sorted_A.emplace_back(A[right_start++]);
      
	  // A[left_start:mid - 1] will be the inversions.
	  // A[left_start] > A[right_start] means A[right_start] is less than A[left_start]...A[mid-1]
	  // so, the inver_count is mid - left_start
      inver_count += mid - left_start;
    }
  }
  // copy the remaining element of left array and right array to sorted_A
  copy(A.begin() + left_start, A.begin() + mid, back_inserter(sorted_A));
  copy(A.begin() + right_start, A.begin() + end, back_inserter(sorted_A));

  // Update A with sorted_A -> copy back the merged elements to original vector A.
  copy(sorted_A.begin(), sorted_A.end(), A.begin() + start);
  return inver_count;
}

/*
// A STL Set based approach for inversion count 

1) Create an empty Set in C++ STL (Note that a Set in C++ STL is 
   implemented using Self-Balancing Binary Search Tree). And insert
   first element of array into the set.

2) Initialize inversion count as 0.

3) Iterate from 1 to n-1 and do following for every element in arr[i]
     a) Insert arr[i] into the set-> not working with repeated values -> multiset.
     b) Find the first element greater than arr[i] in set
        using upper_bound() defined Set STL.
     c) Find distance of above found element from last element in set
        and add this distance to inversion count.

4) Return inversion count.
*/

// Returns inversion count in arr[0..n-1] 
template <typename T>
int count_inversions_stl(std::vector<T> vec) 
{ 
	// Create an empty set and insert first element in it 
	std::multiset<T> multi_set1; 
	multi_set1.insert(vec[0]); 

	int invcount = 0; // Initialize result 

	//std::set<T>::iterator itset1; // Iterator for the set 

	// Traverse all elements starting from second 
	for (size_t i=1; i<vec.size(); i++) 
	{ 
		// Insert arr[i] in set (Note that set maintains 
		// sorted order) 
		multi_set1.insert(vec[i]); 

		// Set the iterator to first greater element than arr[i] 
		// in set (Note that set stores arr[0],.., arr[i-1] 
		auto itset1 = multi_set1.upper_bound(vec[i]); 

		// Get distance of first greater element from end 
		// and this distance is count of greater elements 
		// on left side of arr[i] 
		invcount += distance(itset1, multi_set1.end()); 
	} 

	return invcount; 
} 
