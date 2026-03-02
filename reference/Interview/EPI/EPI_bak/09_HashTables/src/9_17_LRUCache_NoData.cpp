// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#include "LRUCache.h"
#include <vector>

int main(int argc, char* argv[]) {

	vector<int> data = { 1, 2, 3, 1, 4, 5, 6, 1, 3, 3 };
	LRUCache ca(4);
	
	for(auto vec : data) {
		ca.read(vec);
		cout << "READ " << vec << endl;
		ca.print();
	}
	
  return 0;
}
