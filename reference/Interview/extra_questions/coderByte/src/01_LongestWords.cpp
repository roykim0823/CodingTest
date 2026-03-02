#include <iostream>
using namespace std;

string LongestWord(string sen) { 

  // code goes here   
  string curr;
  string longest;
  
  for(std::size_t i=0; i<sen.size(); i++) {
      char ch = std::tolower(sen[i]);
      if(ch>='a' && ch<='z') {
          curr.push_back(sen[i]);
      } else if(ch==' ') {
          if(curr.length() > longest.length())
            longest=curr;
          curr.clear();
      }
  }
  // check for the last word
  if(curr.length() > longest.length())
        longest=curr;

    return longest;            
}

int main() { 
  
  // keep this function call here
  string input1 = "fun&!! time"
  string input2 = "I love dogs"
  cout << LongestWord(input1) << endl;
  cout << LongestWord(input2) << endl;
  return 0;
    
}
