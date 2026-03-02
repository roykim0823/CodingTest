#include <iostream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <cmath>
using namespace std;

string ScaleBalancing(string strArr[]) { 

  // code goes here   
  char* s=(char*)strArr[0].c_str()+1;
  unsigned int minSide=strtol(s,&s,10),maxSide=strtol(s+1,0,10);
  if(minSide>maxSide)
  {
      //minSide^=maxSide^=minSide^=maxSide;
      unsigned int temp=minSide;
      minSide=maxSide;
      maxSide=temp;
  }
  
  vector<unsigned int> weights;
  stringstream ss;
  // Single element on smaller side
  for(s=(char*)strArr[1].c_str();*s!=']';)
  {
      weights.push_back(strtol(s+1,&s,10));
      if(minSide+weights.back()==maxSide)
      {
          ss<<weights.back();
          return ss.str();
      }
  }
  
  for(unsigned int i=0;i<weights.size();i++)
  {
      for(unsigned int j=i+1;j<weights.size();j++)
      {
          // two element on smaller side || each element on both side
          if(minSide+weights[i]+weights[j]==maxSide || minSide+weights[i]==maxSide+weights[j] || minSide+weights[j]==maxSide+weights[i])
          {
              ss<<min(weights[i],weights[j])<<','<<max(weights[i],weights[j]);
              return ss.str();
          }
      }
  }
  return "not possible"; 
  // code goes here   
  
            
}

int main() { 
   
  // keep this function call here
  /* Note: In C++ you first have to initialize an array and set 
     it equal to the stdin to test your code with arrays. */
     
  string A[] = gets(stdin);
  cout << ScaleBalancing(A);
  return 0;
    
}
