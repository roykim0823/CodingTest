// Q. Write a method which finds the maximum of two numbers without using any comparison operators
// Hint: Using a-b

/* 
Checks:
	A common way of implementing a max function is to look at the sign of a-b. In this case, we can't
	use a comparison operator on this sign, but we can use multiplication.
	Let k equal the sign of (a-b) such if a-b>=0, then k is 1. Eles k=0.
	Let q be the inverse of k.
*/

#include <iostream>
using namespace std;

int sign(int a) 
{
	// 1: Positive, 0: Negative
	return ((a>>31) & 0x1) ^ 1; // ^1 is for flipping the bit
}


// It doesn't work if a-b is an overflow.
int getMaxNaive(int a, int b) {
	int k=sign(a-b);
	int q=k^1;	
	return a*k+b*q;
}

// a-b is an overflow if the signs of a and b are differernt
// if a and b have different signs:
// 	1. a>0, b<0 then k=1
//  2. a<0, b>0 then k=0
//  -> Either way, k=sign(a)
// else
// 	k=sign(a-b)	// no overflow
int getMax(int a, int b)
{
	int c=a-b;

	int sa=sign(a);
	int sb=sign(b);
	int sc=sign(c);

	// If a and b have different signs, then k=sign(a)
	int use_sign_of_a = sa ^ sb;

	// If a and b have the same sign, then k=sign(a-b);
	int use_sign_of_c = sa^sb^1;

	int k=use_sign_of_a & sa + use_sign_of_c * sc;
	int q =k^1;

	return a*k + b*q;
}
int main(){
	cout << getMax(0x80000000,0x7FFFFFFF) << endl; 
	cout << getMaxNaive(0x80000000,0x7FFFFFFF) << endl;
	cout << sizeof(int) << endl;
    return 0;
}
