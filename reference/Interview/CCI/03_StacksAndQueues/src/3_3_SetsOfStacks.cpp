// Q. Implement SetOFStacks with threshold value (capacity)
//	If the size of a stack exceeds the threshold value, then push data in new stack.
// FOLLOW UP: implement popAt(int index), a pop on a specific stack

#include <iostream>
#include <vector>
using namespace std;

const int STACK_SIZE = 10;

class stack{
private:
    int *buf;
    int cur;
    int capacity;

public:
    stack(int capa = STACK_SIZE){
        buf = new int[capa];
        cur = -1;
        capacity = capa;
    }
    ~stack() { delete[] buf; }
	void push(int val) 	{ buf[++cur] = val; }
    void pop()	{ if(cur>-1) --cur; }
    int top()	{ return buf[cur]; }
    bool empty(){ return cur==-1; }
    bool full()	{ return cur==capacity-1; }
};

class SetOfStacks{
private:
    vector<stack*> stacks;
    int cur;	// stack index
    int capacity;

public:
    SetOfStacks(){
        cur = 0;
        stacks.push_back(new stack());	
    }
    ~SetOfStacks(){
        stacks.clear();
    }
    void push(int val){
        if(stacks[cur]->full()) {
			++cur;
			stacks.push_back(new stack());	// add new stack
		}
        stacks[cur]->push(val);
    }

    void pop(){
        if(stacks[cur]->empty()) {
			stacks.pop_back(); // remove a stack
			if(cur>0)--cur;
		}
        stacks[cur]->pop();
    }

	// need to implement rollover
    void popAt(int idx){	
        while(stacks[idx]->empty()) --idx;
        stacks[idx]->pop();
    }
    int top(){
        if(stacks[cur]->empty()) --cur;
        return stacks[cur]->top();
    }
    bool empty(){
        if(cur==0) return stacks[0]->empty();
        else return false;
    }
};

int main(){
    SetOfStacks ss;
    for(int i=0; i<STACK_SIZE*10+1; ++i){
        ss.push(i);
    }
    while(!ss.empty()){
        cout<<ss.top()<<endl;
        ss.pop();
    }

    return 0;
}
