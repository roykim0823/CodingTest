// Q. Assume you have a method is "isSubstring" which checks if one word is a substring of another. 
//    Given two strings, s1 and s2, write code to check if s2 is a rotatin of s1 using only one call to 
//    "isSubstring". e.g., "waterbottle" is a rotation of "erbottlewat"

/* 
Checks:
	If we imagine that s2 is a rotation of s1, then we can ask what the 
	rotation point is. For example, if you rotate waterbottle after wat,
	you get erbottlewat. In a rotation, we cut s1 into two parts, x and y,
	and rearrange them to get s2.
 		s1 = xy = waterbottle
 		x = wat, y=erbottle
		s2 = yx = erbottlewat
 	So, we need to check if there's a way to split s1 into x and y such that
	xy = s1 and yx=s2. Regardless of where the division between x and y is,
 	we can see that yx will always be a substring of xyxy. That is, s2 will
	always be a substring of s1s1.
Solutions:
	1. check the strings are empty
	2. check the strings' sizes are equal
	3. s2 is substring of s1+s1?
*/

#include <iostream>
#include <string>

using namespace std;

bool isSubstring(string s1, string s2) {
	if(s1.find(s2) != string::npos) 
		return true;
	else
		return false;
}

bool isRotation(string s1, string s2)
{
	// If one of strings is empty, then return false
	if(s1.empty() || s2.empty())
		return false;
	// If the length of s2 is different from s1, return false
	if(s1.size() != s2.size())
		return false;
	
	return isSubstring(s1+s1, s2);
}

int main() {
	string s1, s2;
	cin>> s1;
	cin>> s2;
	cout << isRotation(s1, s2) << endl;
	return 0;
}
	
