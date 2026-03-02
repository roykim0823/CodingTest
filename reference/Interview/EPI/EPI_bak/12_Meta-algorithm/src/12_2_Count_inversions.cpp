// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include "countInversion.h"

template <typename T>
void print(vector<T> vec) {
	for(auto v : vec) {
		cout << v << " ";
	}
	cout << endl;
}

int main(int argc, char* argv[]) {
  default_random_engine gen((random_device())());
  //for (int times = 0; times < 1000; ++times) {
  for (int times = 0; times < 10; ++times) {
    int n;
    if (argc == 2) {
      n = atoi(argv[1]);
    } else {
      uniform_int_distribution<int> dis(1, 10000);
      n = dis(gen);
    }
    vector<int> A;
    for (int i = 0; i < n; ++i) {
      uniform_int_distribution<int> dis(-1000000, 1000000);
      A.emplace_back(dis(gen));
    }
    assert(check_inversions(A) == count_inversions_stl(A));
    assert(check_inversions(A) == count_inversions(A));
  }
  return 0;
}
