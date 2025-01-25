#include <numeric>
#include <string>
#include "test_framework/generic_test.h"

using std::accumulate;
using std::string;

string ConstructFromBase(int, int);

string ConvertBase(const string& num_as_string, int b1, int b2) {
  bool is_negative = num_as_string.front() == '-';
  int num_as_int =
      accumulate(begin(num_as_string) + is_negative, end(num_as_string), 0,
                 [b1](int x, char c) {
                   return x * b1 + (isdigit(c) ? c - '0' : c - 'A' + 10);
                 });
  return (is_negative ? "-" : "") +
         (num_as_int == 0 ? "0" : ConstructFromBase(num_as_int, b2));
}

string ConstructFromBase(int num_as_int, int base) {
  return num_as_int == 0
             ? ""
             : ConstructFromBase(num_as_int / base, base) +
                   (char)(num_as_int % base >= 10 ? 'A' + num_as_int % base - 10
                                                  : '0' + num_as_int % base);
}

// original version
// number string base b1 -> base b2 (2<=base<=16)
string convertBase(const string &s, const int b1, const int b2) {
  // Check if the number is negative
  bool neg = false;  // default
  if(s[0] == '-')
    neg = true;

  int x=0;
  size_t i= (neg == true? 1:0);  // calculate start index of the number
  for (; i<s.size(); ++i) {
    x = x * b1;
    if(isdigit(s[i]))
      x += s[i] - '0';
    else
      x += s[i] - 'A' + 10;
  }

  string ans;
  while(x) {  // digits on base b2 in reverse order
    int r = x % b2;
    ans.push_back(r>=10 ? 'A' + r-10 : '0' + r);
    x /= b2;
  }

  if(ans.empty())
    ans.push_back('0');
  if(neg)
    ans.push_back('-');

  reverse(ans.begin(), ans.end());
  return ans;
}


int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"num_as_string", "b1", "b2"};
  GenericTestMain(args, "convert_base.cc", "convert_base.tsv",
                  &ConvertBase, DefaultComparator{}, param_names);
  GenericTestMain(args, "convert_base.cc", "convert_base.tsv",
                  &convertBase, DefaultComparator{}, param_names);
}
