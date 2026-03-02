// n & (n-1) == 0 -> n is a power of 2
// 1. n & (n-1) == 0 means n and n-1 do not share any 1s.
// 2. Thus, n is single 1 and trailing 0s. -> power of 2

/* e.g.,
	if		n = abcde1000
	then  n-1 = abcde0111

	abcde must be all 0s, if n & (n-1) == 0 -> n is single 1 and trailing 0s. -> Power of 2
*/

#include <iostream>

bool powerOf2(int n) {
	return (n & (n-1)) == 0;
}

int main() {
	int x = 0xabcd1000;
	std::cout << x << " is power of 2? " << powerOf2(x) << std::endl;

	for(int i=0; i<1000000; i++) {
		if(powerOf2(i))
			std::cout << i << std::endl;
	}
    return 0;
}
