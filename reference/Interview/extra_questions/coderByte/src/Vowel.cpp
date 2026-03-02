#include <iostream>
#include <string>
#include <sstream>
using namespace std;

bool isVowel(const char c)
{
    return c=='a'||c=='e'||c=='i'||c=='o'||c=='u'||c=='y';
}

string VowelSquare(string strArr[]) { 
    int rows = sizeof(strArr)/sizeof(*strArr);
    //int cols = strArr->length();
    int cols = strArr[0].size();

  // code goes here   
  for(unsigned int l=0;l<rows-1;l++)
  {
      for(unsigned int c=0;c<cols-1;c++)
      {
          if(isVowel(strArr[l][c])&&isVowel(strArr[l][c+1])&&isVowel(strArr[l+1][c])&&isVowel(strArr[l+1][c+1]))
          {
              stringstream ss;
              ss<<l<<'-'<<c;
              return ss.str();
          }
      }
  }
  return "not found"; 
            
}

int main() { 
   
  // keep this function call here
  /* Note: In C++ you first have to initialize an array and set 
     it equal to the stdin to test your code with arrays. */
     
  string A[] = gets(stdin);
  cout << VowelSquare(A);
  return 0;
    
}
