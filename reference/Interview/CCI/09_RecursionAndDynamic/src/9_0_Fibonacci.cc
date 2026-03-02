/* A good hint that a problem is recursive is that it can be built of sub-problems.
   e.g., Design an algorithm to compute the nth...
         Write code to list the first n...
		 Implement a method to compute all...

How to approach:
	Recursive solutions, by definition, are built off solutions to sub-problems. Many times,
	this will mean simply to compute f(n) by adding something, removing something, or otherwise
	changing the solution for f(n-1). In other cases, you might have to do something more complicated.

	You should consider both bottom-up and top-down recursive solutions. The base case and build
	approach works quite well for recursive problems.

	1. Bottom-up Recursion: 1, 2, 3, .... find the pattern
		Bottom-up recursion is often the most intuitive. We start with knowing how to solve the problem
		for a simple case, like a list with only one element, and figure out how to solve the problem
		for two elements, then for three elements, and so on. The key here is to think about how you
		can build the solution for one case off of the previous case.
	
	2. Top-Down Recursion
		Think about how we can divide the problem for case N into sub-problems. Be careful of overlap
		between the cases.
	
	3. Dynamic Programming: simiar to recursive method but caches the intermediate results for reuse.

	4. Iterative Solution:
		Revursive algorithm can be space inefficient. Each recursive call adds a new layer to the stack,
		which means that if you algorithm has O(n) recursive calls, then it uses O(n) memory space.
		Iterative can save the space requirement, but it can be more complex.
*/


#include <iostream>
using namespace std;
typedef unsigned long long ll;

// Pure Recursive: O(2^n)
ll fib_v1(ll n){
	//if(n <= 0) return 0;			// Error
	//if(n == 1) return 1;      	// First Base
	//if(n == 2) return 2; 			// Second Base
	
	if(n <= 2) return n;          	// Combine all base cases
    else return fib_v1(n-1) + fib_v1(n-2);
}

// Dynamic Programming
ll fib_v2(ll n){
	static ll fib_result[100];

	if(n<=2) return n;
	if(fib_result[n] != 0) return fib_result[n]; // return cached result
    fib_result[n] = fib_v2(n-1) + fib_v2(n-2);
    return fib_result[n];
}

// Simple iterative version
ll fib_iter(ll n) {
	if(n<=2) return n;
	// 1, 2, 3, 5 : cur=prev+pprev;
	int pprev=1, prev=2, cur;
	for(int i=3; i<=n; i++) {
		cur=pprev+prev;
		pprev=prev;
		prev=cur;
	}
	return cur;
}

int main(){
    for(int i=1; i<35; ++i)
        cout<<fib_v1(i)<<" ";
    cout<<endl;
    for(int i=1; i<35; ++i)
        cout<<fib_v2(i)<<" ";
    cout<<endl;
    for(int i=1; i<35; ++i)
        cout<<fib_iter(i)<<" ";
    cout<<endl;
    return 0;
}
