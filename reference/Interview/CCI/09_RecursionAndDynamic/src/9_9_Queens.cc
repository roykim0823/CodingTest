// 8 Queens problem in 8x8 board
// Hint: Each row -> there is only one queen and Each col -> there is only one queen
#include <iostream>
using namespace std;

#define Q	4	// The number of queens and the size of board (QxQ)

void print(int *c, int n){
    for(int i=0; i<n; ++i){
        for(int j=0; j<n; ++j){
            if(j == c[i]) cout<<"1 ";
            else cout<<"0 ";
        }
        cout<<endl;
    }
    cout<<endl;

	/*
	cout << "c values: ";
	for(int i=0; i<n; ++i)
		cout << c[i] << " ";
	cout << endl;
	*/
}

void search(int r, int *c, int n, int &cnt){
    if(r == n){
        print(c, n);
        ++cnt;
        return;
    }
    for(int i=0; i<n; ++i){
        c[r] = i; 	// c[r]={0, 1, 2, ... , n-1}
        int ok = 1;
        for(int j=0; j<r; ++j)
            if(c[r]==c[j] ||  		// check same column, no need to check row since only 1 q in each row
				r-j==c[r]-c[j] ||  	// r-j (row's diff) check the diagonal (NW)
				r-j==c[j]-c[r]){    // r-j (row's diff) check the diagonal (NE)
                ok = 0;
                break; // Any violation 
            }
        if(ok) search(r+1, c, n, cnt);
    }
}
int main(){
	int c[Q];	// Each queens position (row, c[row]==col)
	int cnt=0;

    search(0, c, Q, cnt);
    cout<<cnt<<endl;

    return 0;
}
