// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>
#include "drawing_skylines.h"

using namespace std;

int main(int argc, char* argv[]) {
  default_random_engine gen((random_device())());
  // Random test 2000 times.
/*/  for (int times = 0; times < 2000; ++times) {
	
  for (int times = 0; times < 1; ++times) {
    int n;
    if (argc == 2) {
      n = atoi(argv[1]);
    } else {
      //uniform_int_distribution<int> dis(1, 5000);
      uniform_int_distribution<int> dis(1, 20);
      n = dis(gen);
    }
    vector<Skyline<int, int>> A;
    for (int i = 0; i < n; ++i) {
      uniform_int_distribution<int> left_dis(0, 999);
      int left = left_dis(gen);
      uniform_int_distribution<int> right_dis(left, left + 200);
      int right = right_dis(gen);
      uniform_int_distribution<int> height_dis(0, 99);
      int height = height_dis(gen);
      A.emplace_back(Skyline<int, int>{left, right, height});
    }
    vector<Skyline<int, int>> ans = drawing_skylines(A);
    cout << "n = " << n << endl;
    // just check there is no overlap.
    for (size_t i = 0; i < ans.size(); ++i) {
      assert(ans[i].left <= ans[i].right);
      if (i > 0) {
        assert(ans[i - 1].right <= ans[i].left);
        assert(ans[i - 1].right != ans[i].left ||
               ans[i - 1].height != ans[i].height);
      }
    }
  }
*/
	
    vector<Skyline<int, int>> A = { {1, 5, 11}, {2, 7, 6}, {3, 9, 13}, {12, 16, 7}, {14, 25, 3}, 
					{19, 22, 18}, {23, 29, 13}, {24, 28, 4}};
    vector<Skyline<int, int>> ans = drawing_skylines(A);
    for (size_t i = 0; i < ans.size(); ++i) {
		cout << ans[i].left << " " << ans[i].right << " " << ans[i].height << endl;
	}

  return 0;
}
