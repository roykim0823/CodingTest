// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <cassert>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;

string decoding(const string &s) {
  int count = 0;
  string ret;
  for (const char &c : s) {
    if (isdigit(c)) {
      count = count * 10 + c - '0';
    } else {  // isalpha.
      ret.append(count, c);
      count = 0;
    }
  }
  return ret;
}

string encoding(const string &s) {
  int count = 1;
  stringstream ss;
  for (int i = 1; i < s.size(); ++i) {
    if (s[i] == s[i - 1]) {
      ++count;
    } else {
      ss << count << s[i - 1];	// insert count and a character
      count = 1;               	// update the last character
    }
  }
  ss << count << s.back();
  return ss.str();
}

// auxiliary function
string numToString(int num) {
	string num_str;
	char digit; 
	int num_digit;

	while(true) {
		// each digit at a time
		num_digit = num%10;
		switch(num_digit) {
			case 0: digit = '0'; break;
			case 1: digit = '1'; break;
			case 2: digit = '2'; break;
			case 3: digit = '3'; break;
			case 4: digit = '4'; break;
			case 5: digit = '5'; break;
			case 6: digit = '6'; break;
			case 7: digit = '7'; break;
			case 8: digit = '8'; break;
			case 9: digit = '9'; break;
			default:
				cerr << "Error" << endl;
				break;
		}
		num -= num_digit;
		num_str = digit+num_str;
		if(num==0)
			break;
		// remove each digit value;
		num /= 10;
	}
	return num_str;
}




int main(int argc, char *argv[]) {
  if (argc == 3) {
    cout << encoding(argv[1]) << ' ' << decoding(argv[2]) << endl;
  }
  assert(string("4a1b3c2a") == encoding("aaaabcccaa"));
  assert(string("eeeffffee") == decoding("3e4f2e"));
  assert(string("aaaaaaaaaaffffee") == decoding("10a4f2e"));
  return 0;
}
