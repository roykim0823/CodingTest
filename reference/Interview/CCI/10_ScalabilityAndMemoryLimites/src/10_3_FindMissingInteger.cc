// Q. Given an input file with four billion non-negative integers, provide an algorithm to generate
//    an integer which is not contained in the file. 
//    -> Find missing numbers from 4 billion numbers: 2^32=4 billon
//    Only with 1GB memory
// Q+. Only with 10MB memory

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#define N	20000		// 20000 numbers
using namespace std;

// Using Simple Bit Vector
void findOpenNum_v1(string fileName)	// Assuming Enough Memory: 1GB memory
{
	int int_len = sizeof(int)*8;		// number of bits in int
	int bit_len = N / int_len;			// size of integer for all bits
	int *flag_bit = new int[bit_len];	// flag_bit
    int v;								// store each number

    freopen(fileName.c_str(), "r", stdin); 	// READ the input file

	while(scanf("%d", &v) !=EOF) {
		flag_bit[v/int_len] |= 1 << (v%int_len);	// Set each number's flag bit
	}

	for(int i=0; i<bit_len; ++i) {
		for(int j=0; j<int_len; ++j) {
			if ( (flag_bit[i] & (1<<j)) == 0 ) {	// If the flag_bit is not set
				cout << i*int_len + j << endl;
			}
		}
	}
	delete [] flag_bit;
	fclose(stdin);
}

// Using Block-wised counter: 2 phases algorithm
// 1. Each counter counts the number in the certain range (block)
//	  -> read file once
// 2. Find any counter values are less than the range -> some numbers are missed.
//    -> #reading file = #counters < the range
void findOpenNum_v2(string fileName)	// Assuming Small Memory: 10M memory
{
	int int_len = sizeof(int)*8;		// number of bits in int
	int cntRange = N/100;				// Range of the counter
	int cntSize = N/cntRange;			// size (length) of the couter 
	int *cnt = new int[cntSize];
    int v;								// store each number
	vector<bool> flag_bit;             	// Not using integer since cntRange does not fit in the integer
	flag_bit.resize(cntRange);
	

	for(int i=0; i<cntSize; i++) 
		cnt[i]=0;	// Initialize to 0
	// 1st Phase
    freopen(fileName.c_str(), "r", stdin); 	// READ the input file
	while(scanf("%d", &v) !=EOF) {
		++cnt[v/cntRange];
	}
	fclose(stdin);
    
	// Find a counter value is less than the range
	int sum=0;
	for(int i=0; i<cntSize; ++i) {
		if(cnt[i] < cntRange) {
			// 2nd Phases
			int start_range = i*cntRange;
			//int flag_bit_size = cntRange/int_len+1;
			//int *flag_bit = new int[flag_bit_size];	// flag_bit used in the 2nd phase
			for(int i=0; i<cntRange; i++)
				flag_bit[i]=false;
    		freopen(fileName.c_str(), "r", stdin); 			// READ the input file
			while(scanf("%d", &v) !=EOF) {
				if (v>=start_range && v<start_range+cntRange) {
					v-=start_range;	// make v in [0, cntRange)
					flag_bit[v] = true;	// Set each number's flag bit
				}
			}

			for(int i=0; i<cntRange; ++i) {
				if(flag_bit[i]==false) {
					cout << i + start_range << endl;
				}
			}
			fclose(stdin);
		}
	}
}

int main(){
    findOpenNum_v1("10_3.in");
	cout << endl;
    findOpenNum_v2("10_3.in");
    return 0;
}
