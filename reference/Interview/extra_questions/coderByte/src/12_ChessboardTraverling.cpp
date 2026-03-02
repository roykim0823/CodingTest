#include <iostream>
#include <vector>

using namespace std;

vector<int> tokenizer(string str) {
	vector<int> nums;
	nums.push_back(str[1] - '0');
	nums.push_back(str[3] - '0');
	nums.push_back(str[6] - '0');
	nums.push_back(str[8] - '0');

	return nums;
}

int factorial(int n) {
	int f=n;
	while(n>1) {
		--n;
		f*=n;
	}
	return f;
}


int ChessboardTraveling(string str) { 
	vector<int> nums;

	// get the coordination
	nums = tokenizer(str);

	int x_diff = nums[2]-nums[0]; 	// a-x
	int y_diff = nums[3]-nums[1];   // b-y

	cout << nums[0] << " " << nums[1] << " " << nums[2] << " " << nums[3] << endl;
	cout << x_diff << " " << y_diff << endl;
	// n chooses k = n! / k! (n-k)!
	int ans = factorial(x_diff+y_diff) / (factorial(x_diff) * (factorial(y_diff)));
	
	return ans; 
}


// from (1,1) to (3,3) in 3x3 chess board
// xxyy, xyxy, xyyx, yxyx, yxxy, yyxx - 6 ways
// ( (x_diff+y_diff)! / (x_diff)!(y_diff)! )
// combination without replacement



int main() { 
	  
	// keep this function call here
	//string s;
	//cout << ChessboardTraveling(gets(stdin));
	cout << ChessboardTraveling(s);
	return 0;
} 

