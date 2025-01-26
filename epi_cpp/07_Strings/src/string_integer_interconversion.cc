#include <numeric>
#include <string>
#include <stdexcept>  // for throw invalid_argument

#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"


using std::accumulate;
using std::string;


// Integer to String
// 1. for positive integer x:
//   - divide x by 10 iteratively, and record the remainder till we get to 0
//   - It yields the result from the LSB, and needs to be reverse!
// 2. for negative integer x
//   - calculate -x (positive) and add '-'
string IntToString(int x) {
  bool is_negative = false;
  if (x < 0) {
    is_negative = true;
  }

  string s;
  do {
    // divide x by 10 iteratively, and record the remainder till x becomes 0
    s += '0' + abs(x % 10);
    x /= 10;
  } while (x);

  s += is_negative ? "-" : "";  // Adds the negative sign back if is_negative.
  return {rbegin(s), rend(s)};  // reverse the string since the result yields from the LSB
}

int StringToInt(const string& s) {
  return (s[0] == '-' ? -1 : 1) * accumulate(begin(s) + (s[0] == '-'), end(s),
                                             0, [](int running_sum, char c) {
                                               return running_sum * 10 + c -
                                                      '0';
                                             });
}

// original version of StringToInt
// We define the valid string for this function as those matching regexp
// -?[0-9]+
int StringToInt_ORI(const string &s) {
  // "-" starts as a valid integer, but has no digits -> error
  if(s == "-") {
    throw std::invalid_argument("illegal input");
  }

  bool is_negative = (s[0] == '-');
  int x=0;

  for(size_t i=is_negative; i<s.size(); ++i) {  // is_negative to skip the '-' character
    if(isdigit(s[i])) {
      x = x * 10 + s[i] - '0';
    } else {  // without else invalid argument is ignored
      throw std::invalid_argument("illegal input");
    }
  }
  return is_negative ? -x: x;
}

void Wrapper(int x, const string& s) {
  if (IntToString(x) != s) {
    throw TestFailure("Int to string conversion failed");
  }

  if (StringToInt(s) != x) {
    throw TestFailure("String to int conversion failed");
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "s"};
  return GenericTestMain(args, "string_integer_interconversion.cc",
                         "string_integer_interconversion.tsv", &Wrapper,
                         DefaultComparator{}, param_names);
}
