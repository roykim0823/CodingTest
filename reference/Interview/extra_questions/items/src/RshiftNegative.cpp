#include <iostream>
#include <string>
#include <vector>

using namespace std;

int leftShift(int a) {
	int result = 0;
	while ( (a <<= 1) && result<128) {
		cout << a << " ";
		result+=1;
		if (result%8==0)
			cout << endl;
	}
	return result;
}

int rightShift(int a) {
	int result = 0;
	while ( (a >>= 1) && result<128) {
		cout << a << " ";
		result+=1;
		if (result%8==0)
			cout << endl;
	}
	return result;
}

int main()
{
	cout << "right shift negative integer: -16" << endl;
	rightShift(-16);cout << endl;
	cout << "right shift positive integer: 16" << endl;
	rightShift(16); cout << endl;
	cout << "left shift negative integer: -16" << endl;
	leftShift(-16);	cout << endl;
	cout << "left shift positive integer: 16" << endl;
	leftShift(16);	cout << endl;
	//cout << foo(-16) << endl;
}
