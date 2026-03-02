/* Find Next Sparse Number:
   A number is Sparse if there are no two adjacent 1s in its binary representation. 
   For example 5 (binary representation: 101) is sparse, but 6 (binary representation: 110) 
   is not sparse.
   Given a number x, find the smallest Sparse number which greater than or equal to x.
*/

#include<bits/stdc++.h>
using namespace std;

// O(logn) 
bool isSparse(int n) {
	// n is not sparse if there is set in AND of n and n/2 (right shift)
	if(n & (n>>1))
		return false;
	return true;
}

// Naive Solution
// O(nlogn), the next sparse number can be at most O(n) distance away
int nextSparse1(int x) {
	while(1) {
		if(isSparse(x))	// O(logn)
			return x;
		else
			x++;
	}
}

/* Efficient Solution
1) Find binary of the given number and store it in a
   boolean array.
2) Initialize last_finalized bit position as 0.
2) Start traversing the binary from least significant bit.
    a) If we get two adjacent 1's such that next (or third)
       bit is not 1, then
          (i)  Make all bits after this 1 to last finalized
               bit (including last finalized) as 0.
          (ii) Update last finalized bit as next bit.
*/
int nextSparse2(int x)
{
	// Find binary representation of x and store it in bin[].
	// bin[0] contains least significant bit (LSB), next bit is in bin[1], and so on.
	vector<bool> bin;
	while (x != 0) {
		bin.push_back(x&1);
		x >>= 1;
	}

	// There my be extra bit in result, so add one extra bit
	bin.push_back(0);
	int n = bin.size(); // Size of binary representation

	// The position till which all bits are finalized
	int last_final = 0;

	// Start from second bit (next to LSB)
	for (int i=1; i<n-1; i++)
	{
	// If current bit and its previous bit are 1, but next
	// bit is not 1.
	if (bin[i] == 1 && bin[i-1] == 1 && bin[i+1] != 1)
	{
			// Make the next bit 1
			bin[i+1] = 1;

			// Make all bits before current bit as 0 to make
			// sure that we get the smallest next number
			for (int j=i; j>=last_final; j--)
				bin[j] = 0;

			// Store position of the bit set so that this bit
			// and bits before it are not changed next time.
			last_final = i+1;
		}
	}

	// Find decimal equivalent of modified bin[]
	int ans = 0;
	for (int i =0; i<n; i++)
		ans += bin[i]*(1<<i);
	return ans;
}

// Driver program
int main()
{
	vector<int> vec = {6, 4, 38, 44};
	for(auto x : vec) {
		cout << x << " ->  Next Sparse Number is " << nextSparse1(x) 
	     	 << " " << nextSparse2(x) << endl;
	}
	return 0;
}

