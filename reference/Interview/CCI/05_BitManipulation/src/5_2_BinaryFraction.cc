// Q. print a real number between 0 and 1 (e.g., 0.72) in the binary representation
// 	with at most 32 characters or print ERROR
// ex) .0101(2) = 1 * (1/2^1) + 0 * (1/2^2) + 1 * (1/2^3) = 1 * 0.5 + 0 * 0.25 + 1* 0.125

/*
Solutions:
	A. To print the decimal part, multiply n by 2 and check if 2n>=1.
	   This is "shifting" the fractional sum.
	B. Instead of multiplying 2, compare n to 0.5, 0.25, ...
*/

#include <iostream>
#include <bitset>
#include <string>
#include <vector>

using namespace std;

// Version A
string realToBinary(double num) {
   	if (num >=1 || num <=0) {
		return "Error.";
	}

	string binary;
	binary.append(".");
	while(num>0) {
		// Setting a limit on length: 32 chracters.
		if(binary.size() >=32) {
			return "Error.";
		}

		double r=num*2;
		if(r>=1) {
			binary.append("1");
			num=r-1;
		} else {
			binary.append("0");
			num = r;
		}
	}

	return binary;
}
	
// Version B	
string realToBinary2(double num) {
   	if (num >=1 || num <=0) {
		return "Error.";
	}

	string binary;
	double frac=0.5;
	binary.append(".");
	while(num>0) {
		// Setting a limit on length: 32 chracters.
		if(binary.size() >=32) {
			return "Error.";
		}

		if(num >=frac) {
			binary.append("1");
			num-=frac;
		} else {
			binary.append("0");
		}
		frac /=2;	// fraction keeps decreasing
	}

	return binary;
}
	
int main()
{
	vector<double> num = {0.5, 0.25, 0.125, 0.75, 0.875, 0.375, 0.975};
	for(int i=0; i<num.size(); i++) {
		cout << "num = " << num[i] << endl;
		cout << "ver1 = " << realToBinary(num[i]) << endl;
		cout << "ver2 = " << realToBinary2(num[i]) << endl;
	}
}	
