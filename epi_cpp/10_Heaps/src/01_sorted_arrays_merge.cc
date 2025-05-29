#include <algorithm>
#include <iterator>
#include <queue>
#include <vector>

#include "test_framework/generic_test.h"

using std::greater;
using std::next;
using std::priority_queue;
using std::vector;

vector<int> MergeSortedArrays(const vector<vector<int>>& sorted_arrays) {
  struct IteratorCurrentAndEnd {
    // heap order is decided by the value pointed by the current iterator
    bool operator<(const IteratorCurrentAndEnd& that) const {
      return *current > *that.current;
    }

    vector<int>::const_iterator current, end;  // it holds each sorted array's iterator
  };

  priority_queue<IteratorCurrentAndEnd> min_heap;

  // Just push the current begin and end iterator of each sorted array.
  for (const vector<int>& sorted_array : sorted_arrays) {
    if (!empty(sorted_array)) {
      min_heap.push({cbegin(sorted_array), cend(sorted_array)});
    }
  }

  vector<int> result;
  while (!empty(min_heap)) {
    auto [current, end] = min_heap.top();
    min_heap.pop();
    result.emplace_back(*current);
    if (next(current) != end) {
      min_heap.push({next(current), end});  // reinserted the array by moving the current
    }
  }
  return result;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sorted_arrays"};
  return GenericTestMain(args, "sorted_arrays_merge.cc",
                         "sorted_arrays_merge.tsv", &MergeSortedArrays,
                         DefaultComparator{}, param_names);
}
