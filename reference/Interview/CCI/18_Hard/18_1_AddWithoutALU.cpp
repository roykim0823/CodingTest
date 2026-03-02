// Write a function that adds two numbers without + or any arithmetic operators
// a + b -> sum(a^b), carry(a&b)<<1

#include <iostream>
using namespace std;

// Recursive
int Add(int a, int b){
    if(b == 0) return a;
    int sum = a ^ b; 			// add without carrying
    int carry = (a & b) << 1; 	// carry, but don't add
    return Add(sum, carry); 	// recurse
}

// Iterative
int Add2(int a, int b){
    while(b != 0){
        int sum = a ^ b;
        int carry = (a & b) << 1;
        a = sum;
        b = carry;
    }
    return a;
}
int main(){
    int a = 70, b = -134310;
    cout<<Add(a, b)<<endl;
    cout<<Add2(a, b)<<endl;
    return 0;
}
