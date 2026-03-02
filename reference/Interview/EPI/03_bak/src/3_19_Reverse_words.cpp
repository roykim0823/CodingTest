// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <string>

using std::cout;
using std::default_random_engine;
using std::endl;
using std::random_device;
using std::string;
using std::uniform_int_distribution;

/* The code for computing the position for each character in a string in a single pass is farily complex.
   However, a two stage iteration is easy:
   1. reverse the entire string.
   2. reverse each string
   ex) "ram is costly"
    1. "yltsoc si mar"
	2. "costly is ram"
*/

void reverse_words(string* input) {
  // Reverse the whole string first.
  reverse(input->begin(), input->end());

  size_t start = 0, end;
  while ((end = input->find(" ", start)) != string::npos) {
    // Reverse each word in the string.
    reverse(input->begin() + start, input->begin() + end);
    start = end + 1;
  }
  // Reverse the last word.
  reverse(input->begin() + start, input->end());
}

void check_answer(const string &ori, string* str) {
  reverse_words(str);
  assert(ori.compare(*str) == 0);
}

string rand_string(int len) {
  default_random_engine gen((random_device())());
  string ret;
  while (len--) {
    uniform_int_distribution<int> dis(0, 52);
    int ch = dis(gen);
    if (ch == 52) {
      ret += ' ';
    } else if (ch < 26) {
      ret += ch + 'a';
    } else {
      ret += ch - 26 + 'A';
    }
  }
  return ret;
}

int main(int argc, char *argv[]) {
  default_random_engine gen((random_device())());
  for (int times = 0; times < 1000; ++times) {
    string str;
    if (argc >= 2) {
      str += argv[1];
      for (int i = 2; i < argc; ++i) {
        str += ' ';
        str += argv[i];
      }
    } else {
      uniform_int_distribution<int> dis(0, 9999);
      str = rand_string(dis(gen));
    }
    string original_str(str);
    cout << str << endl;
    reverse_words(&str);
    cout << str << endl;
    check_answer(original_str, &str);
  }
  return 0;
}
