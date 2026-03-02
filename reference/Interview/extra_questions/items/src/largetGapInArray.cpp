// A C++ program to find largest gap 
// between two elements in an array. 
#include<bits/stdc++.h> 
using namespace std; 

// O(N^2) 
int solve_naive(int a[], int n) 
{ 
	int max1 = INT_MIN; 
	for (int i = 0; i < n; i++) 
	{ 
		for (int j = 0; j < n; j++) 
		{ 
			if (abs(a[i] - a[j]) > max1) 
			{ 
				max1 = abs(a[i] - a[j]); 
			} 
		} 
	} 
	return max1; 
}

// O(N)
int solve(int a[], int n)
{
    int min1 = a[0];
    int max1 = a[0];

    // finding maximum and minimum of an array
    for (int i = 0; i < n; i++)
    {
        if (a[i] > max1)
            max1 = a[i];
        if (a[i] < min1)
            min1 = a[i];
    }
    return abs(min1 - max1);
}

// Driver Code 
int main() 
{ 
	int arr[] = { -1, 2, 3, -4, -10, 22 }; 
	int size = sizeof(arr) / sizeof(arr[0]); 
	cout << "Largest gap is : " << solve(arr, size) << endl;
	return 0; 
} 

// This code is contributed 
// by Akanksha Rai 

