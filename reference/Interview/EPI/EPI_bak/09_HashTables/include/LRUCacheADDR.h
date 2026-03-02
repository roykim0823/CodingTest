// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.
#ifndef CACHELRU_DATA_H
#define CACHELRU_DATA_H
#include <iostream>
#include <cassert>
#include <list>
#include <unordered_map>
#include <utility>

using namespace std;
typedef unsigned ADDR;

template <size_t capacity>
class LRUCache {
 public:
  bool read(ADDR address, int* data) {
    auto it = cache_.find(address);
    if (it == cache_.end()) {
      return false;
    }

	// read the cache block
    *data = it->second.second;
	// make the current block to the most recent used one
    moveToFront(address, it);
    return true;
  }

  void write(ADDR address, int data) {
    auto it = cache_.find(address);
	// if cache hits
    if (it != cache_.end()) {
		// update data
        it->second.second = data;
      	moveToFront(address, it);
    } else { // cache misses
      // Remove the least recently used.
      if (cache_.size() == capacity) {
        cache_.erase(accessHistory.back());
        accessHistory.pop_back();
      }

	  // fill the data into cache 
      accessHistory.emplace_front(address);
      cache_[address] = {accessHistory.begin(), data};
    }
  }

  bool erase(ADDR address) {
    auto it = cache_.find(address);
    if (it == cache_.end()) {
      return false;
    }

    accessHistory.erase(it->second.first);
    cache_.erase(it);
    return true;
  }

  // print access history
  void print() {
	  for(auto t : accessHistory) {
		  cout << "ADDR = " << t << ", DATA = " << cache_[t].second << endl;
	  }
	  cout << endl;
  }

private:
 	// address, pair<iterator for the ording list, data>
  	typedef unordered_map<ADDR, pair<typename list<ADDR>::iterator, int>> Table;
  	Table cache_;
  	list<ADDR> accessHistory;

  // Move the most recent accessed item to the front.
  void moveToFront(ADDR address, const typename Table::iterator& it) {
	// 1. remove the target entry in the ordering list 
    accessHistory.erase(it->second.first);
	// 2. add the removed one to the front
    accessHistory.emplace_front(address);
	// 3. update the reference
    it->second.first = accessHistory.begin();
  }
};
#endif
