// Q. Wirte a method to replace all spaces in a string with '%20'.

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

// Use the string's insert function
// Convert a space between words
void replaceSpace_ver1(string &str) {	
	string replace = "%20";
	bool space=false;   	// delimiter
	for (int i=0; i<str.size(); i++)  {
		if(str[i] != ' ')	{
			if(space == true) 	{
				str.insert(i-1, replace);
				space=false;
			}
		} else if(str[i] == ' ') {
			space=true;
			str.erase(i, 1);
		}
	}
}

// Ver2: Convert All spaces
void replaceSpace_ver2(string &str) {
	int length = str.size();
	
	// Count the number of spaces
	int spaceCnt=0;
	for(int i=0; i<length; i++) {
		if(str[i]==' ') 
			spaceCnt++;
	}
	int newLength= length+spaceCnt*2; // needed spaece = #' '*3, thus spaceCnt*2
	str.resize(newLength); 	

	for(int i=length-1; i>=0; i--) {
		if(str[i]==' ') {
			str[newLength-1] = '0';
			str[newLength-2] = '2';
			str[newLength-3] = '%';
			newLength -=3;
		} else {
			str[newLength-1] = str[i];
			newLength--;
		}
	}
}


int main()
{
	string str1 = "Mr John Smith   ";
	string str2 = "Mr John Smith   ";
	string str3 = "Mr John Smith   ";
	cout << str1 << endl;
    replaceSpace_ver1(str1);
	cout << str1 << endl;
    replaceSpace_ver2(str2);
	cout << str2 << endl;
	return 0;
}
