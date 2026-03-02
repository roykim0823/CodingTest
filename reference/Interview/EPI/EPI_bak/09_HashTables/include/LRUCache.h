// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#ifndef LRUCACHE_H_
#define LRUCACHE_H_

#include <iostream>
#include <cassert>
#include <list>
#include <unordered_map>
#include <utility>

using namespace std;

/* two data structure to implement LRU Cache
   1. Queue implemented a double linked list.
      -> Most recently used will be near front, and Least recently used will be near rear
   2. A Hash with cache index as key and the address of the corresponding queue node as value
*/
class LRUCache {
	// store keys of cache
	// the key can be considered an address, so the real data is omitted.
	list<int> dq;

	// store reference of key in cache
	unordered_map<int, list<int>::iterator> ma;
	int capacity;

public:
	LRUCache(int n=4) : capacity(n) {}

	// read the cache block by key x
	int read(int x) {
		// not present in cache
		if(ma.find(x) == ma.end()) {
			// if cache is full
			if(dq.size() == capacity) {
				// delete LRU element
				int last = dq.back();
				dq.pop_back();
				ma.erase(last);
			}
		} else {	// present in cache
			// present in cache
			dq.erase(ma[x]);	// relocate it to the front
		}

		// update the reference
		dq.push_front(x);
		ma[x] = dq.begin();
	}

	void print() {
		for(auto it = dq.begin(); it !=dq.end(); ++it) {
			cout << (*it) << " ";
		}
		cout << endl;
	}
};

#endif
