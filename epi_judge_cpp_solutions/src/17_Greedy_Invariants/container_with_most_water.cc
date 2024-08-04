#include <algorithm>
#include <iterator>
#include <vector>

#include "../../test_framework/generic_test.h"
using std::size;
using std::vector;

// Problem
// You are given an array of positive integers where each integer represents the height
// of a verical line on a chart.
// Find two lines which together with the x-axis forms a container that would hold the
// greatest amount of water. Return the area of water it would hold.

// Analysis
// area = min(a, b) x (bi - ai) : height x width

// BruthForce: Time=O(n^2), Space=O(1)
int maxArea1(std::vector<int>& heights) {
  int max_area = 0;
  for (int i=0; i<(int)heights.size()-1; ++i) {
    for (int j=i+1; j<heights.size(); ++j) {
      int height = std::min(heights[i], heights[j]);
      int width = j-i;
      int area = height * width;
      max_area = std::max(max_area, area);
    }
  }
  return max_area;
}

// Optimized: Time=O(n), Space=O(1)
int maxArea2(std::vector<int>& heights) {
  int left = 0;
  int right = heights.size() - 1;
  int max_area = 0;

  while (left < right) {
    int heightOfContainer = std::min(heights[left], heights[right]);
    int widthOfContainer = right - left;
    int area = heightOfContainer * widthOfContainer;

    if (area > max_area) {
      max_area = area;
    }

    if (heights[left] < heights[right]) {
      ++left;
    }
    else {
      --right;
    }
  }
  return max_area;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"heights"};
  bool test1 = GenericTestMain(args, "container_with_most_water.cc", "container_with_most_water.tsv", &maxArea1,
                         DefaultComparator{}, param_names);
  bool test2 = GenericTestMain(args, "container_with_most_water.cc", "container_with_most_water.tsv", &maxArea2,
                         DefaultComparator{}, param_names);
  return test1 && test2;
}
