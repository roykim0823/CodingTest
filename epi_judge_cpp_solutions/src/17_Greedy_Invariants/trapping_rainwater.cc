#include <algorithm>
#include <iterator>
#include <vector>

#include "../../test_framework/generic_test.h"
using std::size;
using std::vector;

// Problem
// Given an array of integers representing an elevation map
// where the width of each bar is 1, return how much rainwater
// can be trapped.

// Analysis
// area += min(maxLeft, maxRight) - currentHeight

// BruthForce: Time=O(n^2), Space=O(1)
int trap_water1(std::vector<int>& heights) {
  int total_water = 0;
  for (int i=0; i<heights.size(); ++i) {
    // Calculate each ith's leftwall and rightwall
    int leftInx = i, rightInx = i, maxLeft = 0, maxRight = 0;

    while(leftInx>=0) {
      maxLeft = std::max(maxLeft, heights[leftInx]);
      --leftInx;
    }
    while(rightInx < heights.size()) {
      maxRight = std::max(maxRight, heights[rightInx]);
      ++rightInx;
    }
    int current_water = std::min(maxLeft, maxRight) - heights[i];
    if (current_water>0) {
      total_water += current_water;
    }
  }
  return total_water;
}

// Optimized: Time=O(n), Space=O(1)
int trap_water2(std::vector<int>& heights) {
  int total_water = 0;
  int leftInx=0, rightInx=heights.size()-1;
  int maxLeft=0, maxRight=0;

  while(leftInx < rightInx) {
    if(heights[leftInx] <= heights[rightInx]) {
      if(heights[leftInx] >= maxLeft) {
        maxLeft = heights[leftInx];
      } else {
        total_water += maxLeft - heights[leftInx];  // since maxLeft is smaller or equal to maxRight
      }
      ++leftInx;
    } else {
      if(heights[rightInx] >= maxRight) {
        maxRight = heights[rightInx];
      } else {
        total_water += maxRight - heights[rightInx];
      }
      --rightInx;
    }
  }
  return total_water;
}

// Dynamic Programming: Time=O(n), Space=O(n)
int trap_water3(std::vector<int>& heights) {
  int n = heights.size();

  // calculate each index's left wall and right wall
  vector<int> left(n);
  vector<int> right(n);

  left[0] = heights[0];
  right[n-1] = heights[n-1];

  for (int i=1; i<n; ++i) {
    left[i] = std::max(left[i-1], heights[i]);
    right[n-(i+1)] = std::max(right[n-i], heights[n-(i+1)]);
  }

  int total_water = 0;
  // caculate the trapped water
  for (int i=0; i<n; ++i) {
    total_water+= (std::min(left[i], right[i])) - heights[i];
  }
  return total_water;
}



int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"heights"};
  bool test1 = GenericTestMain(args, "trapping_rainwater.cc", "trapping_rainwater.tsv", &trap_water1,
                         DefaultComparator{}, param_names);
  bool test2 = GenericTestMain(args, "trapping_rainwater.cc", "trapping_rainwater.tsv", &trap_water2,
                         DefaultComparator{}, param_names);
  bool test3 = GenericTestMain(args, "trapping_rainwater.cc", "trapping_rainwater.tsv", &trap_water3,
                         DefaultComparator{}, param_names);
  return test1 && test2 && test3;
}
