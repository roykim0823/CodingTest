// Q. Implement rand7() with rand5()
#include <iostream>
#include <cmath>
using namespace std;

int rand5() {
	return rand()%5;
}
// Not equal probability
int rand7_naive() {
	int v = rand5() + rand5();
	return v % 7;
}

// Nondeterministic number of calls due to discarding 21~24
int rand7_nonDeter() {
	while(true) {
		int num= 5*rand5() + rand5();
		if(num<21) {
			return num%7;
		}
	}
}

int rand7() {
	while(true) {
		int r1= 2*rand5();	// evens between 0~9
		int r2= rand5();    // for 0~1
		if(r2 !=4) {	// r2 has extra even num-discard the extra
			int rand1=r2 %2;	// 0 or 1
			int num = r1 + rand1; 	// will be the range 0 to 9
			if(num<7)
				return num;
		}
	}
}


int main() {
}
