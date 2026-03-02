// Q. You are given an array of integers (both positive and negative). Find the contiguous sequence
//    with the largest sum. Return the sum.

/*
Checks:
	This is a challenging problem, but an extremely common one. Let's approach this by looking
	at an example.
		2 3 -8 -1 2 4 -2 3
*/

#include <iostream>
using namespace std;

bool g_Invalid = false;

int GetMaxSum(int a[], int n){
    if(a==NULL || n<=0){
        g_Invalid = true;
        return 0;
    }
    g_Invalid = false;
    
    //int max_sum = 1<<31; // Min Int
    int max_sum = 0; // Min Int, MaxSum=0 if all numbers are negative 
    int cur_sum = 0;
    for(int i=0; i<n; ++i){
        if(cur_sum <= 0)
            cur_sum = a[i];
        else
            cur_sum += a[i];

        if(cur_sum > max_sum)
            max_sum = cur_sum;
    }

    return max_sum;
}
int main(){
    int a[] = {2, 3, -8, -1, 2, 4, -2, 3}; 	// size=8
    int b[] = {-2, -8, 3, -2, 4, -10};     	// size=6
    int c[] = {-3, -10, -5, -1, 0};     			// size=3
    
	cout<<"a= " << GetMaxSum(a, 8) <<endl;
    cout<<"b= " << GetMaxSum(b, 6) <<endl;
    cout<<"c= " << GetMaxSum(c, 5) <<endl;
    
	return 0;
}
