#include <algorithm>
#include <iterator>
#include <vector>
#include <unordered_map>

#include "test_framework/generic_test.h"

using std::size;
using std::vector;

// BruteForce: Time=O(n^2), Space=O(1)
bool HasTwoSum1(vector<int>& A, int target) {
  for (int i=0; i<(int)A.size()-1; ++i) {
    int complement = target - A[i];
    for (int j=i+1; j<A.size(); ++j) {
      if (A[j] == complement) {
        return true;
      }
    }
  }
  return false;
}

// Using Sorting: Time=O(nlogn), Space=O(1) for inspace sort
bool HasTwoSum2(vector<int>& A, int target) {
  std::sort(A.begin(), A.end());
  int i = 0, j = A.size() - 1;
  while (i < j) {
    int sum = A[i] + A[j];
    if (sum == target) {
      return true;
    } else if (sum < target) {
      ++i;
    } else {  // sum > t.
      --j;
    }
  }
  return false;
}

// Dynamic Programming: Time=O(n), Space=O(n)
bool HasTwoSum3(vector<int>& A, int target) {
  std::unordered_map<int, int> mp;

  for (int i=0; i<A.size(); ++i) {
    int complement = target - A[i];
    if(mp.find(complement) != mp.end()) {
      return true;
    }
    mp[A[i]] = i;

  }
  return false;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "target"};
  bool test1 = GenericTestMain(args, "two_sum.cc", "two_sum.tsv", &HasTwoSum1,  // A is sorted in two_sum.tsv
                         DefaultComparator{}, param_names);
  bool test2 = GenericTestMain(args, "two_sum.cc", "two_sum.tsv", &HasTwoSum2,  // A is sorted in two_sum.tsv
                         DefaultComparator{}, param_names);
  bool test3 = GenericTestMain(args, "two_sum.cc", "two_sum.tsv", &HasTwoSum3,  // A is sorted in two_sum.tsv
                         DefaultComparator{}, param_names);
  return test1 && test2 && test3;
}
