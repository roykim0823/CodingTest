#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <random>
#include <stack>
#include <queue>

// sort stack with minimum storage
using namespace std;

void insert(stack<int>& S, int e);

// 1. Use Recursion O(n) storage
void sort(stack<int>& S) {
  if (!S.empty()) {
    int e = S.top();
    S.pop();
    sort(S);
    insert(S, e);
  }
}

void insert(stack<int>& S, int e) {
  if (S.empty() || S.top() <= e) {
    S.push(e);
  } else {
    int f = S.top();
    S.pop();
    insert(S, e);
    S.push(f);
  }
}

// 2. Use another stack, O(n^2) time, O(n) space
stack<int> Ssort(stack<int> s){
    stack<int> t;
    while(!s.empty()){
        int data = s.top(); // step 1
        s.pop();
        // step 2: t.pop() and push it in s until t.top()<= data
        while(!t.empty() && t.top()>data){
            s.push(t.top());
            t.pop();
        }
        t.push(data);
    }
    return t;
}

// 3. Simple Using Priority Queue
void Qsort(stack<int> &s){
    priority_queue< int,vector<int>,greater<int> > q;
    while(!s.empty()){
        q.push(s.top());
        s.pop();
    }
    while(!q.empty()){
        s.push(q.top());
        q.pop();
    }
}



int main(int argc, char* argv[]) {
  default_random_engine gen((random_device())());
  for (int times = 0; times < 1; ++times) {
    int n;
    if (argc == 2) {
      n = atoi(argv[1]);
    } else {
      uniform_int_distribution<int> dis(1, 10);
      n = dis(gen);
    }
    stack<int> S, S2;
    uniform_int_distribution<int> dis(0, 100);
    for (int i = 0; i < n; ++i) {
      S.push(dis(gen));
      S2.push(dis(gen));
    }
    sort(S);
	Qsort(S2);
    int pre = numeric_limits<int>::max();
    while (!S.empty()) {
      assert(pre >= S.top());
      cout << S.top() << endl;
      pre = S.top();
      S.pop();
    }
	cout << "S2..." << endl;
    pre = numeric_limits<int>::max();
    while (!S2.empty()) {
      assert(pre >= S2.top());
      cout << S2.top() << endl;
      pre = S2.top();
      S2.pop();
    }
  }
  return 0;
}
