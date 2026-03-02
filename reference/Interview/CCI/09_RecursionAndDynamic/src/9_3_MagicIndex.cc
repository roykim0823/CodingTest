// Q. A magic index in an array[1...n-1] is defined to be an index such that
//    A[i]==i. Given a sorted array of distinct integers, write a method to find
//    a magic index, if one exists, in array A
// Q+. What if the values are not distinct?

#include <iostream>
#include <algorithm> // for min
#include <vector>
using namespace std;

// BruteFource
int magicIndex_v1(int array[], int length)
{
	for(int i=0; i<length; i++) {
		if(array[i]==i)
			return i;
	}
	return -1;
}


// Using binary search
// Search decision based on the value. e.g., if a[5]=3, then the magic index cannot be in the left side
int magicIndex_v2(int array[], int length, int begin, int end) {
	if(end<begin || begin<0 || end >=length) 
		return -1;
	
	int mid=(begin+end)/2;
	if(array[mid]==mid)
		return mid;
	else if (array[mid] > mid)
		return magicIndex_v2(array, length, begin, mid-1);	// Left Side
	else 
		return magicIndex_v2(array, length, mid+1, end);    // Right Side
}

// FOLLOW UP
//int magicIndex_v3(int array[], int length, int begin, int end) {
void magicIndex_v3(int array[], int length, int begin, int end, vector<int> &index) {
	if(end<begin || begin<0 || end >=length)
		return ;
	
	int midIndex=(begin+end)/2;
	int midValue = array[midIndex];
	if(midValue==midIndex) {
		index.push_back(midIndex);
	    //return midIndex;
	}
	//	cout << midIndex << " ";

	// Need to search both side
	// Search Left
	int leftIndex = min(midIndex-1, midValue);
	//int left = magicIndex_v3(array, length, begin, leftIndex, index);
	magicIndex_v3(array, length, begin, leftIndex, index);
	//if(left>=0)
	//	index.push_back(left);
	//	return left;
	//	cout << left << "  ";
	
	// Search Right
	int rightIndex = max(midIndex+1, midValue);
	//int right = magicIndex_v3(array, length, rightIndex, end, index);
	magicIndex_v3(array, length, rightIndex, end, index);
    //if(right>=0)
	//	index.push_back(right);
	//    return right;
	//	cout << right << " ";
}

int main() {
	int a[]={-40, -20, -1, 1, 2, 3, 5, 7, 9, 12, 13};
	int b[]={-10, -5, 2, 2, 2, 3, 4, 7, 9, 12, 13};

	cout << magicIndex_v1(a, 11) << endl;
	cout << magicIndex_v1(b, 11) << endl;

	cout << magicIndex_v2(a, 11, 0, 10) << endl;
	cout << magicIndex_v2(b, 11, 0, 10) << endl;

	vector<int> indexA, indexB;
	magicIndex_v3(a, 11, 0, 10, indexA);
	magicIndex_v3(b, 11, 0, 10, indexB);
    for(int i=0; i<indexA.size(); i++)
		cout << indexA[i] << " ";
	cout << endl;
    for(int i=0; i<indexB.size(); i++)
		cout << indexB[i] << " ";
	cout << endl;
	
	return 0;
}
