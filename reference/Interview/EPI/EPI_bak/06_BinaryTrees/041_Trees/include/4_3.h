// Q. Given a sorted (increasing order) array, write an alogirthm to create a BST with min height.

// Checks: root should be the middle of the array
#include "BST.h"

using namespace std;

void bstree::insertSorted(int arr[], int begin, int end) {
	if(end < begin) return;

	int mid = (begin + end) /2;
	this->insert(arr[mid]);
	insertSorted(arr, begin, mid-1);
	insertSorted(arr, mid+1, end);
}
