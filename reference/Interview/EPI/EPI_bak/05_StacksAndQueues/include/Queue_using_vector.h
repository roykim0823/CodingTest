// Copyright (c) 2013 Elements of Programming Interviews. All rights reserved.

#ifndef SOLUTIONS_QUEUE_USING_VECTOR_H_
#define SOLUTIONS_QUEUE_USING_VECTOR_H_

#include <algorithm>
#include <vector>
#include <queue>
#include <stdexcept>

using std::vector;
using std::length_error;

// @include
template <typename T>
class Queue {
private:
	size_t head, tail, count;
	vector<T> data;
public:
	Queue(const size_t &cap = 8) 
	: head(0), tail(0), count(0), data({cap}) {}

  	void enqueue(const T& x) {
		// Dynamically resize due to data.size() limit
		if(cout == data.size()) {
			data.resize(data.size()<<1);
		}
		// Perform enqueu
		data[tail] = x;
		tail = (tail + 1) % data.size();
		++count;
	}

  	T dequeue(void) {
		if(count) {
			--count;
			T ret = data[head];
			head = (head + 1) % data.size();
			return ret;
		}
		throw length_error("empty queue");
	}

	const size_t &size(void) const {
		return count;
	}
};
// @exclude
#endif  // SOLUTIONS_QUEUE_WITH_MAX_USING_DEQUE_H_
