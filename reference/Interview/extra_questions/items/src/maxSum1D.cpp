/*
Kadane’s Algorithm:

Initialize:
    max_so_far = 0
    max_ending_here = 0

Loop for each element of the array
  (a) max_ending_here = max_ending_here + a[i]
  (b) if(max_ending_here < 0)
            max_ending_here = 0
  (c) if(max_so_far < max_ending_here)
            max_so_far = max_ending_here
return max_so_far
Explanation:
Simple idea of the Kadane’s algorithm is to look for all positive contiguous segments of the array (max_ending_here is used for this). And keep track of maximum sum contiguous segment among all positive segments (max_so_far is used for this). Each time we get a positive sum compare it with max_so_far and update max_so_far if it is greater than max_so_far
*/

// C++ program to print largest contiguous array sum 
#include <iostream> 
#include <climits> 

using namespace std; 

int maxSubArraySum(int a[], int size) 
{ 
	int max_so_far = INT_MIN, max_ending_here = 0; 

	for (int i = 0; i < size; i++) 
	{ 
		max_ending_here = max_ending_here + a[i]; 
		if (max_so_far < max_ending_here) 
			max_so_far = max_ending_here; 

		if (max_ending_here < 0) 
			max_ending_here = 0; 
	} 
	return max_so_far; 
} 

// Improved by comparing max_so_far with max_ending_here only if max_ending_here > 0
int maxSubArraySum2(int a[], int size) 
{ 
	int max_so_far = 0, max_ending_here = 0; 
	for (int i = 0; i < size; i++) 
	{ 
		max_ending_here = max_ending_here + a[i]; 
		if (max_ending_here < 0) 
			max_ending_here = 0; 

		/* Do not compare for all elements. Compare only 
		when max_ending_here > 0 */
		else if (max_so_far < max_ending_here) 
			max_so_far = max_ending_here; 
	} 
	return max_so_far; 
} 

// this handles the case when all numbers in array are negative
int maxSubArraySum3(int a[], int size) 
{ 
	int max_so_far = a[0]; 
	int curr_max = a[0]; 

	for (int i = 1; i < size; i++) 
	{ 
		curr_max = max(a[i], curr_max+a[i]); 
		max_so_far = max(max_so_far, curr_max); 
	} 
	return max_so_far; 
} 
// C++ program to print largest contiguous array sum 
#include<iostream> 
#include<climits> 
using namespace std; 


int maxSubArraySumIdx(int a[], int size) 
{ 
	int max_so_far = INT_MIN, max_ending_here = 0, 
	start =0, end = 0, s=0; 

	for (int i=0; i< size; i++ ) 
	{ 
		max_ending_here += a[i]; 

		if (max_so_far < max_ending_here) 
		{ 
			max_so_far = max_ending_here; 
			start = s; 
			end = i; 
		} 

		if (max_ending_here < 0) 
		{ 
			max_ending_here = 0; 
			s = i + 1; 
		} 
	} 
	cout << "Maximum contiguous sum is "
		<< max_so_far << endl; 
	cout << "Starting index "<< start 
		<< endl << "Ending index "<< end << endl; 
} 


/* Driver program to test maxSubArraySum */
int main() 
{ 
	int a[] = {-2, -3, 4, -1, -2, 1, 5, -3}; 
	int n = sizeof(a)/sizeof(a[0]); 
	 
	cout << "Maximum contiguous sum is " << maxSubArraySum(a, n) << endl;
	cout << "Maximum contiguous sum is " << maxSubArraySum2(a, n) << endl;
	cout << "Maximum contiguous sum is " << maxSubArraySum3(a, n) << endl;
	maxSubArraySumIdx(a, n); 
	return 0; 
} 
