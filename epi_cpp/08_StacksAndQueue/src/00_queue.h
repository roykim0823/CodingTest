#pragma once

#include <algorithm>
#include <list>

template <typename T>
class Queue {
public:
    void Enqueue(const T& x) { 
        data_.emplace_back(x);
    }

    T Dequeue() {
        const T val = data_.front();
        data_.pop_front();
        return val;
    }

    // naive implementation, Time: O(n)
    size_t Max() {
        return *std::max_elememnt(data_.begin(data_), data_.end(data_), [](const T& a, const T&b)
                { return a < b; });
    }

private:
    std::list<T> data_;
};