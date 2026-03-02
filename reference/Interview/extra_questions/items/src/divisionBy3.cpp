#include <iostream>
using namespace std;

// Perform x/3 without divisor operator
// 1/3 = 0.3333...
// 1/3 = 0.01010101... in binary so, 
// 1/3 = 1/4 + 1/16 + 1/64 + ...
float division(float x) {
	float t=0;
	int cnt=0;
	while(x>0) {
		x=x/4;
    	t+=x;
		cnt++;
	}
	cout << cnt << "\titers returns -> "; 
	return t;
}

int main() {
	for(int i=0; i<100; i++) {
		cout << i << " / 3 = " << division((float)i) << endl;
	}
	return 0;
}
