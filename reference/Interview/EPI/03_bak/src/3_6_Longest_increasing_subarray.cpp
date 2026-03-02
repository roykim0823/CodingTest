// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

using std::cout;
using std::default_random_engine;
using std::endl;
using std::pair;
using std::random_device;
using std::uniform_int_distribution;
using std::vector;

// @include
pair<int, int> find_longest_increasing_subarray(const vector<int> &A) {
  int max_len = 1;	// each element is a minimum increasing subarray
  pair<int, int> ans(0, 0);
  int i = 0;
  while (i < A.size()) {
    // Checks backwardly and skip if A[j] >= A[j + 1].
    bool is_skippable = false;
    for (int j = i + max_len - 1; j >= i; --j) {
      if (j + 1 >= A.size() || A[j] >= A[j + 1]) {
        i = j + 1;
        is_skippable = true;
        break;
      }
    }

    // Checks forwardly if it is not skippable.
    if (is_skippable == false) {
      i += max_len - 1;
      while (i + 1 < A.size() && A[i] < A[i + 1]) {
        ++i, ++max_len;
      }
      ans = {i - max_len + 1, i};
    }
  }
  return ans;
}
// @exclude

void simple_test() {
  auto ans = find_longest_increasing_subarray({-1, -1});
  assert(ans.first == 0 && ans.second == 0);
  ans = find_longest_increasing_subarray({1, 2});
  assert(ans.first == 0 && ans.second == 1);
}

template<typename T>
void print(vector<T> vec, int beg=0, int end=0) {
	//for(auto v : vec) {
	if(end==0) end=vec.size();
	for(int i=beg; i<end; i++) {
		cout << vec[i] << " ";
	}
	cout << endl;
}

int main(int argc, char *argv[]) {
  simple_test();
  default_random_engine gen((random_device())());
  for (int times = 0; times < 10; ++times) {
    vector<int> A;
    if (argc > 2) {
      for (size_t i = 1; i < argc; ++i) {
        A.emplace_back(atoi(argv[i]));
      }
    } else {
      int n;
      if (argc == 2) {
        n = atoi(argv[1]);
      } else {
        //uniform_int_distribution<int> dis(1, 1000000);
        uniform_int_distribution<int> dis(1, 20);
        n = dis(gen);
      }
      uniform_int_distribution<int> pos_or_neg(0, 1);
      uniform_int_distribution<int> dis(0, n - 1);
      for (int i = 0; i < n; ++i) {
        A.emplace_back((pos_or_neg(gen) ? -1 : 1) * dis(gen));
      }
    	pair<int, int> result = find_longest_increasing_subarray(A);
		print(A);
    	cout << result.first << ' ' << result.second << " -> " ;
		print(A, result.first, result.second+1);
    }
  }
  return 0;
}
