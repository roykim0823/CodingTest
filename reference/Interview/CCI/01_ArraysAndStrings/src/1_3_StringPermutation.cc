// Q. Given two strings, write a method to decide if one is permutation of the other.
// -> Ask anagram

/* 
Checks:
1. Clarify the question: 
	a. if the anagram comparion is case sensitive or not.
	b. if whitespace is significant
2. Assume: case sensitive and whitespace is significant.
3. If two strings are anagrams, then they have the same characters, but in
   different order.

Solutions:
	1. Compare the string length. If the length is different, return false 
    2. Sort both string
	3. Compare the sorted string
*/

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

bool permutation(string str1, string str2) {	
	if( str1.size() != str2.size() )
		return false;				

	string tmp1 = str1;
	string tmp2 = str2;
	sort(tmp1.begin(), tmp1.end());
	sort(tmp2.begin(), tmp2.end());
	if(tmp1 == tmp2)
		return true;	// sorted string are the same, then they are anagram.
	else
		return false;
}

int main()
{
	string str1;
	string str2;
	cin >> str1 >> str2;

    bool result = permutation(str1, str2);
	if(result)
		cout << "True" << endl;
	else
		cout << "False" << endl;

	return 0;
}
