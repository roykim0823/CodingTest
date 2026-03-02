// Implement randomly generate a set of m integers. Each element must have equal probability of being chosen
// -> to avoid pick the same element twice -> shink or shift the array -> expensive operations
// --> Using swap for the picked element
#include <iostream>
#include <cstdlib>
using namespace std;

void Swap(int &a, int &b){
    int t = a;
    a = b;
    b = t;
}
int *PickMRandomly(int a[], int n, int m){
	int *temp = new int[n];		// copy a to temp (a should not be modified)
	for(int i=0; i<n; i++)
		temp[i]=a[i];
    
	for(int i=0; i<m; ++i){
        int j = rand() % (n-i) + i;
        Swap(temp[i], temp[j]);
    }
	return temp;
}
int main(){
    srand((unsigned)time(0));
    int n = 9, m = 5;
    int a[] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9
    };
    int *subset=PickMRandomly(a, n, m);
    for(int i=0; i<m; ++i)
        cout<<subset[i]<<" ";
	cout<<endl;
    for(int i=0; i<n; ++i)
        cout<<a[i]<<" ";
	cout<<endl;
	delete subset;
    return 0;
}
