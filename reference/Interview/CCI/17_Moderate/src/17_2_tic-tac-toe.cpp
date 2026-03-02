// Q. Design an algorithm to figure out if someone has won a game of tic-tac-toe

/* 
Checks: There are a few major design decisions to consider
	1. Will hasWon be called just once, or many times (for instance, as part of a tic-tac-toe
	   website)? If the latter is the case, we may want to add pre-processing time in order to
	   optimize the runtime of hasWon.
	2. Tic-tac-toe is usually on a 3x3 board. Do we want to design for just that, or do we want
	   to implement it as an NxN solution?
	3. In general, how much do we prioritize compactness of code vs. speed of execution vs. 
	   clarity of code? Remember that the most efficient code may not always be the best.
	   Your ability to understand and maintain the code matters too.
*/

	
#include <iostream>
using namespace std;

enum Check{
    ROW, COLUMN, DIAGONAL, REDIAGONAL
};

enum Marks {
	E, O, X	// E:Empty, O:Nought, X:Cross
};

int Check(Marks board[], int n, Check check){
    int type = 0;
    for(int i=0; i<n; ++i){
        bool found = true;
        for(int j=0; j<n; ++j){
            int k = 0;
            if(check == ROW)
                k = i * n + j;		// access row element
            else if(check == COLUMN) 
                k = i + j * n;		// access col element
        	else if(check == DIAGONAL)
            	k = i + i * n; 		// SE diag
        	else
            	k = i + (n-1-i) * n;	// SW diag

            if(j == 0){
                type = board[k]; 	// type is the first element
            }
            else if(board[k] != type){
                found = false; 		// Different Mark is found
                break; 
            }
        }
        if(found) return type;
    }
    return 0;
}

int HasWon(Marks board[], int n){
    int type = 0;
    type = Check(board, n, ROW);
    if(type != 0) return type;

    type = Check(board, n, COLUMN);
    if(type != 0) return type;

    type = Check(board, n, DIAGONAL);
    if(type != 0) return type;

    type = Check(board, n, REDIAGONAL);
    if(type != 0) return type;

    return 0;
}
int main(){
    int n = 3; // 3*3
    
	Marks board[] = {
        X, X, O,
        O, X, X,
        O, X, E,
    };

    int type = HasWon(board, n);
    if(type != 0)
        cout<<type<<" won!"<<endl;
    else
        cout<<"nobody won!"<<endl;
    return 0;
}
