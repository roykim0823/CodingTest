// Q. Write method to implement the multiply, subtract, and divide operations for integers.
//    Use only the add operator

/* Solutions:
1. Subtraction:
	a + b = a + (-b) = a + (~b+1)

2. multiply
	a * b = a + a + ... (b times addition)

3. division
    keep (a-b) untill a-b>=0
*/

#include <iostream>
using namespace std;

const int INF = ~(1<<31);

void swap(int &a, int &b){
    a = a^b;
    b = a^b;
    a = a^b;
}

int flipSign(int a) 
{
	// Flip the bits and +1
	return (~a) + 1;
}

int abs(int a){
    if(a < 0) a = flipSign(a);
    return a;
}

bool opsign(int a, int b){
    return (a>0 && b<0) || (a<0 && b>0);
}

int times(int a, int b){
    int aa = abs(a), bb = abs(b);
    //if(aa < bb) swap(aa, bb);
	if(aa < bb)	// for less loop iteration 
		return times(bb, aa);

    int sum = 0;
    for(int i=0; i<bb; ++i)
		sum+=aa;
	
    if(opsign(a, b)) sum = flipSign(sum);
    return sum;
}

// a - b = a + (-b)
int minuss(int a, int b)
{
    return a + flipSign(b);
}

// integer division
int divide(int a, int b){ 	
    if(b == 0) return INF;
    int aa = abs(a), bb = abs(b);
    int res = 0;
    for(; (aa -= bb)>=0; ++res);
    if(opsign(a, b)) res = flipSign(res);
    return res;
}
int main(){
    int a[] = {
        8, 0, -8, -5, 9
    };
    int b[] = {
        3, 5, 3, 0, -3
    };
    for(int i=0; i<5; ++i){
		cout<< a[i] << ", " << b[i] << " X, -, %" << endl;
        cout<<times(a[i], b[i])<<" "<<minuss(a[i], b[i])<<" "<<divide(a[i], b[i])<<endl<<endl;;
    }
    return 0;
}
