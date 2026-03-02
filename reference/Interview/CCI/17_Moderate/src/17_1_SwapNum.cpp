// Q. Write a function to swap a number in place (that is, without temporary variables).

#include <iostream>

using namespace std;

// Version1: Using the difference
void swap(int &a, int &b) {
	// example for a=9, b=4;
	a = a-b;	// a = 5, diff
	b = a+b;	// b = 9, diff + b
	a = b-a;	// a = 4, b(a) - diff

	return;
}

// Version2: Using the bit difference
void swap_xor(int &a, int &b) {
	// example for a=101 (in binary) and b=110
	a = a^b;	// a = 011 (bit diff)
	b = a^b;	// b = 101 (diff ^ b) -> a
	a = a^b;	// a = 110 (diff ^ b(a)) -> b

	return ;
}


int main() {
	int a,b;
	int n=10;

	for(int i=-10; i<0; i++) {
		a=i; b=n-i;
		cout << "a, b = " << a << ", " << b << " ";
		swap(a, b);
		cout << "swap(a,b) = " << a << ", " << b << endl;;
	}
    cout<< endl;
	for(int i=-10; i<0; i++) {
		a=i; b=n-i;
		cout << "a, b = " << a << ", " << b << " ";
		swap_xor(a, b);
		cout << "swap(a,b) = " << a << ", " << b << endl;;
	}

	return 0;
}

		





