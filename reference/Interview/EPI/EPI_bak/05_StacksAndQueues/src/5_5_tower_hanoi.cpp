// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include <array>
#include <iostream>
#include <random>
#include <stack>

using std::array;
using std::cout;
using std::default_random_engine;
using std::endl;
using std::random_device;
using std::stack;
using std::uniform_int_distribution;

// transfer (move) a disk from "from" to "to" using "use" as a buffer
void transfer(int n, array<stack<int>, 3>& pegs, int from, int to, int use);

void move_tower_hanoi(int n) {
  array<stack<int>, 3> pegs;
  
  // Initialize pegs.
  for (int i = n; i >= 1; --i) {
    pegs[0].push(i);
  }

  // start!
  transfer(n, pegs, 0, 2, 1);
}

void transfer(int n, array<stack<int>, 3>& pegs, int from, int to, int use) {
  if (n > 0) {
	// Move top n-1 disks from 0 to 1 using 2
    transfer(n - 1, pegs, from, use, to);
	
	// Move top from 0 to 2
	cout << "Move disk " << pegs[from].top() <<" from peg " << from << " to peg " << to << endl;
    pegs[to].push(pegs[from].top());
    pegs[from].pop();
    
	// Move top n-1 disks from 2 to 1 using 0 as a buffer
	transfer(n - 1, pegs, use, to, from);
  }
}

int main(int argc, char* argv[]) {
  int n;
  if (argc == 2) {
    n = atoi(argv[1]);
  } else {
    default_random_engine gen((random_device())());
    uniform_int_distribution<int> dis(1, 10);
    n = dis(gen);
  }
  cout << "n = " << n << endl;
  move_tower_hanoi(n);
  return 0;
}
