#include <stdio.h>

int main()
{
	unsigned int i;
	
	//for(i=100; i>=0; --i)   	// when i=0 then --i is INT_MAX -> infinite loop
	for(i=100; i>0; --i)
	//	printf("%d\n", i);
		printf("%u\n", i);

	return 0;
}
