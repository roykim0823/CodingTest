/*Square root fo an integer
- Given Given an integer x, find square root of it. If x is not a perfect square, then return floor(¡îx).

A Simple Solution to find floor of square root is to try all numbers starting from 1. For every tried number i, if i*i is smaller than x, then increment i. We stop when i*i becomes more than or equal to x. Below is the implementation of above idea.
 */

// A C++ program to find floor(sqrt(x)
#include<bits/stdc++.h>
using namespace std;
 
// Returns floor of square root of x
// -> O(sqrt(x))
int floorSqrt(int x)
{
    // Base cases
    if (x == 0 || x == 1)
    return x;
 
    // Staring from 1, try all numbers until
    // i*i is greater than or equal to x.
    int i = 1, result = 1;
    while (result <= x)
    {
      i++;
      result = i * i;
    }
    return i - 1;
}
 
/*
A Better Solution to do Binary Search.
Let  's' be the answer.  We know that 0 <=  s <= x.

Consider any random number r. 

    If r*r <= x, s >= r

    If r*r > x, s < r. 

Algorithm:
1) Start with 'start' = 0, end = 'x',
2) Do following while 'start' is smaller than or equal to 'end'.
      a) Compute 'mid' as (start + end)/2
      b) compare mid*mid with x.
      c) If x is equal to mid*mid, return mid.
      d) If x is greater, do binary search between mid+1 and end. In this case, we also update ans (Note that we need floor).
      e) If x is smaller, do binary search between start and mid-1

Below is the implementation of above idea.
*/

// Returns floor of square root of x         
// -> O(logx)
// Double version: EPI-8.9 (p.263)
// implement sqrt(x)
int floorSqrt(int x) 
{    
    // Base cases
    if (x == 0 || x == 1) 
       return x;
 
    // Do Binary Search for floor(sqrt(x))
    int start = 1, end = x, ans;   
    while (start <= end) 
    {        
        int mid = (start + end) / 2;
 
        // If x is a perfect square
        if (mid*mid == x)
            return mid;
 
        // Since we need floor, we update answer when mid*mid is 
        // smaller than x, and move closer to sqrt(x)
        if (mid*mid < x) 
        {
            start = mid + 1;
            ans = mid;
        } 
        else // If mid*mid is greater than x
            end = mid - 1;        
    }
    return ans;
}


void main() 
{ 
	float a,b,c; 
	printf("\n enther the number square root who wanna found\n"); 
	scanf("%f",&a); 
	int i,j; 
	b=a; 
	for(i=1;i<10;i++) 
	{ 
	b=0.5*(b+a/b); 
	} 
	printf("\nsqrt of %f is %f\n\n",i,a,b); 
}
