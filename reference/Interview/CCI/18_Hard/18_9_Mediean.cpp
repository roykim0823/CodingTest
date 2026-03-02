// Q. Numbers are randomly generated and passsed to a method. Write a program to find and maintain 
//    the median value as new values are generated.
//    -> Using Two Priority Queue: min and max

// This will roughly divide the elements in half
// Roughly half? 
// if maxHeap.size() > minHeap.size(), then minHeap.top() will be the median
// if maxheap.siez() == minHeap.size(), then the average of maxHeap.top() and minHeap.top() will be median
// to rebalance the heaps, it is always maxHeap with extra element when the number of elements is odd.  // maxHeap: the values less than the median
// minHeap: the values above the median
// median is the mean (average) of the two middlemost numbers if the number of values is even.

#include <iostream>
#include <algorithm>
#include <queue>
#include <cstdlib>
using namespace std;

class Median{
private:
    priority_queue<int,vector<int>,less<int> > max_heap;		// top() is maximum value
    priority_queue<int,vector<int>,greater<int> > min_heap;		// top() is minimum value

public:
    void Insert(int v);
//    int GetValue();
    double GetValue();
};

void Median::Insert(int v){
	// Insert maintains a condition that max_heap.size() >= minHeap.size()
    if(max_heap.size() == min_heap.size())	// Even numbers, it includes that case that both are empty
	{
		// becomes odd number: max_heap = min_heap+1
		if(!min_heap.empty() && min_heap.top() < v) {
        	max_heap.push(min_heap.top());  // move min_heap.top() to max_heap
			min_heap.pop();
			min_heap.push(v);  				
		} else {
			max_heap.push(v);
		}
    } else {	
		// odd number (max_heap.top() is median) becomes even number
       	if(v < max_heap.top()) {
			min_heap.push(max_heap.top()); // move the median (max_heap.top()) to min_heap
			max_heap.pop();
			max_heap.push(v);
		} else {
			min_heap.push(v);
		}
	}
}
		
double Median::GetValue(){
	if(max_heap.empty())	// also min_heap is empty()
		return 0;
    if(max_heap.size() == min_heap.size())
        return ((double)(max_heap.top()+min_heap.top())) / 2.0;
    if(max_heap.size() > min_heap.size())  // max_heap.top() is the median when the number of values is odd. 
        return max_heap.top();
}

int main(){
    srand((unsigned)time(0));
    Median md;
    vector<int> vi;
    int num = rand() % 30; //数量是30以内的随机数
    for(int i=0; i<num; ++i){
        int data = rand() % 100; //元素是100内的数
		data = i;
        vi.push_back(data);
        md.Insert(data);
    	for(int i=0; i<vi.size(); ++i)
        	cout<<vi.at(i)<<" "; //排序的序列
		cout << " -> median = " << md.GetValue() << endl;;
    }
    sort(vi.begin(), vi.end());
    for(int i=0; i<num; ++i)
        cout<<vi.at(i)<<" "; //排序的序列
    //md.Insert(3);
    //md.Insert(1);
    //md.Insert(2);
    cout<<endl<<md.GetValue()<<endl; //中位数
    return 0;
}
