// Q. Imagine you have a square matrix, where each cell (pixel) is either black or white. Design an 
//    algorithm to find the maximum subsquare such that all four borders are filled with block pixels
// 	  -> white pixel=0, black pixel>0 (default=1)

/* Solutions:
1. Simple Naive: O(n^4)
	We know that the biggest possible square has a length of size N, and there is only one possible
	square of size NxN. We can easily check for that square and return if we find it
	If we do not find a square of size NxN, we can try the nest best thing: (N-1)x(N-1).
	We iterate through all squars of this size and return the first one we find. We then do the same
	for N-2, N-3, and so on. Since we are searching ;rogressively smaller squares, we know that the 
	first square we find is the biggest.

2. Pre-Processing: O(n^3)
	Cut down the time of isSquare to O(1).
	Ierate from right to left, bottom to top. At each cell, we do the following computation:
	if A[r][c] is white, put zero right and zero below are 0
	else 
		A[r][c].zeroRight = A[r][c+1].zeroRight+1
		A[r][c].zeroBelow = A[r+1][c].zeroBelow+1
*/

#include <iostream>
#include <cstdio>
using namespace std;

const int MAX_N = 100;
int matrix[MAX_N][MAX_N];
int preProcessed[MAX_N][MAX_N];

struct SubSquare{
    int row, col, size;
};

inline int max(int a, int b){
    return a > b ? a : b;
}

bool IsSquare(int row, int col, int size){  // O(N) -> can be O(1) by preprocessing
    for(int i=0; i<size; ++i){
        if(matrix[row][col+i] == 0)			// top
            return false;
        if(matrix[row+size-1][col+i] == 0)  // bottom
            return false;
        if(matrix[row+i][col] == 0)         // left
            return false;
        if(matrix[row+i][col+size-1] == 0)  // right
            return false;
    }
    return true;
}

SubSquare FindSubSquare(int n){
    int max_size = 0; //最大边长
    int col = 0;
    SubSquare sq;
    while(n-col > max_size){
        for(int row=0; row<n; ++row){
            int size = n - max(row, col);  	// n - row or n - col since it is Square Matrix
            while(size > max_size){
                if(IsSquare(row, col, size)){
                    max_size = size;
                    sq.row = row;
                    sq.col = col;
                    sq.size = size;
                    break;
                }
                --size;
            }
        }
        ++col;
    }
    return sq;
}

int main(){
    freopen("18.11.in", "r", stdin);
    int n;
    cin>>n;
    for(int i=0; i<n; ++i)
        for(int j=0; j<n; ++j)
            cin>>matrix[i][j];
    SubSquare sq = FindSubSquare(n);
    cout<<"top:    "<<sq.row<<endl;
    cout<<"bottom: "<<sq.row+sq.size-1<<endl;
    cout<<"left:   "<<sq.col<<endl;
    cout<<"right:  "<<sq.col+sq.size-1<<endl;
    fclose(stdin);
    return 0;
}
