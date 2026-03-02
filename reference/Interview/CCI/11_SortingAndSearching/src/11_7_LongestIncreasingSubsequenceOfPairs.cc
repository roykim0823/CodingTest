// Q. Find longest increasing subsequence of pairs

#include <iostream>
using namespace std;

// Naive: O(n^2)
int LISP(int a[], int size) {
	int start, end, length;
	int value=a[0];
	int max=0;
	for(int i=0; i<size; i++) {
		length=1;
		for(int j=i+1; j<size; j++) {
			if(a[j-1]>a[j])
			{
				break;
			}
			length++;
			if(length>max) {
				start=i;
				end=j;
				max=length;
			}
		}
	}

	cout << start << " " << end << endl;
	return max;
}
			  
// Revised: O(n)
int LISP2(int a[], int size) {
	int start, end, length;
	int value=a[0];
	int max=0;
	for(int i=0; i<size; i++) {
		length=1;
		for(int j=i+1; j<size; j++) {
			if(a[j-1]>a[j])
			{
				i=j-1;
				break;
			}
			length++;
			if(length>max) {
				start=i;
				end=j;
				max=length;
			}
		}
	}

	cout << start << " " << end << endl;
	return max;
}
			  
int main(){
	int a[] = {13, 14, 10, 11, 12, 9, 8, 1, 2, 3, 4, 5, 6};

	cout << LISP(a, 5) << endl;
	cout << LISP(a, 13) << endl;
	cout << LISP2(a, 5) << endl;
	cout << LISP2(a, 13) << endl;

}
