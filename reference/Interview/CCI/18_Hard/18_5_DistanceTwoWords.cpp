// Q. You have a large text file containing words. Given any two words, find the shortest distance
//    (in terms of number of words) between them in th file. Can you make the searching operation in O(1)?
//    What about the space complexity for your solution?
//	  -> Minimum distance between given two words in a large text files
//    -> Use hashtable for other pairs of words

/* Checks: first ask your interviewer the word order matters. If so, small modification required.
	Assumption: the word order doesn't matter
*/

/* Solution: 
	1. keep track of the last position of word1 and word2
	2. calculate the distacne
	3. compare it the previous min distance
*/
	
#include <iostream>
using namespace std;

const int kMaxInt = ~(1<<31);	// MAX integer

int ShortestDist(string text[], int n, string word1, string word2){
    int min = kMaxInt;
    int pos1 = -1;
    int pos2 = -1;

	cout << min << endl;
    for(int pos=0; pos<n; ++pos){
        if(text[pos] == word1){
            pos1 = pos;
            int dist = pos1 - pos2;
            if(dist < min)
                min = dist;
        }
        else if(text[pos] == word2){
            pos2 = pos;
            int dist = pos2 - pos1;
            if(dist < min)
                min = dist;
        }
    }

    return min;
}
int main(){
    string text[] = {
        "What","is","your","name","My","name","is","Hawstein"
    };
    int len = 8;
    cout<<ShortestDist(text, len, "My", "is")<<endl;
    return 0;
}
