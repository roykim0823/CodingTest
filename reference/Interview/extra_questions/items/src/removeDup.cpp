#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// assumption: vec is unsorted 
// Time Complexity: O(n)
// Space Complexity: O(n) in Hash

vector<int> removeDuplicate(vector<int> &vec) {
	// using hash to detect any duplication
	//unordered_map<int, bool> hash;
	unordered_set<int> hash;
	vector<int> noDup;	// vector to store unique numbers

	for(int i=0; i<vec.size(); i++) {
		if(hash.find(vec[i]) != hash.end()) // duplicate found
			continue;
		//hash[vec[i]]=true;					// hash the number to detect its duplicates
		hash.insert(vec[i]);
		noDup.push_back(vec[i]);            // insert unique number in noDup 
	}
	return noDup;
}

int main() {
	vector<int> data = { 1, 3, 4, 2, 13, 9, 3, 16};
	vector<int> noDup=removeDuplicate(data);
	for(int i=0; i<noDup.size() ; i++) {
		cout << noDup[i] << " ";
	}
	cout << endl;
}
