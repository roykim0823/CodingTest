#include <iostream>
#include <unordered_map>
#include <string>
#include <stack>
#include <vector>

struct Names {
	int id;
	int pid;
	std::string name;
};


/* Names:
	Animal, id = 1, pid = -1;
    	Mammal, id = 20, pid = 1
			Cat, id = 200, pid = 20
			Dog, id = 300, pid = 20
			Man, id = 111, pid = 20
		Insects, id = 40, pid = 1
			Fly, id = 410, pid = 40
			Spider, id = 420, pid = 40
			...
*/


void fillNames(std::vector<Names> & vec) {
	vec.push_back({420, 40, "Spider"});
	vec.push_back({410, 40, "Fly"});
	vec.push_back({111, 20, "Man"});
	vec.push_back({300, 20, "Dog"});
	vec.push_back({200, 20, "Cat"});
	vec.push_back({40, 1, "Insects"});
	vec.push_back({20, 1, "Mammal"});
	vec.push_back({1, -1, "Animal"});
}

// first approach
// without sorting
// searching fullName: n x O(n) = n^2
// however, with hashing the parent name
// close to n

void printFullName(std::vector<Names> &vec) {
	std::unordered_map<int, int> index_hash;	// search without sorting
												// bindary search can be applied after sorting by id 
	std::unordered_map<int, std::string> parentName_hash;
	std::stack<std::string> fullName;

	for(int i=0; i<vec.size(); i++) {
		index_hash[vec[i].id] = i;
	}

	for(int i=0; i<vec.size(); i++) {
		int pid = vec[i].pid;
		std::string concat;
		if(parentName_hash.find(pid) != parentName_hash.end()) { // found
			//std::cout << "FROM HASH -> ";
			concat = parentName_hash[pid];
		} else {

			//fullName.push(vec[i].name);
			while(pid != -1) {
				Names parent = vec[index_hash[pid]];

				// insert parentName
				fullName.push(parent.name);
				// update pid
				pid = parent.pid;
			}

		
			while(fullName.empty()==false) {
				concat += fullName.top()+".";
				fullName.pop();
			}

			// Add parent string to parentName_hash;
			parentName_hash[vec[i].pid] = concat;
		}
		
		concat += vec[i].name;
		std::cout << "Name: " << vec[i].name << "\tFull Name: " << concat << std::endl;
	}
}

// another approach
// sorting by ID: O(nlogn)
// searching fullName: n x O(logn) : O(nlongn)
// -> O(nlogn) 
// with cache : close to n
// => O(NlogN) due to the sorting


int main() {
	std::vector<Names> data;
	fillNames(data);
	printFullName(data);
	return 0;
}
