// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

using std::cout;
using std::default_random_engine;
using std::endl;
using std::max;
using std::min;
using std::numeric_limits;
using std::random_device;
using std::uniform_int_distribution;
using std::vector;

int find_kth_in_two_sorted_arrays(const vector<int>& A,
                                  const vector<int>& B,
                                  int k) {
  // Lower bound of elements we will choose in A.
  int b = max(0, static_cast<int>(k - B.size()));
  // Upper bound of elements we will choose in A.
  int t = min(static_cast<int>(A.size()), k);

  while (b < t) {
    int x = b + ((t - b) >> 1);
    int A_x_1 = (x <= 0 ? numeric_limits<int>::min() : A[x - 1]);
    int A_x = (x >= A.size() ? numeric_limits<int>::max() : A[x]);
    int B_k_x_1 = (k - x <= 0 ? numeric_limits<int>::min() : B[k - x - 1]);
    int B_k_x = (k - x >= B.size() ? numeric_limits<int>::max() : B[k - x]);

    if (A_x < B_k_x_1) {
      b = x + 1;
    } else if (A_x_1 > B_k_x) {
      t = x - 1;
    } else {
      // B[k - x - 1] <= A[x] && A[x - 1] < B[k - x].
      return max(A_x_1, B_k_x_1);
    }
  }

  int A_b_1 = b <= 0 ? numeric_limits<int>::min() : A[b - 1];
  int B_k_b_1 = k - b - 1 < 0 ? numeric_limits<int>::min() : B[k - b - 1];
  return max(A_b_1, B_k_b_1);
}

// below is the implementation from AFI, with FindOrderStatistic renamed to 
// find_kth_in_two_sorted_arrays for consistency with EFI
int find_kth_in_two_sorted_arrays_AFI(vector<int> a1, vector<int>a2, int k) {
  // Check the validity of input.
  assert(k > 0);
  assert(a1.size() + a2.size() >= k);
  // Find an index begin <= l < end such that a1[0]..a1[l-1]
  // and a2[0]..a2[k-l-1] are the smallest k numbers.
  int begin = max(0, (int) (k - a2.size()));
  int end = min((int) a1.size(), k) +1;
  long l;
  while(begin < end) {
    l = (begin + end)/2;
    // can we include a1[l] in the k smallest numbers?
    if((l < a1.size()) && (k-l > 0) && (a1[l] < a2[k-l-1])) {
      begin = l + 1;
    } else if ((l > 0) && (k-l < a2.size()) && (a1[l-1] > a2[k-l])) {
      // this is the case where we can discard a[l-1]
      // from the set of k smallest numbers.
      end = l;
    } else {
      // we found our answer since both inequalities were false.
      begin = l;
      break;
    }
  }
  if (begin == 0) {
    return a2[k - begin -1];
  } else if (begin == k) {
    return a1[k-1];
  } else {
    return max(a1[begin -1], a2[k - begin -1]);
  }
}

template <typename T>
T check_answer(const vector<T>& A, const vector<T>& B, int k) {
  int i = 0, j = 0, count = 0;
  T ret;
  while ((i < A.size() || j < B.size()) && count < k) {
    if (i < A.size() && j < B.size()) {
      if (A[i] < B[j]) {
        ret = A[i];
        ++i;
      } else {
        ret = B[j];
        ++j;
      }
    } else if (i < A.size()) {
      ret = A[i];
      ++i;
    } else {
      ret = B[j];
      ++j;
    }
    ++count;
  }
  return ret;
}

void small_test() {
  // AA: handwritten test
  vector<int> A0;
  vector<int> B0;
  A0.emplace_back(0);
  A0.emplace_back(1);
  A0.emplace_back(2);
  A0.emplace_back(3);
  B0.emplace_back(0);
  B0.emplace_back(1);
  B0.emplace_back(2);
  B0.emplace_back(3);
  assert(0 == find_kth_in_two_sorted_arrays(A0, B0, 1));
  assert(0 == find_kth_in_two_sorted_arrays(A0, B0, 2));
  assert(1 == find_kth_in_two_sorted_arrays(A0, B0, 3));
  assert(1 == find_kth_in_two_sorted_arrays(A0, B0, 4));
  assert(2 == find_kth_in_two_sorted_arrays(A0, B0, 5));
}

int main(int argc, char* argv[]) {
  small_test();
  default_random_engine gen((random_device())());
  // Random test 10000 times.
  //for (int times = 0; times < 10000; ++times) {
  for (int times = 0; times < 10; ++times) {
    vector<int> A, B;
    int n, m, k;
    if (argc == 3) {
      n = atoi(argv[1]), m = atoi(argv[2]);
      uniform_int_distribution<int> k_dis(1, n + m);
      k = k_dis(gen);
    } else if (argc == 4) {
      n = atoi(argv[1]), m = atoi(argv[2]), k = atoi(argv[3]);
    } else {
      uniform_int_distribution<int> dis(1, 10000);
      n = dis(gen), m = dis(gen);
      uniform_int_distribution<int> k_dis(1, n + m);
      k = k_dis(gen);
    }
    uniform_int_distribution<int> dis(0, 99999);
    for (size_t i = 0; i < n; ++i) {
      A.emplace_back(dis(gen));
    }
    for (size_t i = 0; i < m; ++i) {
      B.emplace_back(dis(gen));
    }
    sort(A.begin(), A.end()), sort(B.begin(), B.end());
    /*
    for (int i = 0; i < A.size(); ++i) {
      cout << A[i] << ' ';
    }
    cout << endl;
    for (int j = 0; j < B.size(); ++j) {
      cout << B[j] << ' ';
    }
    cout << endl;
    */
    int ans = find_kth_in_two_sorted_arrays(A, B, k);
    //cout << k << "th = " << ans << endl;
    assert(ans == check_answer(A, B, k));
    A.clear(), B.clear();
  }
  return 0;
}
