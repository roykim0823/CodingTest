// Q. An array A contains all the integer from 0 through n, except for one number which is missing.
//    But, we cannot access an entire integer in A with a single operation. Just 1 bit at a time.
//	  Can you find the missing number in O(n)?

#include <iostream>
#include <cstring>
using namespace std;

// use the fact sum(1..n) = n(n+1)/2
// sum(0..n-1) = (n-1)n/2
// if there is no missing one sum would be n(n+1)/2 since it starts at 0
// Naive: Ignore bit access, O(n) -> O(nlogn) since n -> log2(n) bits
int missingNum(int a[], int n) {
	int sum=0;
	for(int i=0; i<n; i++) 
		sum+=a[i];
	return (n*(n+1)/2) - sum;
}
	
// O(n) version, check even or odd at the LSB bit
int missingNum2(int a[], int n) {
	int missing_num=0;
	bool flag_even=true;
	for(int i=0; i<n; i++) 
	{
		if(flag_even && a[i]%2==1)	// Error
			break;
		if(flag_even==false && a[i]%2==0)	// Error
			break;
		if(flag_even && a[i]%2==0) {	// even number
			missing_num++;
			flag_even=false; 	// next is odd
		}
		if(flag_even==false && a[i]%2==1) {	// odd number
			missing_num++;
			flag_even=true;		// next is even 
		}
	}
	return missing_num;
}

int main(){
	int a[11][10];
	int n=10;
	for(int i=0; i<=n; i++) {
		int inx=0;
		for(int j=0; j<=n; j++) {
			if(i==j) continue;
			a[i][inx++]=j;
		}
		for(int k=0; k<n; k++) 
			cout << a[i][k] << " ";
		cout << endl;
		cout << "Missing Number: " << missingNum(a[i], 10) 
			 << " " << missingNum2(a[i], 10) << endl;
	}

    return 0;
}
