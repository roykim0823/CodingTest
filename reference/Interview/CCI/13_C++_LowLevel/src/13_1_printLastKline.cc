// Q. Write a method to print the last K lines of an input file using C++.

/* 
Checks:
	1. Brute force: cout the number of lines (N) and then print from N-K to Nth line.
		-> Requires two reads of the files
	2. Using a K-size circular array
*/

#include <iostream>
#include <fstream>
using namespace std;

void printLastKLines(ifstream &fin, int k){
    string line[k];
    int lines = 0;
    string tmp;

	// Read file line by line into the circular array
    while(getline(fin, tmp)){//&& !fin.eof()
        line[lines%k] = tmp;
        ++lines;
    }
    // while(!fin.eof()){
    //     getline(fin, line[lines%k]);
    //     ++lines;
    // }
    int start, cnt;

    // Compute start of circular array, and the size of it
    if(lines < k){
        start = 0;
        cnt = lines;
    }
    else{
        start = lines%k;
        cnt = k;
    }
    for(int i=0; i<cnt; ++i)
        cout<<line[(start+i)%k]<<endl;
}
int main(){
    ifstream fin("13.1.in");
    int k = 4;
    printLastKLines(fin, k);
    fin.close();
    return 0;
}
