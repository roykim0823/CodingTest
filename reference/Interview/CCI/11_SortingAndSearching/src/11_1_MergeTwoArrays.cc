// Q. You are given two sorted arrays, A and B, and A has a large enough buffer
//    at the end to hold B. Write a method to merge B into A in sorted order

/*
Checks:
	1. A has enough buffer at the end, we won't need to allocate additional space.
	2. If we insert an element into the front of A, then we'll have to shift the existing elements 
	   backwards to make room for it.
	3. It's better to insert elements into he back of array, where there's empty space
*/

#include <iostream>
using namespace std;

// B is much smaller than A
// start at the end of A
void merge(int a[], int b[], int a_size, int b_size){
    int i = a_size - 1;
	int j = b_size - 1;
    int k = a_size + b_size - 1;

	// Merge a and b, starting from the last element in each
    while(i>=0 && j>=0){
        if(a[i] > b[j]) a[k--] = a[i--];
        else a[k--] = b[j--];
    }

	// Copy remaining elements from b into a
    while(j>=0) a[k--] = b[j--];

	// Don't need to copy the contents of A after running of of elements B.
	// -> They are already in place.
}

int main(){
    int a[15] = {
        1, 3, 7, 8, 9
    };
    int b[] = {
        2, 4, 5, 6, 10
    };
    int n = 5, m = 5;

    merge(a, b, 5, 5);

    for(int i=0; i<m+n; ++i)
        cout<<a[i]<<" ";
    return 0;
}
