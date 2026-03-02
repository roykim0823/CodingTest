// Q. Implement MyQueue class which implements a queue using two stacks
// Q+. MyStack with two queue

/*
Solutions: 
	1. Use the second stack to reverse LIFO to FIFO on pop() or top()
	2. Use the lazy approach (reverse back on push())
			
*/

#include <iostream>
#include <stack>
#include <queue>
#include <stdexcept>
using namespace std; 
template <typename T>
class MyStack{
private:
    queue<T> qin, qout;
public:
    MyStack(){
        
    }
    ~MyStack(){
        
    }
    void push(T val){
        qin.push(val);
    }

    void pop(){
        if(!qin.empty())
            move(qin, qout);
        else
            move(qout, qin);
    }

    T&  top(){
        if(!qin.empty())
            return qin.back();
        else
            return qout.back();
    }

    bool empty(){
        return qin.empty() && qout.empty();
    }
    int size(){
        return qin.size() + qout.size();
    }

	// for stack pop(): 
	// keep popping  an element in the src ,then store it in dst queue
	// except for the last element (which is the top of stack)
    void move(queue<T> &src, queue<T> &dst){ 
		while(!src.empty()){
            T val = src.front();
            src.pop();
            if(src.empty()) break;
            dst.push(val);
        }
    }
};

// Implement Queue using Two Stacks
template <typename T>
class MyQueue{
private:
    stack<T> stackNewest, stackOldest;	// stackOldest is FIFO order
public:
    MyQueue(){}
    ~MyQueue(){}

private:
	// Make elements in FIFO order in stackOldest
	// Or LIFO order in stackNewest
	void shiftStack(stack<T> &src, stack<T> &dst) {
		if (dst.empty()) {	// Move the stack only if the dst is empty (lazy approach)
			while(!src.empty()) {
				dst.push(src.top());
				src.pop();
			}
		}
	}

public:
	void enqueue(const T x) {
		stackNewest.push(x);
	}

	void dequeue(void) {
		shiftStack(stackNewest, stackOldest); 	// Make stackOldest in FIFO
		if(stackOldest.empty() == false) {
			//T ret = stackOldest.top();
			stackOldest.pop();
			//return ret;
		}
		else
			throw length_error("empty queue");
	}

	T&  front(void) {
		shiftStack(stackNewest, stackOldest);	// Make stackOldest in FIFO
		if(stackOldest.empty() == false) {
			T& ret = stackOldest.top();
			return ret;
		}
	}
		
	T& back(void) {
		if(stackNewest.empty() == false) 
			return stackNewest.top();
		else {
			if(stackOldest.empty() == false) {
				shiftStack(stackOldest, stackNewest);
				return stackNewest.top();
			}
		} 
	}

	int size(void) {
		return stackNewest.size()+stackOldest.size();
	}

	bool empty(void) {
		/*
		if(this->size() == 0)
			return true;
		else 
			return false;
		*/
		return stackNewest.empty() && stackOldest.empty();
	}


};

int main(){
    // Queue
    MyQueue<int> q;
    for(int i=0; i<10; ++i){
        q.enqueue(i);
    }

	cout<<"My Queue"<<endl;
    cout<<q.front()<<" "<<q.back()<<endl;
    cout<<"Dequeue!"<<endl;
    q.dequeue();
    cout<<q.front()<<" "<<q.back()<<endl;
    cout<<"Eequeue 10"<<endl;
    q.enqueue(10);
    cout<<q.front()<<" "<<q.back()<<endl;
    
	cout<<q.size()<<" "<<q.empty()<<endl;
    
	// Stack
	MyStack<int> st;
    for(int i=0; i<10; ++i){
        st.push(i);
    }
	cout<<"My Stack"<<endl;
    cout<<st.top()<<endl;
    st.pop();
    cout<<st.top()<<endl;
    st.push(100);
    cout<<st.top()<<endl;
    for(int i=0; i<2; ++i){
        st.pop();
        cout<<st.top()<<endl;
    }
    return 0;
}
