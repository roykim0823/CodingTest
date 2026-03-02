// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

using std::cout;
using std::default_random_engine;
using std::endl;
using std::ostream_iterator;
using std::random_device;
using std::uniform_int_distribution;
using std::vector;
using std::swap;

template<typename T>
vector<T> permutate(vector<T> &vec) {	// random shuffle
	vector<T> t(vec);

	int n=t.size();
  	default_random_engine gen((random_device())());
	for(int i=0; i<n; ++i) {
		//int j = rand() % (n-i) + i;
      	uniform_int_distribution<int> dis(i, n-1);
		int j = dis(gen);
		swap(t[i], t[j]);
	}
	return t;
}

int main(int argc, char *argv[]) {
  default_random_engine gen((random_device())());
  int n;
  for (int times = 0; times < 10; ++times) {
    if (argc == 2) {
      n = atoi(argv[1]);
    } else {
      uniform_int_distribution<int> dis(1, 20);
      n = dis(gen);
    }
    vector<int> A, perm;
    for (int i = 0; i < n; ++i) {
      A.emplace_back(i);
      perm.emplace_back(i);
    }
	copy(A.begin(), A.end(), ostream_iterator<int>(cout, " "));
    cout << endl;

    // knuth shuffle
    random_shuffle(perm.begin(), perm.end());
    copy(perm.begin(), perm.end(), ostream_iterator<int>(cout, " "));
    cout << endl;

    vector<int> B;
    B = permutate(A);
    copy(B.begin(), B.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
  }
  return 0;
}
