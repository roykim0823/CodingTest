#include <cmath>
#include <vector>
#include "test_framework/generic_test.h"

using std::vector;

// One to One relationship bewteen the 2^|S| bit array of length |S|
// and the set of all subsets of S
// O(|S|*2^|S|) time, O(1) space
vector<vector<int>> GeneratePowerSet(const vector<int>& input_set) {
  vector<vector<int>> power_set;
  for (int subset_idx = 0; subset_idx < (1 << input_set.size()); ++subset_idx) {
    int bit_array = subset_idx;
    vector<int> subset;
    while (bit_array) {
      int idx = log2(bit_array & ~(bit_array-1));  // log2(the right most set bit)
      subset.emplace_back(input_set[idx]);
      bit_array &= bit_array - 1;  // clear the right most '1' in bit_array
    }
    power_set.emplace_back(subset);
  }
  return power_set;
}

// Find all factors of the product of a given list of numbers (or primes)
// ex) {2, 3, 7} -> 1, 2, 3, 6, 7, 14, 21, 42
// modify the generate_power_set
vector<int> factors(const vector<int> &S) {
  vector<int> products;
  for(int i=0; i< ( 1<< S.size()); ++i) { // 1<<S.size() = 2^S.size()
    int x=i;
    int product = 1;
    bool one=false;
    while (x) {
      int idx = log2( x & ~(x-1));  // log2(the right most set bit)
      if(S[idx]==1) {               // '1' is element then skip the product calculation
        one=true;
        break;;
      }
      product *= S[idx];  // calcuate the product
      x &= x-1;           // x = x with the right most '1' is cleared
    }
    if(one==false)
      products.push_back(product);
  }
  return products;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args {argv + 1, argv + argc};
  std::vector<std::string> param_names {"input_set"};
  GenericTestMain(args, "power_set.cc", "power_set.tsv", &GeneratePowerSet, &UnorderedComparator<vector<vector<int>>>, param_names);
}
// clang-format on
