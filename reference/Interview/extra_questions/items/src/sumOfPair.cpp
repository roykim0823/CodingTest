//Write a program that, given an array A[] of n numbers and another number x, determines whether or not there exist two elements in S whose sum is exactly x. 

#include <iostream>
#include <algorithm>
#include <unordered_set>

using namespace std;

/* METHOD 1 (Use Sorting)
ALGORITHM:
hasArrayTwoCandidates (A[], ar_size, sum)
1) Sort the array in non-decreasing order.
2) Initialize two index variables to find the candidate
   elements in the sorted array.
       (a) Initialize first to the leftmost index: l = 0
       (b) Initialize second  the rightmost index:  r = ar_size-1
3) Loop while l < r.
       (a) If (A[l] + A[r] == sum)  then return 1
       (b) Else if( A[l] + A[r] <  sum )  then l++
       (c) Else r--
4) No candidates in whole array - return 0

Analysis:
Time Complexity: 
	1) With Merge or Heap Sort: (-)(nlogn)
	2) Quick Sort: O(n^2) worst case
Auxiliary Space: depends on sorting algorithm
	1) Merge Sort: O(n)
	2) Heap Sort: O(1)
*/

bool hasArrayTwoCandidate(int A[], int arr_size, int sum)
{
	int l, r;

	// Sort the elements
	sort(A, A+arr_size);

	// look for the two candidates in the sorted array
	l=0;
	r = arr_size-1;
	
	while(l<r) {
		int candidate = A[l]+A[r];
		if(candidate == sum) {
			cout << "left = " << l << ", right = " << r << endl;
			return true;
		} else if(candidate < sum) 
			l++;
		else	// candidate > sum
			r--;
	}

	return false;
}

/* METHOD 2 (Use Hashing)
ALGORITHM:
1) Initialize an empty has table s.
2) Look for the pair in s, and Insert A[i] into s.
	(a) look for the current pair
	(b) insert A[i] into s.
Analysis:
Time Complexity: O(n)
Auxiliary Space: O(n) in the hash table
*/

bool findPair(int A[], int arr_size, int sum) 
{
	unordered_set<int> s;
	for(int i=0; i<arr_size; i++) {
		int theOther = sum - A[i];
		if(theOther >= 0 && s.find(theOther) != s.end()) { 
		cout << "A[i] = " << A[i] << ", the Other = " << theOther << endl;
			return true;                                 
		}
		s.insert(A[i]);
	}
	return false;
}

int main() {
	int A[] = {1, 4, 45, 6, 10, -8};
	int n = 16;
	int arr_size = sizeof(A) / sizeof(A[0]);

	//if(hasArrayTwoCandidate(A, arr_size, n))
	if(findPair(A, arr_size, n))
		cout << "Array has two elements with given sum= " << n << endl;
	else
		cout << "Array does not have two elements with given sum= " << n << endl;
	
	return 0;
}

