#include <algorithm>
#include <iterator>
#include <vector>

#include "test_framework/generic_test.h"

#define main _main
#include "00_two_sum.cc"
#undef main

using std::vector;

bool HasThreeSum(vector<int> A, int t) {
  sort(begin(A), end(A));
  // Finds if the sum of two numbers in A equals to t - a.
  // how to exclude the current a?
  return any_of(begin(A), end(A), [&](int a) { return HasTwoSum(A, t - a); });
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "t"};
  return GenericTestMain(args, "three_sum.cc", "three_sum.tsv", &HasThreeSum,
                         DefaultComparator{}, param_names);
}
