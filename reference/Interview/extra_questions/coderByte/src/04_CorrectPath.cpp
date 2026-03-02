#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <set>
using namespace std;

bool isCorrectPath(const string& s)
{
    set<pair<int,int> > visited;
    int l=0,c=0;
    for(unsigned int i=0;i<s.length();i++)
    {
        switch(s[i])
        {
            case 'd':
                l++;
                break;
            case 'r':
                c++;
                break;
            case 'u':
                l--;
                break;
            case 'l':
                c--;
                break;
            default:
                break;
        }
        if(l<0||l>=5||c<0||c>=5||!visited.insert(make_pair(l,c)).second)
        {
            return false;
        }
    }
    return l==4&&c==4;
}

void next(vector<pair<unsigned int,char> >& chars, const map<char,char>& nextChars)
{
    unsigned int i=chars.size()-1;
    do
    {
        chars[i].second=nextChars.at(chars[i].second);
    }while(chars[i--].second=='d');
}

string CorrectPath(string str) { 

  // code goes here   
  vector<pair<unsigned int,char> > chars;
  for(unsigned int i=0;i<str.length();i++)
  {
      if(str[i]=='?')
      {
          chars.push_back(make_pair(i,'d'));
      }
  }
  map<char,char> nextChars;
  nextChars['d']='r';
  nextChars['r']='u';
  nextChars['u']='l';
  nextChars['l']='d';
  for(unsigned int i=0;i<(1<<(2*chars.size()));i++)
  {
      for(unsigned int j=0;j<chars.size();j++)
      {
          str[chars[j].first]=chars[j].second;
      }
      if(isCorrectPath(str))
      {
          return str;
      }
      next(chars,nextChars);
  }
            
}

int main() { 
  
  // keep this function call here
  cout << CorrectPath(gets(stdin));
  return 0;
    
}
