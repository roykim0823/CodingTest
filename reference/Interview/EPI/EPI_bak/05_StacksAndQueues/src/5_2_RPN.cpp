// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <cassert>
#include <sstream>
#include <stack>
#include <string>

using std::stack;
using std::string;
using std::stringstream;

// evaluate an RPN expression iterates through the string from left to right
int eval(const string& s) {
  stack<int> eval_stack;
  stringstream ss(s);
  string symbol;

  while (getline(ss, symbol, ',')) {
    if (symbol == "+" || symbol == "-" || symbol == "*" || symbol == "/") {
      int y = eval_stack.top();
      eval_stack.pop();
      int x = eval_stack.top();
      eval_stack.pop();

	  // perform calculation and push back the result to the stack
      switch (symbol.front()) {
        case '+':
          eval_stack.emplace(x + y);
          break;
        case '-':
          eval_stack.emplace(x - y);
          break;
        case '*':
          eval_stack.emplace(x * y);
          break;
        case '/':
          eval_stack.emplace(x / y);
          break;
      }
    } else {  // number.
      eval_stack.emplace(stoi(symbol));
    }
  }
  return eval_stack.top();
}

int main(int argc, char* argv[]) {
  assert(0 == eval("2,-10,/"));
  assert(-5 == eval("-10,2,/"));
  assert(5 == eval("-10,-2,/"));
  assert(-5 == eval("5,10,-"));
  assert(6 == eval("-10,-16,-"));
  assert(12 == eval("10,2,+"));
  assert(15 == eval("1,2,+,3,4,*,+"));
  assert(42 == eval("1,2,3,4,5,+,*,+,+,3,4,*,+"));
  return 0;
}
