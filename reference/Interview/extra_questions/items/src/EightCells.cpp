// 8 Cell's next day's state
// 1. 0 if both neighbor cells are 0s or 1s (same), otherwise 1
// 2. assume first and last cell's left and right cell's state is 0

#include <iostream>
#include <vector>

using namespace std;

void print(vector<int> vec) {
	for(size_t i=0; i<vec.size(); i++)
		cout << vec[i] << " ";
	cout << endl;
}

vector<int> cellCompete(int* states, int days) 
{
    // WRITE YOUR CODE HERE  
    const int cellLength=8;
    vector<int> curState;
    vector<int> nextState;
    
    // Copy the input to curState;
    for(int i=0; i<cellLength; i++) 
        curState.push_back(states[i]);
    nextState.resize(cellLength);
        
    while(days--) {
        // calculate end cells' states
		/*
        if(curState[1]==0)
            nextState[0]=0;
        else 
            nextState[0]=1;
		cout << nextState[0] << " " << nextState.size() << endl;
        if(curState[6]==0)
            nextState[7]=0;
        else
            nextState[7]=1;
        */
		// Since first cell's left and last cell's right is 0
		// then their state is decided by first's right and last's left cell
		nextState[0] = curState[1];	
		nextState[7] = curState[6];
        for(int i=1; i<cellLength-1; i++) {
            if(curState[i-1]==curState[i+1])
                nextState[i]=0;
            else
                nextState[i]=1;
        }
        curState=nextState;
    }
    return nextState;
}

int main() {
	int input1[8] = {1, 0, 0, 0, 0, 1, 0, 0};
	int day1=1;

	vector<int> t = cellCompete(input1, day1);
	print(t);

	return 0;
}
