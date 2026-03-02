#include <iostream>

int main() {
	int a[] = {1, 2, 3, 4, 5, 6};
	std::cout << (1+3)[a] - a[0] + (a+1)[2] << std::endl; 	// a[4] - a[0] + a[3] = 5 - 1 + 4 = 8

	return 0;
}
