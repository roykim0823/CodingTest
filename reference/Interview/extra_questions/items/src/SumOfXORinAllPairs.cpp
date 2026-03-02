#include <bits/stdc++.h>

using namespace std;

// Naive: Brute Force
// Complexity: T=O(n^2), S=O(1)
int pairXORSum(vector<int> vec) {
	int sum=0;

	for(int i=0; i<vec.size(); i++) {
		for(int j=i+1; j<vec.size(); j++) {
			sum += vec[i] ^ vec[j];
		}
	}
	return sum;
}

// Improved by bit manipulation
// XOR Sum of all PAIR is equal to number of 1s X number of 0s
// So, calculate each bit's sum
// bit_sum = #1 * #0 * 2^(bit_position)
// Complexity: T=O(n), S=O(1)
int pairXORSum_bit(vector<int> &vec) {
	long sum=0;
	vector<int> temp(vec);
	for(int i=0; i<32; i++) {
		int zero_cnt=0;
		int one_cnt=0;

		for(int j=0; j<temp.size(); j++) {
			if(temp[j]%2 == 0)
				zero_cnt++;
			else
				one_cnt++;
			temp[j] = temp[j]>>1;	// temp[j]/=2;
		}
		sum += zero_cnt * one_cnt * (1 << i);
	}
	return sum;
}

int main() {
	long sum = 0;
	vector<int> vec = { 5, 9, 7, 6 };
	cout << pairXORSum(vec) << endl;
	cout << pairXORSum_bit(vec) << endl;
	return 0;
}
