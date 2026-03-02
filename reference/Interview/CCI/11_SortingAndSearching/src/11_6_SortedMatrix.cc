// Q. Given a matrix in which each row and each column is sorted in ascending order,
//    write a method to find an element 
// Naive: perform BinarySearch on each rows -> O(m log n), mXn matrix

#include <iostream>
#include <cstdio>
using namespace std;

int d[20][20];


int search(int m, int n, int x){	//O(n)
    int r = 0, c = n-1; 			// Start with the right most column in the top row
    while(r<m && c>=0){
        if(d[r][c] == x) return (r * n + c);
        else if(d[r][c] < x) ++r; 	// if the first of the col < x, then the entire row are less than x
        else --c;                   // if the first of the col > x, then the entire col are larger than x
    }
    return -1;
}

int main(){
    freopen("11_6.in", "r", stdin);
    int m, n;
    cin>>m>>n;
    for(int i=0; i<m; ++i)
        for(int j=0; j<n; ++j)
            cin>>d[i][j];

    int k = search(m, n, 13);
    if(k == -1) cout<<"not found"<<endl;
    else cout<<"position: "<<k/n<<" "<<k%n<<endl;
    fclose(stdin);
    return 0;
}
