#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> divideString(string &str) {	
	vector<string> words;
	string temp;
	bool delimiter=false;   	// delimiter
	for (int i=0; i<str.size(); i++)  {
		if(str[i] != ' ')	{	// delimiter is space
        	temp+=str[i];
		} else if(str[i] == ' ') {
			if(temp.size())
				words.push_back(temp);
			temp="";
		}
	}
	if(temp.size())		// Add last word
		words.push_back(temp);

	return words;
}

int main()
{
	string str = "  Mr John Smith   Roy";
	cout << str << endl;
	vector<string>::iterator iter;
	vector<string> words = divideString(str);
	for(iter=words.begin(); iter!=words.end(); ++iter)
		cout << *iter << endl;
	return 0;
}
