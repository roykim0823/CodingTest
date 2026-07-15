#include <algorithm>
#include <stack>
#include <stdexcept>

#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"

using std::length_error;
using std::max;
using std::stack;

// MAX, Time: O(1), Space: Worst O(n), Best O(1)
class Stack {
 public:
  bool Empty() const { 
    return element_.empty(); 
  }

  int Max() const {
    if (Empty()) {
      throw std::length_error("Max(): empty stack");
    }
    return cached_max_with_cnt_.top().max;
  }

  int Pop() {
    if (Empty()) {
      throw std::length_error("Max(): empty stack");
    }
    int pop_element = element_.top();
    element_.pop();

    // update the cached_max_with_cnt
    const int current_max = cached_max_with_cnt_.top().max;
    if (pop_element == current_max) {
      int& max_freq = cached_max_with_cnt_.top().cnt;
      --max_freq;
      if (max_freq==0) {
        cached_max_with_cnt_.pop();
      } 
    }
    return pop_element;
  }

  void Push(int x) {
    element_.push(x);
    if (cached_max_with_cnt_.empty()) {
      cached_max_with_cnt_.emplace(MaxWithCount{x, 1});
    } else {
      int& current_max = cached_max_with_cnt_.top().max;
      if (x == current_max) {
        int& max_freq = cached_max_with_cnt_.top().cnt;
        ++max_freq;
      } else if (x > current_max) {
        cached_max_with_cnt_.emplace(MaxWithCount{x, 1});
      }
    }
  }

 private:
  stack<int> element_;
  struct MaxWithCount {
    int max, cnt;
  };
  stack<MaxWithCount> cached_max_with_cnt_;
};


// Test Code
struct StackOp {
  std::string op;
  int argument;
};

namespace test_framework {
template <>
struct SerializationTrait<StackOp> : UserSerTrait<StackOp, std::string, int> {};
}  // namespace test_framework

void StackTester(const std::vector<StackOp>& ops) {
  try {
    Stack s;
    for (auto& x : ops) {
      if (x.op == "Stack") {
        continue;
      } else if (x.op == "push") {
        s.Push(x.argument);
      } else if (x.op == "pop") {
        int result = s.Pop();
        if (result != x.argument) {
          throw TestFailure("Pop: expected " + std::to_string(x.argument) +
                            ", got " + std::to_string(result));
        }
      } else if (x.op == "max") {
        int result = s.Max();
        if (result != x.argument) {
          throw TestFailure("Max: expected " + std::to_string(x.argument) +
                            ", got " + std::to_string(result));
        }
      } else if (x.op == "empty") {
        int result = s.Empty();
        if (result != x.argument) {
          throw TestFailure("Empty: expected " + std::to_string(x.argument) +
                            ", got " + std::to_string(result));
        }
      } else {
        throw std::runtime_error("Unsupported stack operation: " + x.op);
      }
    }
  } catch (length_error&) {
    throw TestFailure("Unexpected length_error exception");
  }
}

// clang-format off


int main(int argc, char* argv[]) {
  std::vector<std::string> args {argv + 1, argv + argc};
  std::vector<std::string> param_names {"ops"};
  return GenericTestMain(args, "stack_with_max.cc", "stack_with_max.tsv", &StackTester,
                         DefaultComparator{}, param_names);
}
// clang-format on
