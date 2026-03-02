// Q. Design a method to find the frequency of occurrences of any given word in a book.

/*
Checks:
1. Single Query
	Simply go through the book, word by word, and count the number of times that a word appears.
	We know we can't do better thatn that since we must look at every word in the book.

2. Repetitive Queries
	If we're doing the operation repeatedly, then we can probably afford to take some time and 
	extra memory to do pre-processing on the book. We can create a hash table which maps from a word
	to its frequency. The frequency of any word can be easily looked up in O(1) time. 
*/

#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <boost/algorithm/string.hpp> 	// boost for string trim
using namespace std;

void setupDictionary(vector<string> &book, map<string, int> &word_table) {
    vector<string>::iterator iter;
	for(iter=book.begin(); iter!=book.end(); ++iter) 
	{
		string word=*iter;
		// convert the word in the book to lowercase
		boost::algorithm::to_lower(word);
		// trim the word
		boost::algorithm::trim(word);	// trim both sides

		if(word!="") {
			if(word_table.find(word)!=word_table.end()) {
				// find the word, increase the counter
                word_table[word]+=1;
			} else { // not Foudn the word
				word_table.insert(pair<string,int>(word,0));
			}
		}
	}
	return;
}

int getFrequency(string word, map<string, int> &word_table) {
	if(word_table.size()==0 || word=="") return -1;
	boost::algorithm::to_lower(word);
	if(word_table.find(word)!=word_table.end()) 
		return word_table[word]+1;
	else
		return 0;	// NOT Found
}

int main(){
	string str[] ={ "a", "abc", "ad", "abcd", "abc", "kor" };
	vector<string>book;
	for(int i=0; i<6; i++)
		book.push_back(str[i]);
	
	map<string, int> word_table;
	setupDictionary(book, word_table);
	cout << getFrequency("A", word_table) << endl;
	cout << getFrequency("ABC", word_table) << endl;
	cout << getFrequency("", word_table) << endl;
	cout << getFrequency("KOR", word_table) << endl;
	cout << getFrequency("abcd", word_table) << endl;
	cout << getFrequency("ddd", word_table) << endl;


    
	return 0;
}
