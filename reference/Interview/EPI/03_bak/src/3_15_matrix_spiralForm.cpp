#include <iostream>
#include <vector>

using namespace std;

template <typename T>
vector<T> matrixToSpiralFrom (vector<vector<T>> matrix) {
    vector<T> spiral;
	int row_begin, row_end;	// row indices
	int col_begin, col_end;	// col indices

	row_begin=col_begin=0;
	row_end=matrix.size();
	col_end=matrix[0].size();

	while(row_begin < row_end && col_begin < col_end) {
		// read right
		for(int i=col_begin; i<col_end; i++) {
			spiral.push_back(matrix[row_begin][i]);
		}
		row_begin++;                    

		// read down
		for(int i=row_begin; i<row_end; i++) {
			spiral.push_back(matrix[i][col_end-1]);
		}
		col_end--;                                       

		// read left
		if(row_begin<row_end) {   	// check if all row is not done
			for(int i=col_end-1; i>=col_begin; i--) {
				spiral.push_back(matrix[row_end-1][i]);
			}
			row_end--;                    
		}

		// read up
		if(col_begin<col_end) {   	// check if all col is not done
			for(int i=row_end-1; i>=row_begin; i--) {
				spiral.push_back(matrix[i][col_begin]);
			}
			col_begin++;
		}                                                
	}

	return spiral;
}

template <typename T>
vector<vector<T>> fillMatrix(int n, int m) {
	vector<vector<T>> mat;
	int value=1;
	for(int i=0; i<n; i++) {
		vector<T> row;
		for(int j=0; j<m; j++) {
			row.push_back(value++);
		}
		mat.push_back(row);
	}
	return mat;
}

template <typename T>
void printMat(vector<vector<T>> mat) {
	for(auto row : mat) {
		for (auto v : row) 
			cout << v << "\t";
		cout << endl;
	}
}

template <typename T>
void print(vector<T> vec) {
	for(auto v : vec) {
		cout << v << " ";
	}
	cout << endl;
}

int main() {
	for(int i=1; i<5; i++) {
		for(int j=1; j<5; j++) {

	vector<vector<int>> m = fillMatrix<int>(i, j);
	cout << "Matrix : " << endl;
	printMat<int>(m);
	vector<int> s = matrixToSpiralFrom(m);
	cout << "Spiral Form : " << endl;
	print(s);
	cout << endl;
	getchar();
		}
	}
}

	
