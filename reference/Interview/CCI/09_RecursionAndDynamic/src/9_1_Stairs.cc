// Q. A child is running up a staircase with n steps, and cap hop either 1 step,
//	  2 steps, or 3 steps at a time. Count the number of possible ways to run up the stairs

/* Checks: We can approach this problem from the top down. 
	On the very last hop, up to the nth step, the child could have done either a single, double 
	or triple step hop. That is, the last move might have been a single step hop from step n-1, 
	a double step hop from step fro n-2, or a triple step hop from n-3. The total number of ways 
	of reaching the last step is therefore the sum of the number of ways of reaching each of 
	the last three steps.

	Bottom-up:
	1: 1					-> 1
	2: 11, 2				-> 2
	3: 111, 12, 21, 3 		-> 4
	4: 1111, 112, 121, 211, 22, 13, 31	-> 7 => 7 = 4+2+1
*/

#include <iostream>
using namespace std;
#define MAX 16

// Pure Recursive: O(3^N)
long countWays_v1(int n){
	if(n<3) return n;
	if(n==3) return 4;
	
	// Top down approach: On the very last hop
	return countWays_v1(n-1) + countWays_v1(n-2) + countWays_v1(n-3);
}

long countWays_v2(int n, long map[]) {
	if(n<0) return 0;
	if(n==0) return 1;
    
	if(map[n] > -1) return map[n];

	map[n] = countWays_v2(n-1, map) +
			 countWays_v2(n-2, map) +
			 countWays_v2(n-3, map);
	return map[n];
}

long countWays_v3(int n) {
	//assert(n>0);
	if (n<3) return n;
	if (n==3) return 4;
	long ppprev=1, pprev=2, prev=4, cur;
	for(int i=4; i<=n; i++) {
		cur=ppprev+pprev+prev;
		ppprev=pprev; 	// update ppprev, pprev, prev
		pprev=prev;
		prev=cur;
	}
	return cur;
}

	
int main(){
	long map[MAX+1];
	for(int i=0; i<MAX+1; i++)
		map[i]=-1;

	for(int i=1; i<=MAX; i++)  {
		cout << i << ": ";
		cout << countWays_v1(i) << " ";
		cout << countWays_v2(i, map) << " "; 
		cout << countWays_v3(i) << " " << endl;
	}
	
    return 0;
}
