/*  Let n be the count, prev_avg be the previous average and x be the new number being added. 
    The average after including x number can be written as 
	(prev_avg*n + x)/(n+1).
*/

#include <stdio.h>

// Returns the new average after including x
float getAvg(float prev_avg, int x, int n)
{
	return (prev_avg * n + x) / (n + 1);
}

// Prints average of a stream of numbers
void streamAvg(float arr[], int n)
{
	float avg = 0;
	for (int i = 0; i < n; i++) {
		avg = getAvg(avg, arr[i], i);
		printf("Average of %d numbers is %f \n", i + 1, avg);
	}
	return;
}

/* The above function getAvg() can be optimized using following changes. 
   We can avoid the use of prev_avg and number of elements by using static variables 
   (Assuming that only this function is called for average of stream). 
   Following is the oprimnized version.
*/
// Returns the new average after including x
float getAvg2(int x)
{
	static int sum, n;

	sum += x;
	return (((float)sum) / ++n);
}


// Prints average of a stream of numbers
void streamAvg2(float arr[], int n)
{
	float avg = 0;
	for (int i = 0; i < n; i++) {
		avg = getAvg2(arr[i]);
		printf("Average of %d numbers is %f \n", i + 1, avg);
	}
	return;
}

// Driver program to test above functions
int main()
{
	float arr[] = { 10, 20, 30, 40, 50, 60 };
	int n = sizeof(arr) / sizeof(arr[0]);
	printf("version1:\n");
	streamAvg(arr, n);
	printf("version2:\n");
	streamAvg2(arr, n);

	return 0;
}

