#include <vector>
#include "test_framework/generic_test.h"

using std::vector;

struct Subarray {
  int start = 0, end = 0;
};

Subarray FindLongestIncreasingSubarray(const vector<int>& A) {
  int max_length = 1;
  Subarray result;
  int i = 0;
  while (i < size(A) - max_length) {
    // Backward check and skip if A[j - 1] >= A[j].
    bool is_skippable = false;
    for (int j = i + max_length; j > i; --j) {
      if (A[j - 1] >= A[j]) {
        i = j;
        is_skippable = true;
        break;
      }
    }

    // Forward check if it is not skippable.
    if (!is_skippable) {
      i += max_length;
      while (i < size(A) && A[i - 1] < A[i]) {
        ++i, ++max_length;
      }
      result = {i - max_length, i - 1};
    }
  }
  return result;
}

// Simple Version
// O(n) Time, O(1) Space
Subarray findLongestIncSubArray(const vector<int>& A) {
    // 'max' to store the length of longest increasing subarray 
    // 'len' to store the lengths of longest increasing subarray at different  
    // instants of time 
    int max = 1, len = 1, maxIndex = 0; 
      
    // traverse the array from the 2nd element 
    for (int i=1; i<A.size(); ++i) { 
        // if current element if greater than previous element, then this
        // element helps in building up the previous increasing subarray
        // encountered so far 
        if (A[i] > A[i-1]) 
            ++len; 
        else { 
            // check if 'max' length is less than the length 
            // of the current increasing subarray. If true,  
            // then update 'max' 
            if (max < len) { 
                max = len; 
                  
                // index assign the starting index of  
                // longest increasing contiguous subarray.    
                maxIndex = i - max; 
            } 
                  
            // reset 'len' to 1 as from this element 
            // again the length of the new increasing 
            // subarray is being calculated     
            len = 1;     
        }     
    } 
      
    // comparing the length of the last 
    // increasing subarray with 'max' 
    if (max < len) {  
        max = len; 
        maxIndex = A.size() - max; 
    } 
  
  Subarray result = { maxIndex, maxIndex+max-1 };
  return result;
} 

int FindLongestIncreasingSubarrayWrapper(const vector<int>& A) {
  // Subarray result = FindLongestIncreasingSubarray(A);
  Subarray result = findLongestIncSubArray(A);
  return result.end - result.start + 1;
}

// clang-format off


int main(int argc, char* argv[]) {
  std::vector<std::string> args {argv + 1, argv + argc};
  std::vector<std::string> param_names {"A"};
  return GenericTestMain(args, "longest_increasing_subarray.cc", "longest_increasing_subarray.tsv", &FindLongestIncreasingSubarrayWrapper, DefaultComparator{}, param_names);
}
// clang-format on
