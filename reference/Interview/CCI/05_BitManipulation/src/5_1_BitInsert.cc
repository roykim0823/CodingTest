// Q. insert m into n[j:i]
#include <iostream>
#include <bitset>

using namespace std;

int updateBits(int n, int m, int i, int j) {
	// 1. clear the bits j through i in N: using mask bits
	int allOnes = ~0;
	int left = allOnes << (j+1);
	int right = ((1<<i) - 1);  //0100 + 1111 = 0011
	int mask = left | right;

	// 2. clear bits j though i
	int n_cleared = n & mask; 
	int m_shifted = m << i;

	return n_cleared | m_shifted;
}

int main()
{
	int n=0b10000000000;
	int m=0b10011;
	int i=2, j=6;

	cout << bitset<32>(n) << endl;
	cout << bitset<32>(updateBits(n, m, i, j)) << endl;
	//cout << bitset<32>((1<<i)-1) << endl;
}	
