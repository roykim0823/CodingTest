/* Majority Element

Write a function which takes an array and prints the majority element (if it exists), otherwise prints “No Majority Element”. A majority element in an array A[] of size n is an element that appears more than n/2 times (and hence there is at most one such element).
Examples :

```
Input : {3, 3, 4, 2, 4, 4, 2, 4, 4}
Output : 4 

Input : {3, 3, 4, 2, 4, 4, 2, 4}
Output : No Majority Element
```
*/

/* METHOD 1 (Basic)
The basic solution is to have two loops and keep track of maximum count for all different elements. 
If maximum count becomes greater than n/2 then break the loops and return the element having maximum count. 
If maximum count doesn’t become more than n/2 then majority element doesn’t exist.
- Complexity: T=O(n^2), S=O(1)
*/

// C++ program to find Majority 
// element in an array 
#include <bits/stdc++.h> 
using namespace std; 

// Function to find Majority element 
// in an array 
void findMajority(int arr[], int n) 
{ 
	int maxCount = 0; 
	int index = -1; // sentinels 
	for(int i = 0; i < n; i++) 
	{ 
		int count = 0; 
		for(int j = 0; j < n; j++) 
		{ 
			if(arr[i] == arr[j]) 
			count++; 
		} 
		
		// update maxCount if count of 
		// current element is greater 
		if(count > maxCount) 
		{ 
			maxCount = count; 
			index = i; 
		} 
	} 
	
	// if maxCount is greater than n/2 
	// return the corresponding element 
	if (maxCount > n/2) 
	cout << arr[index] << endl; 
	
	else
		cout << "No Majority Element" << endl; 
} 

/* METHOD 2 (Using Binary Search Tree) -> C++ STL Map
Insert elements in BST one by one and if an element is already present then increment the count of the node. At any stage, if count of a node becomes more than n/2 then return.
The method works well for the cases where n/2+1 occurrences of the majority element is present in the starting of the array, for example {1, 1, 1, 1, 1, 2, 3, 4}.
- Complexity: T=O(nlogn), S=O(n)
*/

/* METHOD 3 (Using Moore’s Voting Algorithm)
This is a two step process.

NOTE : This Method only works when we are given that majority element do exist in the array , otherwise this method won’t work , as in the problem definition we said that majority element may or may not exist but for applying this approach you can assume that majority element do exist in the given input array

    1. The first step gives the element that may be majority element in the array. If there is a majority element in an array, then this step will definitely return majority element, otherwise it will return candidate for majority element.
    2. Check if the element obtained from above step is majority element.This step is necessary as we are not always sure that element return by first step is majority element.

1. Finding a Candidate :
The algorithm for first phase that works in O(n) is known as Moore’s Voting Algorithm. Basic idea of the algorithm is that if we cancel out each occurrence of an element e with all the other elements that are different from e then e will exist till end if it is a majority element.
```
findCandidate(a[], size)
1.  Initialize index and count of majority element
     maj_index = 0, count = 1
2.  Loop for i = 1 to size – 1
    (a) If a[maj_index] == a[i]
          count++
    (b) Else
        count--;
    (c) If count == 0
          maj_index = i;
          count = 1
3.  Return a[maj_index]
```

2. Check if the element obtained in step 1 is majority element or not :
```
printMajority (a[], size)
1.  Find the candidate for majority
2.  If candidate is majority. i.e., appears more than n/2 times.
       Print the candidate
3.  Else
       Print "No Majority Element"
```
- Complexity: T=O(n), S=O(1)
*/

/* Function to find the candidate for Majority */
int findCandidate(int a[], int size) 
{ 
	int maj_index = 0, count = 1; 
	for (int i = 1; i < size; i++) 
	{ 
		if (a[maj_index] == a[i]) 
			count++; 
		else
			count--; 
		if (count == 0) 
		{ 
			maj_index = i; 
			count = 1; 
		} 
	} 
	return a[maj_index]; 
} 

/* Function to check if the candidate 
occurs more than n/2 times */
bool isMajority(int a[], int size, int cand) 
{ 
	int count = 0; 
	for (int i = 0; i < size; i++) 
	
	if (a[i] == cand) 
	count++; 
		
	if (count > size/2) 
	return 1; 
	
	else
	return 0; 
} 

/* Function to print Majority Element */
void printMajority(int a[], int size) 
{ 
/* Find the candidate for Majority*/
int cand = findCandidate(a, size); 

/* Print the candidate if it is Majority*/
if (isMajority(a, size, cand)) 
cout << " " << cand << " " << endl; 
	
else
cout << "No Majority Element" << endl; 
} 

/* Driver function to test above functions */
int main() 
{ 
    int a[] = {1, 3, 3, 1, 2}; 
    int size = (sizeof(a))/sizeof(a[0]); 
      
    findMajority(a, size);  	// METHOD 1
    printMajority(a, size);     // METHOD 3
    
	int b[] = {3, 3, 4, 2, 4, 4, 2, 4, 4}; 
    size = (sizeof(b))/sizeof(b[0]); 
      
    findMajority(b, size);  	// METHOD 1
    printMajority(b, size);     // METHOD 3
      
    return 0; 
} 




