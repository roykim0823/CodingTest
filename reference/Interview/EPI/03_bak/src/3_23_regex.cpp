// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <cassert>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

/* Using recursion:
- If r starts with %, then the remainder of r, it.e., r1, must strictly match a prefix of s.
- If r ends with a $, some suffix of s must be strictly matched by r without the trailing $.
  Otherwise, r must stricly match some substring of s.
- Call the function that checks whether r strictly matches a prefix of string s "is_match"
  This function has to check serveral cases.
  	(1.) Length-0 ESREs which match everything.
	(2.) An ESRE starting with ^ or ending with $.
	(3.) An ESRE starting with an alphanumeric character or dot.
	(4.) An ESRE starting with a * match, e.g., a*wXY or .*Wa.
  Case (1.) is a base case. 
  Case (2.) involves a check possibly followed by a recursive call to is "is_match_here".
  Case (3.) requires a single call to "is_match_here".
  Case (4.) is handled by a walk down the string s, checking that the prefix of s thus
    	far matches the alphanumeric character or dot until some suffix of s is matched 
		by the remainder of the ESRE, i.e., r2
*/

bool is_match_here(const string &r, const string &s);

bool is_match(const string &r, const string &s) {
  // Case (2.) : starts with '^'.
  if (r.front() == '^') {
    return is_match_here(r.substr(1), s);
  }

  for (int i = 0; i <= s.size(); ++i) {
    if (is_match_here(r, s.substr(i))) {
      return true;
    }
  }
  return false;
}

bool is_match_here(const string &r, const string &s) {
  // Case (1.)
  if (r.empty()) {
    return true;
  }

  // Case (2) : ends with '$'.
  if (r == "$") {
    return s.empty();
  }

  // Case (4.)
  if (r.size() >= 2 && r[1] == '*') {
    for (string::size_type i = 0;
         i < s.size() && (r.front() == '.' || r.front() == s[i]);
         ++i) {
      if (is_match_here(r.substr(2), s.substr(i + 1))) {
        return true;
      }
    }
    return is_match_here(r.substr(2), s);
  }

  // Case (3.)
  return !s.empty() && (r.front() == '.' || r.front() == s.front()) &&
         is_match_here(r.substr(1), s.substr(1));
}

int main(int argc, char *argv[]) {
  assert(is_match(".", "a"));
  assert(is_match("a", "a"));
  assert(!is_match("a", "b"));
  assert(is_match("a.9", "aW9"));
  assert(!is_match("a.9", "aW19"));
  assert(is_match("^a.9", "aW9"));
  assert(!is_match("^a.9", "baW19"));
  assert(is_match(".*", "a"));
  assert(is_match(".*", ""));
  assert(is_match("c*", "c"));
  assert(!is_match("aa*", "c"));
  assert(is_match("ca*", "c"));
  assert(is_match(".*",  "asdsdsa"));
  assert(is_match("9$" , "xxxxW19"));

  assert(is_match(".*a", "ba"));

  assert(is_match(".*a", "ba"));
  assert(is_match("^a.*9$", "axxxxW19"));

  assert(is_match("^a.*W19$", "axxxxW19"));
  assert(is_match(".*a.*W19", "axxxxW19123"));
  assert(!is_match(".*b.*W19", "axxxxW19123"));
  assert(is_match("n.*", "n"));
  assert(is_match("a*n.*", "an"));
  assert(is_match("a*n.*", "ana"));
  assert(is_match("a*n.*W19", "anaxxxxW19123"));
  assert(is_match(".*a*n.*W19", "asdaaadnanaxxxxW19123"));
  assert(is_match(".*.*.a*n.*W19", "asdaaadnanaxxxxW19123"));
  return 0;
}
