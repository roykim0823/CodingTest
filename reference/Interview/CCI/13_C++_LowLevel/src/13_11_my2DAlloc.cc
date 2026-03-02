// Q. Write a function in C called my2DAlloc which allocates a two-diminsional array.
//    Minimize the number of calls to malloc and make sure that the memory is accessible by the notation
//    "arr[i][j]."

/* 
Checks:
	A two-dimensional array is essentially an array of arrays. Since we use pointers with arrays, we
	can use double pointers to create a double array.
*/

#include <iostream>
#include <stdlib.h>
using namespace std;

// Create a one-diminsional array of pointers. Then, for each array index, we create a new one-dimensional
// array. This gives us a two-dimensional array that can be accessed via array indices.
int** My2DAlloc(int rows, int cols){
    int **arr = (int**)malloc(rows*sizeof(int*)); 	// array of pointers for each row array pointer
    for(int i=0; i<rows; ++i)
        arr[i] = (int*)malloc(cols*sizeof(int));	// allocate each row at each row array pointer
    return arr;
}

void My2DDealloc(int **rowptr, int rows) {
	for(int i=0; i<rows; i++) {
		free(rowptr[i]);
	}
	free(rowptr);
}

// One malloc call version
int** My2DAlloc1(int rows, int cols){
    int header = rows * sizeof(int*);		// storage for each row's starting address
    int data = rows * cols * sizeof(int);  	// array data
    int **arr = (int**)malloc(header + data);
    int *buf = (int*)(arr + rows);			// starting address of array data
    for(int i=0; i<rows; ++i)
        arr[i] = buf + i * cols;   			// store each row's staring address in the header array
    return arr;
}

void My2DDealloc1(int **arr) {
	free(arr);
}

int main(){
    int **arr = My2DAlloc1(4, 5);
    arr[2][3] = 23;
    cout<<arr[2][3]<<endl;
	My2DDealloc1(arr);
    return 0;
}
