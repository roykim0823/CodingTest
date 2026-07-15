#include <algorithm>
#include <array>
#include <vector>

#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"

using std::swap;
using std::vector;

enum class Color { kRed, kWhite, kBlue };

// O(n) time, O(1) space, two passes (smaller forward, larger backward)
void DutchFlagPartition_two_pass(int pivot_index, vector<Color>* A_ptr) {
  vector<Color>& A = *A_ptr;
  Color pivot = A[pivot_index];

  // First pass: group elements smaller than pivot.
  int smaller = 0;
  for (int i = 0; i < A.size(); ++i) {
    if (A[i] < pivot) {
      swap(A[i], A[smaller++]);
    }
  }

  // Second pass: group elements larger than pivot.
  int larger = A.size() - 1;
  for (int i = A.size() - 1; i >= 0 && A[i] >= pivot; --i) {
    if (A[i] > pivot) {
      swap(A[i], A[larger--]);
    }
  }
}

// O(n) time, O(1) space, single pass with four zones
void DutchFlagPartition_single_pass(int pivot_index, vector<Color>* A_ptr) {
  vector<Color>& A = *A_ptr;
  Color pivot = A[pivot_index];
  /**
   * Keep the following invariants during partitioning:
   * bottom group: A[0, smaller - 1].
   * middle group: A[smaller, equal - 1].
   * unclassified group: A[equal, larger - 1].
   * top group: A[larger, size(A) - 1].
   */
  int smaller = 0, equal = 0, larger = size(A);
  // Keep iterating as long as there is an unclassified element.
  while (equal < larger) {
    // A[equal] is the incoming unclassified element.
    if (A[equal] < pivot) {
      swap(A[smaller++], A[equal++]);
    } else if (A[equal] == pivot) {
      ++equal;
    } else {  // A[equal] > pivot.
      swap(A[equal], A[--larger]);
    }
  }
}

template <void (*Partition)(int, vector<Color>*)>
void DutchFlagPartitionWrapper(TimedExecutor& executor, const vector<int>& A,
                               int pivot_idx) {
  vector<Color> colors;
  colors.resize(A.size());
  std::array<int, 3> count = {0, 0, 0};
  for (size_t i = 0; i < A.size(); i++) {
    count[A[i]]++;
    colors[i] = static_cast<Color>(A[i]);
  }
  Color pivot = colors[pivot_idx];

  executor.Run([&] { Partition(pivot_idx, &colors); });

  int i = 0;
  while (i < colors.size() && colors[i] < pivot) {
    count[static_cast<int>(colors[i])]--;
    ++i;
  }

  while (i < colors.size() && colors[i] == pivot) {
    count[static_cast<int>(colors[i])]--;
    ++i;
  }

  while (i < colors.size() && colors[i] > pivot) {
    count[static_cast<int>(colors[i])]--;
    ++i;
  }

  if (i != colors.size()) {
    throw TestFailure("Not partitioned after " + std::to_string(i) +
                      "th element");
  } else if (count != std::array<int, 3>{0, 0, 0}) {
    throw TestFailure("Some elements are missing from original array");
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "A", "pivot_idx"};
  GenericTestMain(args, "dutch_national_flag.cc", "dutch_national_flag.tsv",
                  &DutchFlagPartitionWrapper<DutchFlagPartition_two_pass>,
                  DefaultComparator{}, param_names);
  return GenericTestMain(
      args, "dutch_national_flag.cc", "dutch_national_flag.tsv",
      &DutchFlagPartitionWrapper<DutchFlagPartition_single_pass>,
      DefaultComparator{}, param_names);
}
