#include <iostream>
#include <vector>
#include <cmath>
#include <iterator>
#include <random>

using namespace std;

template <typename T>
void print_vec(vector<T> vec) {
	for(auto iter= vec.begin(); iter != vec.end(); ++iter)
		cout << *iter << " ";
	cout << endl;
}

// Solution 1: Iterative
// One to One relationship between the 2^|S| bit arrays of length |S| 
// and the set of all subsets of S
// Time = O(|S| * 2^|S|)
vector<vector<int> > generate_power_set(const vector<int> &S) {
	vector<vector<int> > power_set;
	for(int i=0; i< (1<< S.size()); ++i) {		// 1<<S.size() = 2^S.size()
		vector<int> power;
		int x=i;
		while (x) {
			int idx = log2( x & ~(x-1));  		// log2(the right most set bit)
			power.push_back(S[idx]);
            x &= x-1;							// x = x with the right most '1' is cleared
		}
		power_set.push_back(power);
	}
	return power_set;
}

vector<vector<int> > generate_power_set2(const vector<int> &S) {
	vector<vector<int> > power_set;
	for(int i=0; i< ( 1<< S.size()); ++i) {		// 1<<S.size() = 2^S.size()
		vector<int> power;
		for(size_t j=0; j<S.size(); j++) {
			if(i & (1 << j))       	// get the proper element based on i value
				power.push_back(S[j]);
		}
		power_set.push_back(power);
	}
	return power_set;
}

// Solution 2: Recursion
// Time = O(|S| * 2^|S|), Space = O(|S|) which is the maximum of stack depth
void generate_power_set_helper(const vector<int> &S, int idx, vector<int> &res) {
	if(res.empty() == false) {
		// Print the subset
		copy(res.cbegin(), res.cend()-1, ostream_iterator<int>(cout, ", "));
		cout << res.back();
	}
	cout << endl;

	for(size_t i=idx; i< S.size(); i++) {
		res.emplace_back(S[i]);
		generate_power_set_helper(S, i+1, res);
		res.pop_back();
	}
}

void generate_power_setR(const vector<int> &S) {
	vector<int> res;
	generate_power_set_helper(S, 0, res);
}

// Find all factors of the product of a given list of numbers (or primes)
// ex) {2, 3, 7} -> 1, 2, 3, 6, 7, 14, 21, 42
// modify the generate_power_set
vector<int> factors(const vector<int> &S) {
	vector<int> products;
	for(int i=0; i< ( 1<< S.size()); ++i) {		// 1<<S.size() = 2^S.size()
		int x=i;
		int product = 1;
		bool one=false;
		while (x) {
			int idx = log2( x & ~(x-1));  		// log2(the right most set bit)
			if(S[idx]==1) {                     // '1' is element then skip the product calculation
				one=true;
				break;;
			}
			product *= S[idx];                  // calcuate the product
            x &= x-1;							// x = x with the right most '1' is cleared
		}
		if(one==false)
			products.push_back(product);
	}
	return products;
}

vector<int> factors2(const vector<int> &S) {
	vector<int> products;
	for(int i=0; i< ( 1<< S.size()); ++i) {		// 1<<S.size() = 2^S.size()
		int product=1;
		bool one=false;
		for(size_t j=0; j<S.size(); j++) {
			if (i & (1 << j)) {       	// get the proper element based on i value
				if(S[j]==1) {
					one=true;
					break;;
				}
				product *= S[j];
			}
		}
		if(one==false)
			products.push_back(product);
	}
	return products;
}
