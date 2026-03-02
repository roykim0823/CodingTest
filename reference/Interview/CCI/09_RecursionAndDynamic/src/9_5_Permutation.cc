// Q. Write a method to compute all permutations of string
#include <iostream>
#include <vector>
using namespace std;

typedef vector<string> vs;

// My permulation: Using BackTracking
void MyPermute(string s, int start, int end) {
	char temp;
	if (start==end) 
		cout << s << endl;
	else {
		for(int i=start; i<=end; i++) {
			// swap s[l] and s[i]
			temp = s[start];
			s[start]=s[i];
			s[i]=temp;

			cout << "LOOP: " << s << " " << i << " " << start << endl;
			MyPermute(s, start+1, end);
			cout << "ENDD: " << s << " " << i << " " << start << endl;
			// swap s[l] and s[i] -> Backtrack
			temp = s[start];
			s[start]=s[i];
			s[i]=temp;
		}
	}
}

int main(){
    string s = "abc";
    MyPermute(s, 0, s.size()-1);
    return 0;
}
