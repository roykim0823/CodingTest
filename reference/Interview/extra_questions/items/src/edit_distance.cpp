/* Given two strings str1 and str2 and below operations that  
   can performed on str1. Find minimum number of edits   
   (operations) required to convert ‘str1’ into ‘str2’.
	1. Insert
	2. Remove
	3. Replace
   All of the above operations are of equal cost.
*/

// A Naive recursive C++ program to find minimum number 
// operations to convert str1 to str2 
#include<bits/stdc++.h> 
using namespace std; 

// Utility function to find minimum of three numbers 
int min(int x, int y, int z) 
{ 
return min(min(x, y), z); 
} 

// Naive recursive, O(3^m)
int editDist1(string str1 , string str2 , int m ,int n) 
{ 
	// If first string is empty, the only option is to 
	// insert all characters of second string into first 
	if (m == 0) return n; 

	// If second string is empty, the only option is to 
	// remove all characters of first string 
	if (n == 0) return m; 

	// If last characters of two strings are same, nothing 
	// much to do. Ignore last characters and get count for 
	// remaining strings. 
	if (str1[m-1] == str2[n-1]) 
		return editDist(str1, str2, m-1, n-1); 

	// If last characters are not same, consider all three 
	// operations on last character of first string, recursively 
	// compute minimum cost for all three operations and take 
	// minimum of three values. 
	return 1 + min ( editDist(str1, str2, m, n-1), // Insert 
					editDist(str1, str2, m-1, n), // Remove 
					editDist(str1, str2, m-1, n-1) // Replace 
				); 
} 

// A Dynamic Programming based C++ program to find minimum 
// number operations to convert str1 to str2 
// Time Complexity: O(m x n)
// Auxiliary Space: O(m x n)
int editDistDP2(string str1, string str2, int m, int n) 
{ 
	// Create a table to store results of subproblems 
	int dp[m+1][n+1]; 

	// Fill d[][] in bottom up manner 
	for (int i=0; i<=m; i++) 
	{ 
		for (int j=0; j<=n; j++) 
		{ 
			// If first string is empty, only option is to 
			// insert all characters of second string 
			if (i==0) 
				dp[i][j] = j; // Min. operations = j 

			// If second string is empty, only option is to 
			// remove all characters of second string 
			else if (j==0) 
				dp[i][j] = i; // Min. operations = i 

			// If last characters are same, ignore last char 
			// and recur for remaining string 
			else if (str1[i-1] == str2[j-1]) 
				dp[i][j] = dp[i-1][j-1]; 

			// If the last character is different, consider all 
			// possibilities and find the minimum 
			else
				dp[i][j] = 1 + min(dp[i][j-1], // Insert 
								dp[i-1][j], // Remove 
								dp[i-1][j-1]); // Replace 
		} 
	} 

	return dp[m][n]; 
} 

// Driver program 
int main() 
{ 
	// your code goes here 
	string str1 = "sunday"; 
	string str2 = "saturday"; 

	cout << editDistDP1(str1, str2, str1.length(), str2.length()); 
	cout << editDistDP2str1, str2, str1.length(), str2.length()); 

	return 0; 
} 



