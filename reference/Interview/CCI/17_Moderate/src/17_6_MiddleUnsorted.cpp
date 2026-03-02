// Q. Given an array of integers, write a method to find indices m and n such that if you sorted
//    elements m through n, the entire array would be sorted. Minimize n-m (that is, find the smallest
//    such sequence).

/* 
Check:
For example:
	1, 2, 4, 7, 10, 11, 7, 12, 6, 7, 16, 18, 19

Our first though might be to just find the longest increasing subsequence at the beginning and 
the longest increasing subsequence at the end.
left: 1, 2, 4, 7, 10, 11
middle: 7, 12
right: 6, 7, 16, 18, 19

To sort, left < middle < right order, we should "shrink" the left and the right subsequences until
the left.end <= middle.start and middle.end <= right.start since these condition never happens.
*/

#include <iostream>
#include <cstring>
using namespace std;

int findEndOfLeft(int array[], int size) {
	for(int i=i; i<size; i++) {
		if(array[i] < array[i-1]) return i-1;
	}
	return size-1;
}

int findStartOfRight(int array[], int size) {
	for(int i=size-2; i>=0; i--) {
		if(array[i] > array[i+1]) return i+1;
	}
	return 0;
}

int shrinkLeft(int array[], int size, int min_index, int start) {
	int comp=array[min_index];
	for(int i=start-1; i>=0; i--) {
		if(array[i]<=comp) return i+1;
	}
	return 0;
}


int shrinkRight(int array[], int size, int max_index, int start) {
	int comp=array[max_index];
	for(int i=start; i<size; i++) {
		if(array[i]>=comp) return i-1;
	}
	return size-1;
}

void findUnsortedSequence(int array[], int size) {
	// find left subsequence
	int end_left = findEndOfLeft(array, size);

	// find right subsequence
	int start_right = findStartOfRight(array, size);

    // find min and max element of middle
	int min_index = end_left+1;
	if(min_index>=size) return;		// aready sorted;

	int max_index = start_right-1;
	for(int i=end_left; i<=start_right; i++) {
		if(array[i] < array[min_index]) min_index=i;
		if(array[i] > array[max_index]) max_index=i;
	}

    // Slide left until less than array[min_index]
	int left_index = shrinkLeft(array, size, min_index, end_left);

    // Slide right until greater than array[max_index]
	int right_index = shrinkRight(array, size, max_index, start_right);

	cout << left_index << " " << right_index << endl;
}

void printArray(int a[], int size) {
	for(int i=0; i<size; i++)
		cout << a[i] << " ";
	cout << endl;
}

int main() {
	int a[]={1, 2, 4, 7, 10, 11, 7, 12, 6, 7, 16, 18, 19};

//	printArray(a, 13);
	findUnsortedSequence(a, 13);
	printArray(a, 13);
	
}
