#include <iostream>
#include <string>
using namespace std;
int chessboard[8][8];

void printChess() {
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            cout << chessboard[i][j] << " ";
        }
        cout << endl;
    }
}
int noattack(int x, int y, int k) {
    // check the current position
    if(chessboard[x][y]>0)
        return chessboard[x][y];
    else
        chessboard[x][y]=k;
        
    for(int i=0; i<8; i++) {
        // check row and col
        if(x!=i) {
            if (chessboard[i][y]>0) {
                //cout << "[i][y]" << " " << i << " " << y << " " << x << endl;
                 return chessboard[i][y];
            }
               
          
        }
        if(y!=i) {
            if (chessboard[x][i]>0) {
                // cout << "[x][i]" << " " << x << " " <<i << endl;
                 return chessboard[x][i];
            }
          
        }
            
        // check diagonal
        if(i>0) {
            if(x+i < 8 && y+i < 8) { // NE
                if(chessboard[x+i][y+i]>0) {
                    // cout << "[x+i][y+i]" << " " << i  << endl;
                 return chessboard[x+i][y+i];
            }
              
            }
            if(x+i < 8 && y-i >= 0) { // NW
                if(chessboard[x+i][y-i]>0) {
                     // cout << "[x+i][y-i]" << " " << i  << endl;
                 return chessboard[x+i][y-i];
            }
                
            }
            if(x-i >= 0 && y+i < 8) { // SE
                if(chessboard[x-i][y+i]>0) {
                      // cout << "[x-i][y+i]" << " " << i  << endl;
                 return chessboard[x-i][y+i];
            }
             
            }
            if(x-i >=0  && y-i >= 0) { // SW
                if(chessboard[x-i][y-i]>0) {
                     // cout << "[x-i][y-i]" << " " << i  << endl;
                 return chessboard[x-i][y-i];
            }
               
            }
        }
        //cout << x << " " << y << endl;
       // cout << i << endl;
    } 
    return 0;
}
string EightQueens(string strArr[]) { 
    size_t strSize = 8; //sizeof(strArr)/sizeof(string);
    
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++) 
            chessboard[i][j]=0;
    
    for(size_t i=0; i<strSize; i++) {
        int x=strArr[i][1]-'0';
        int y=strArr[i][3]-'0';
        //cout << x << " " << y << endl;
        int result=noattack(x-1, y-1, i+1);
        
        if(result>0) {
            //printChess();
            return strArr[result-1];
        }
            
    }
    return "true";

  // code goes here   
  //return strArr[0]; 
            
}

int main() { 
   
  // keep this function call here
  /* Note: In C++ you first have to initialize an array and set 
     it equal to the stdin to test your code with arrays. */
     
  string A[] = gets(stdin);
  cout << EightQueens(A);
  return 0;
    
}
