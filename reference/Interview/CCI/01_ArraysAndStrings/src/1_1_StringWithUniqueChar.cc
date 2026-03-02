// Q. Implement an algorithm to determine if a string has all unique characters.
// Q+. What if you cannot use additional data structure? 

// Checks:
// 1. Clarify the string: is it ASCII string or a Unicode string?
// 2. Assume ASCII string (256 characters)

// Solutions:
// 1. If the size of string is larger than 256 -> return false
//    ver1: use data structure
//    ver2: Bit vector, char charSet[8], cover all ASCII
//    ver3: use single variable (bit vector), cover only alphbet character 


#include <iostream>
#include <memory>
#include <string>

using namespace std;

bool isUniqueChars_ver1(string str) {
	if(str.size() > 256 )
		return false;
	
	std::unique_ptr<char []> bool (new char[256]());  // declare and initialize
	
	for(int i=0; i != str.size(); ++i) 
	{
		int val = (int)str[i];
		if( charSet[val]) // Already found this char in the str
			return false;
		charSet[val] = true;
	}
	return true;
}

// Bitmap version
bool isUniqueChars_ver2(string str) {
	if(str.size() > 256 )
		return false;
	
	unsigned charSet[8];
	// Initialize charSet
	for(int i=0; i < 8; ++i)
		charSet[i] = 0;

	for(int i=0; i != str.size(); ++i) 
	{
		int val = (int)str[i];
		int idx = val/32, shift=val%32;
		if( charSet[idx] & (1<<shift)) 
			return false;
		charSet[idx] |= (1 << val);
	}
	return true;
}

// Only works in a-z or A-Z (less than 32 cases)
bool isUniqueChars_ver3(string str) {
	if(str.size() > 256 )
		return false;

	int checker = 0;
	for(int i=0; i != str.size(); ++i) 
	{
		int val = (int)(str[i] - 'a');
		if( (checker & (1 << val)) > 0) {
			return false;
		}
		checker |= (1 << val);	// update bit vector
	}
	return true;
}

int main()
{
	string input;
	cin >> input;

	if(isUniqueChars_ver1(input) == true)
		cout << "True" << endl;
	else
		cout << "False" << endl;
	
	return 0;
}
