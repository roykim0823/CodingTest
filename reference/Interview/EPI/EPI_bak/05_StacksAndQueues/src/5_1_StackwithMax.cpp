// Q. Design a stack with a max function with O(1) which returns the max element. 

/* 
Solutions:
	A. Add max element in every stack element
	B. Add auxiliary stack (max, count)
*/

#include <iostream>
#include <stack>
#include <random>
#include <stdexcept> 	// for length error

using namespace std;

// Implement stack with min or max in O(1) time
// Naive: add max for every item
template <typename T>
class StackMaxNaive {
private:
	stack<pair<T, T> > s;	// both T should be int, value and max
	
public:
	const bool empty(void) const { return s.empty(); }
		
	const T& max(void) const 
	{
  		if(empty() == false) {
   			return s.top().second;
   		}
  		throw length_error("empty stack");
	}

	T pop(void) {
		if(empty() == false) {
			T ret = s.top().first;
			s.pop();
			return ret;
		}
		throw length_error("empty stack");
	}


	const T& top(void) const 
	{
  		if(empty() == false) {
   			return s.top().first;
   		}
  		throw length_error("empty stack");
	}
	void push(const T &x) {	// insert item and max or min or etc
		s.emplace(x, std::max(x, empty() ? x : s.top().second));
	}
};

// Better: using auxiliary stack (max, times)
template <typename T>
class StackMax {
private:
	stack<T> s;	// main stack
	stack<pair<T, int> > aux;	// auxiliary stack (max value, cnt)
	
public:
	const bool empty(void) const { return s.empty(); }
		
	const T &max(void) const 
	{
  		if(empty() == false) {
   			return aux.top().first;
   		}
  		throw length_error("empty stack");
	}
	
	T pop(void) {
		if(empty() == false) {
			T ret = s.top();
			s.pop();
			if(ret == aux.top().first) {
				--aux.top().second; 	// Decrease only count value
				if(aux.top().second == 0) {
					aux.pop();
				}
			}
			return ret;
		}
		throw length_error("empty stack");
	}

	const T& top(void) const 
	{
  		if(empty() == false) {
   			return s.top();
   		}
  		throw length_error("empty stack");
	}
	void push(const T &x) {	// insert item and max or min or etc
		s.emplace(x);
		if(aux.empty() == false) {
			if(x == aux.top().first) {
				++aux.top().second;
			} else if (x > aux.top().first) {
				aux.emplace(x,1);
			}
		} else {
			aux.emplace(x, 1);
		}
	}
};

int main()
{
	StackMaxNaive<int> s1;  
	StackMax<int> s2;
	uniform_int_distribution<int> u(-10, 10);
	default_random_engine e;
	for(int i=0; i<20; ++i)
	{
		int data = u(e);
		s1.push(data);
		s2.push(data);
		cout << data << " ";
	}
	cout<<endl;
	for(int i=0; i<20; ++i) {
		cout<<"max= " << s1.max() << " " << s2.max() << endl;
		cout<<"top= " << s1.top() << " " << s2.top() << endl;
		cout<<"pop!" << endl;
		s1.pop(); s2.pop();
	}
	return 0;
}
