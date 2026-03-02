// Given a list of words, write a program to find the longest word made of other words in the list
#include <iostream>
#include <algorithm>
#include <set> 
#include <string>
using namespace std;

inline bool cmp(string s1, string s2){	// Compare length of string
    return s2.length() < s1.length();
}

bool MakeOfWords(string word, int length, set<string> s_set){
    //cout<<"curr: "<<word<<endl;
    int len = word.length();
    //cout<<"len:"<<len<<endl;
    if(len == 0) return true;

    for(int i=1; i<=len; ++i){
        if(i == length) return false;	
        string str = word.substr(0, i);
        //cout<<str<<endl;
        if(s_set.find(str)!=s_set.end()){ 	// substring matches one of the string list
            if(MakeOfWords(word.substr(i), length, s_set) )
                return true;
        }
    }
    return false;
}

void PrintLongestWord(string word[], int n){
    set<string> s_set;
    for(int i=0; i<n; ++i)  	// Make Hash Table with strings of the list
		s_set.insert(word[i]);
    sort(word, word+n, cmp);   	// Sort the string list by length

    for(int i=0; i<n; ++i){
        if(MakeOfWords(word[i], word[i].length(), s_set)){
            cout<<"Longest Word: "<<word[i]<<endl;
            return;
        }
    }
}

int main(){
    string arr[] = {"test", "tester", "testertest", "testing", 
				"apple", "seattle", "banana",  "batting", "ngcat", 
                "batti","bat", "testingtester", "testbattingcat", "aaaaaaaaaaaaaaaaa", "a"};
    int len = 15;
    PrintLongestWord(arr, len);
    return 0;
}
