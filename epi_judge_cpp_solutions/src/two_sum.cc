#include <algorithm>
#include <iterator>
#include <vector>
#include <unordered_map>

#include "../test_framework/generic_test.h"

using std::size;
using std::vector;

bool HasTwoSum1(vector<int>& A, int target) {
  for (int i=0; i<A.size(); ++i) {  // A.size()-1 -> A.size() to allow add itself
    for (int j=i; j<A.size(); ++j) {  // j=i+1 -> j=i to allow add itself
      int sum = A[i] + A[j];
      if (sum == target) {
        return true;
      }
    }
  }
  return false;
}

// Two Sum 2: Time=O(nlogn), Space=O(1) for inspace sort
bool HasTwoSum2(vector<int>& A, int target) {
  std::sort(A.begin(), A.end());
  int i = 0, j = A.size() - 1;
  while (i <= j) {  // i < j -> i<=j to allow add itself
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

bool HasTwoSum3(vector<int>& A, int target) {
  std::unordered_map<int, int> mp;

  for (int i=0; i<A.size(); ++i) {
    int complement = target - A[i];
    if(complement * 2 == target ||  // allow add itself
        mp.find(complement) != mp.end()) {
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
