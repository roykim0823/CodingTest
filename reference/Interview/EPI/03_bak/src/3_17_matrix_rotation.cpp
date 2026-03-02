// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::default_random_engine;
using std::ostream_iterator;
using std::random_device;
using std::uniform_int_distribution;
using std::vector;

void print_matrix(const vector<vector<int>> &A, const std::string str) {
  cout << str << endl;
  for (int i = 0; i < A.size(); ++i) {
    copy(A[i].begin(), A[i].end(), ostream_iterator<int>(cout, " "));
    cout << endl;
  }
}

// R is rotated matrix
void check_answer(const vector<vector<int>> &A, const vector<vector<int>> &R) {
  int idx = A.size()-1;
  for (int j = A.size() - 1; j >= 0; --j) {
    for (int i = 0; i < A.size(); ++i) {
      assert(R[j][idx-i]== A[i][j]);  	// A[i][j] -> Rotate A[j][A.size()-1-i]
    }
  }
}

void fillMatrix(vector<vector<int>> &A) {
	int k = 1;
    for (int i = 0; i < A.size(); ++i) {
      for (int j = 0; j < A[i].size(); ++j) {
        A[i][j] = k++;
      }
    }
}

vector<vector<int>> rotate_matrix(const vector<vector<int>>& A) {
  vector<vector<int>> RA(A);
  int idx=RA.size()-1;
  for (int i = 0; i < (RA.size() >> 1); ++i) { 	// RA.size()/2 -> the number of rotating layers
    for (int j = i; j < idx-i; ++j) {
      int temp = RA[i][j];                  	// save top
      RA[i][j] = RA[idx - j][i];                // left 	-> top
      RA[idx - j][i] = RA[idx - i][idx - j];    // bottom	-> left
      RA[idx - i][idx - j] = RA[j][idx - i];    // right	-> bottom
      RA[j][idx - i] = temp;                	// top		-> right
    }
  }
  return RA;
}

int main(int argc, char *argv[]) {
	for(int i=0; i<8; i++) {
      	vector<vector<int>> A(i, vector<int>(i));
		fillMatrix(A);
		print_matrix(A, "Matrix A");
		vector<vector<int>> RA = rotate_matrix(A);
		print_matrix(RA, "Roated A");
      	check_answer(A, RA);
    }
  return 0;
}
