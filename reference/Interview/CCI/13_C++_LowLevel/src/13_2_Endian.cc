#include <stdio.h>

// empty

// funtition to show bytes in memory, from location start to start + n
void show_mem_rep(char *start, int n) {
	for (int i=0; i<n; i++) {
		printf(" %.2x", start[i]);
	}
	printf("\n");	
}
int main() {
	int i = 0x01234567;
	show_mem_rep((char*)&i, sizeof(i));
	short s = 0x0123;
	show_mem_rep((char*)&s, sizeof(s));
	s = 0x4567;
	show_mem_rep((char*)&s, sizeof(s));
	return 0;
}

