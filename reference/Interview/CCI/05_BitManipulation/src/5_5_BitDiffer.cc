// Q. Write a function to determine the number of bits required to convert integer A to B

/*
Checks: the number of different bits in two numbers -> XOR

*/
#include <iostream>
#include <bitset>
#include <string>

using namespace std;

// Naiver version: O(n)
int bitSwapRequired(unsigned a, unsigned b) {
	int count=0;
	for(unsigned c= a^b; c !=0; c=c>>1)
		count += c & 1;
	return count;
}
	
// Better: O(n), but n is the number of 1s
int bitSwapRequired2(unsigned a, unsigned b) {
	int count=0;
	for(unsigned c= a^b; c !=0; c=c&(c-1))  	// c & (c-1) remove the right most 1
		count++;
	return count;
}

int main()
{
	unsigned a, b;
	a=0xFFFF0000;
	b=0x0000FFFF;
	cout << bitSwapRequired(a, b) << ", " << bitSwapRequired2(a, b) << endl;
	a=0xFFFF0000;
	b=0x00FFFFFF;
	cout << bitSwapRequired(a, b) << ", " << bitSwapRequired2(a, b) << endl;
}	
