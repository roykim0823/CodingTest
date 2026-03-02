// Q. Given a sorted array of strings which is interspersed with empty strings, 
//    write a method to find the location of a given string. 
// Example: find “ball” in [“at”, “”, “”, “”, “ball”, “”, “”, “car”, “”, “”, “dad”, “”, “”] 
// will return 4
// Example: find “ballcar” in [“at”, “”, “”, “”, “”, “ball”, “car”, “”, “”, “dad”, “”, “”] 
// will return -1

#include <iostream>
#include <string>

using namespace std;

int search(string s[], string x, int low, int high){
	// search for empty string: ERROR
    if(x == "") return -1;

	// !!Write Recursive version
    while(low <= high){
        int mid = (low+high)/2;
        int t = mid;
        
		while(s[t] == "" && t <= high) ++t;		// check mid has "" 

        if(t > high) high = mid - 1;  			// right part is all ""
        else
		{
            if(s[t] == x) return t;     		// matched, return the index
            else if(s[t] < x) low = t + 1;  	// mid is less than x, throw away left part
            else high = mid - 1; 				// or throw away right part
        }
    }
    return -1;
}

// Recursrive
int searchR(string s[], string x, int low, int high) {
	if(low>high) return -1;

	// move mid to the middle
	int mid = (low+high)/2;

	// if mid is "", find the closest non-empty string
	if(s[mid]=="") {
		int left = mid-1;
		int right = mid+1;
		while(true) {
			if(left<low && right>high) {
				return -1;
			} else if(right <= high && s[right]!="") {
				mid=right;
				break;
			} else if(left >= low && s[left]!="") {
				mid=left;
				break;
			}
		right++;
		left--;
		}
	}

	// Check for string, and recurse if necessary
	if(s[mid]==x) { // found it
		return mid;
	} else if (s[mid] < x) {	// search right 
		return searchR(s, x, mid+1,  high);
	} else {	// search left
		return searchR(s, x, low, mid-1);
	}
}

int callsearchR(string s[], string x, int size) {
	//if(s==NULL || x==NULL || x=="") return -1;
	if(s==NULL || x=="") return -1;

	return searchR(s, x, 0, size-1);
}


int main(){
    string s[13] = {
        "at", "", "", "", "ball", "", "", "car", "", "", "dad", "", ""
    };
    cout<<search(s, "ball", 0, 12)<<endl;
    cout<<search(s, "ballcar", 0, 12)<<endl;
    cout<<callsearchR(s, "ball", 13)<<endl;
    cout<<callsearchR(s, "ballcar", 13)<<endl;
    return 0;
}
