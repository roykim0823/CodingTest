#include <numeric>
#include <string>
#include "test_framework/generic_test.h"

using std::accumulate;
using std::string;

int SSDecodeColID(const string& col) {
  return accumulate(begin(col), end(col), 0, [](int result, char c) {
    return result * 26 + c - 'A' + 1;
  });
}

int ssDecodeColID(const string& col) {
  int ret = 0;
  for (const char& c : col) {
    ret = ret * 26 + c - 'A' + 1;
  }
  return ret;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"col"};
  GenericTestMain(args, "spreadsheet_encoding.cc",
                         "spreadsheet_encoding.tsv", &SSDecodeColID,
                         DefaultComparator{}, param_names);
  GenericTestMain(args, "spreadsheet_encoding.cc",
                         "spreadsheet_encoding.tsv", &ssDecodeColID,
                         DefaultComparator{}, param_names);
}
