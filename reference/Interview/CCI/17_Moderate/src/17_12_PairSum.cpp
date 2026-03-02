// Q. find all pairs of integers within an array which sum to a specified value

/* Solutions:
1. Simpel Solutions:
	a hash map from integers to integer. Iterate through the array. On each element x, 
	look up sum -x in the has table and, if it exists, print (x, sum-x). Add x to the has table, 
	and	go to the next element
2. Alternative
	1. Sort the array
	2. Check left and right element
*/

#include <iostream>
#include <algorithm>
using namespace std;

// Alternative version 
void PrintPairSum(int a[], int n, int sum){
    if(a==NULL || n<2) return;
    
	// Sort them
    sort(a, a+n);

    int low = 0, high = n-1;
    while(low < high){
        if(a[low]+a[high] > sum)
            --high;
        else if(a[low]+a[high] < sum)
            ++low;
        else{
            cout<<sum << " = " << a[low]<<" + "<<a[high]<<endl;
            ++low; --high;
        }
            
    }
}

int main(){
    int n = 6, sum = 6;
    int a[] = {
        1, 2, 3, 4, 5, 6
    };
    PrintPairSum(a, n, sum);
    return 0;
}
