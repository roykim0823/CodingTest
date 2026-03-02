// Q. Write an algorithm such that if an element in an MxN matrix is 0, 
//    its entire row and column are set to 0.

/* 
Checks:
	At first glance, it seems easy: just iterate through the matrix and every time we see a cell
	with value zero, set its row and column to 0. There's one problem with this solution though:
	when we come across other cells in that row or column, we'll see the zeros and change their 
	row and column to zero. Soon, out entire matrix will be set to zeros

	1. Use a second matrix which flags the zero locations. Then do a second pass thourgh the matrix
	   -> O(MN) space
	2. We don't need MxN space since we set the entire row and column to zero
	   Use M size flag array and N size flag array
	   -> O(M+N) space
*/

#include <iostream>
#include <string>
#include <stdlib.h>	// srand
#include <time.h> 	// time
using namespace std;

const int N=5;

void randomMatrix(int matrix[][N], int n) 
{
	srand(time(NULL));

	for(int i=0; i<n; i++) {
		for(int j=0; j<n; j++) {
			matrix[i][j]=rand()%10;
		}
	}
}
            
void printMatrix(int matrix[][N], int n) 
{
	for(int i=0; i<n; i++) {
		for(int j=0; j<n; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

void setZeros(int matrix[][N], int n) 
{
	bool row[n], col[n];

	// initialize row, col
	for(int i=0; i<n; i++)	{
		row[i]=false;
		col[i]=false;
	}

	// Store the row and column index with value 0
	for(int i=0; i<n; i++) {
		for(int j=0; j<n; j++) {
			if(matrix[i][j]==0) {
				row[i]=true;
				col[j]=true;
			}
		}
	}

	// set matrix[i][j] to 0 if either row i or col j has a 0
	for(int i=0; i<n; i++) {
		for(int j=0; j<n; j++) {
			if(row[i] || col[j])
				matrix[i][j]=0;
		}
	}
}


int main()
{
	int matrix[N][N];
	randomMatrix(matrix, N);
	printMatrix(matrix, N);
	cout << endl;
	setZeros(matrix, N);
	printMatrix(matrix, N);
	cout << endl;
	return 0;
}
