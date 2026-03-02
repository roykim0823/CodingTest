// Q. Given a sorted array of n integers that has been rotated an unknown 
//    number of times, write code to find an element in the arry.
//    You may assume that the array was originally sorted in increasing
//    order. 
//    EXAMPLE: Input: find 5 in array (15 16 19 20 25 1 3 4 5 7 10 14) 
// 			   Output: 8 (the index of 5 in the array)

#include <iostream>
using namespace std;

// Binary search
// O(logN) if all elements are unique.
// O(n) if there are many duplicates
int search(int a[], int left, int right, int x)
{
	int mid=(left+right)/2;
	if(x==a[mid]) return mid;	// found
	if(right < left) return -1;

	// Either left or right half must be ordered.
	// Find out which side is ordered, and then use the normally ordered half
	// to figure out which side to searh to find x
	if(a[left] < a[mid]) {   // left is normally ordered
		if(x>=a[left] && x<=a[mid])
			return search(a, left, mid-1, x);	// search left
		else
			return search(a, mid+1, right, x);	// search right
	} else if (a[mid] < a[left]) {// Right is noramlly ordered
		if(x>=a[mid] && x<=a[right])
			return search(a, mid+1, right, x);	// search right
		else 
			return search(a, left, mid-1, x);	// search left
	} else if(a[left]==a[mid]) { 	// left half is all repeats, here a[mid]!=x
		if(a[mid]!=a[right]) // if right is diff, search it
			return search(a, mid+1, right, x);
		else {	// else, we have to search both halves	(e.g., {2, 2, 2, 3, 4, 2} case)
			int result = search(a, left, mid-1, x); // search left
			if(result==-1)
				return search(a, mid+1, right, x); // search right
			return result;
		}
	}
	return -1;
}

int main(){
    int a[12] = {
        15, 16, 19, 20, 25, 1, 3, 4, 5, 7, 10, 14
    };
    int b[19] = {
        2,2,2,2,2,2,2,2,3,2,2,2,2,2,2,2,2,2,2
    };
	int c[10] = {1, 1, 1, 2, 3, 4, 1, 1, 1, 1};
    cout<<search(a, 0, 11, 3)<<endl;
    cout<<search(b, 0, 18, 2)<<endl;
    cout<<search(c, 0, 9, 3)<<endl;
    return 0;
}
