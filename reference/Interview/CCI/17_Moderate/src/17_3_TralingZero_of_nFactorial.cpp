// Q. Write an algorithm which computes the number of trailing zeros in n factorial.

/* Checks:
	1. Naive Approach: 
		Compute the factorial, and then count the number of trailing zeros by continuously diviing by ten.
		The problem with this though is that the bounds of an int would be exceeded very quickly.
	2. More efficient one:
		Consider a factorial like 13:
			13! = 1*2*3*4*5*6*7*8*9*10*11*12*13
		A trailing zero is created with multiples of 10, and multiples of 10 are created with pairs of
		5-multiples and 2-multiples
			Trainling zero(13!) = 2*...*5*...*10*12*..
		To count the number of zeros, therefore, we only need to count the pairs of multiples of
		5 and 2. There will always be more multiples of 2 than 5 though, so simply counting the
		number of multiples of 5 is sufficient.
		Be carefull here.
			15 -> one trailing zero
			25 -> two trailing zero (5*5)
*/


#include <iostream>
using namespace std;

// Version1 
int factorOf5(int i) {
	int cnt = 0;
	while( i%5 == 0) {
		cnt++;
		i/=5;
	}
	return cnt;
}

int cntFactZeros(int num) {
	int cnt=0;
	for(int i=2; i<=num; i++) {
		cnt+= factorOf5(i);
	}
	return cnt;
}

// more efficient version
// - Directly counting the factors of 5. (n/5, n/25, ...)
int cntFactorZeros2(int num) {
    int cnt=0;
	if(num<0) return -1;

	for(int i=5; num/i>0; i*=5) {
		cnt+=num/i;
	}
	return cnt;
}

int main(){
    for(int i=1; i<100; ++i){
		cout << i << ": " <<  cntFactZeros(i) << ", " << cntFactorZeros2(i) << endl;
    }
    return 0;
}
