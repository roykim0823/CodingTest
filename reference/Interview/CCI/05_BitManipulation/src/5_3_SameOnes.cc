// Q. next smaller or next bigger positive numbers having the same number of 1-bits

/* Solutions
	1. filp 0 to 1, then flip 1 to 0 to maintain the same number of 1-bit
	getNext:
	- Next Bigger: 
		a. the number will be bigger if the 0-to-1 bit was left of the 1-to-0 bit 
		b. next bigger if flip the rihtmost 0-to-1 which has ones on the right of it
			-> flip the rightmost non-trailing zero
		c. Clear all the one to the right of the flipped one
		d. Set 1s by cleared ones-1  on the right most
	
	- Nest Smaller:
		a. the number will be smaller if the 1-to-0 bit was left of the 0-to-1 bit 
		b. next smaller if flip the rihtmost 1-to-0 which has ones on the right of it
			-> flip the rightmost non-trailing one
		c. Clear all the one to the right of the flipped one
		d. Set 1s by cleared ones+1 immediately on the right  
*/

#include <iostream>
#include <bitset>

using namespace std;

unsigned getNext(int n) {
	int c = n;
	int c0=0;
	int c1=0;

	// Count tailing zero
	while((c & 1) == 0 && (c != 0)) {
		c0++;
		c = c>>1;
	}

	// Count consecutive ones after tailing zero
	while((c & 1)==1) {
		c1++;
		c = c>>1;
	}

	// Error if n==111100000 or 0, then there is no bigger number with the same number of 1s.
	if(c0 + c1 == 31 || c0 + c1 == 0)
		return 0;

	int p = c0 + c1;		// position of rightmost non-traling zero

	cout << "c0 = " << c0 << ", c1 = " << c1 << endl;
	n |= (1 << p);			// Flip rightmost non-trailing zero
	cout << bitset<32>(n) << " <- Flip rightmost non-tailing zero" << endl;
	n &= ~((1<<p) - 1);		// Clear all bits to the right of p
	cout << bitset<32>(n) << " <- Clear all bits to the right of p(c0+c1)" << endl;
	n |= (1 << (c1-1)) -1;	// Insert (c1-1) ones on the right
	cout << bitset<32>(n) << " <- Insert (c1-1) ones on the right" << endl;
	
	return n;
}

unsigned getPrev(int n) {
	int c = n;
	int c0=0;
	int c1=0;

	// Count tailing one
	while((c & 1)==1) {
		c1++;
		c = c>>1;
	}

	if (c == 0) return 0;

	// Count tailing zero after the tailing one
	while((c & 1) == 0 && (c != 0)) {
		c0++;
		c = c>>1;
	}


	int p = c0 + c1;		// position of rightmost non-traling ones

	cout << "c0 = " << c0 << ", c1 = " << c1 << endl;
	n &= ((~0) << (p+1));		// Clear from bit p onwards
	cout << bitset<32>(n) << " <- Clear from bit p onwards" << endl;
	int mask = (1<< (c1+1)) -1 ;// sequence of (c1+1)
	cout << bitset<32>(mask) << " <- Mask: sequence of (c1+1) ones" << endl;
    n |= mask << (c0-1);
	cout << bitset<32>(n) << " <- n | mask << (c0-1)" << endl;

	return n;
}

int main()
{
	unsigned num = 0xFFFFD9FC;

	unsigned next, prev;
	cout << bitset<32>(num) << " " << num << endl;
	next = getNext(num);
	cout << bitset<32>(next) << " " << next << endl;
	cout << endl;
	cout << bitset<32>(num) << " " << num << endl;
	prev = getPrev(num);
	cout << bitset<32>(prev) << " " << prev << endl;
	//cout << bitset<32>((1<<i)-1) << endl;
}	
