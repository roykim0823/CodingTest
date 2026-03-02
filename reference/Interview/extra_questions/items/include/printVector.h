#include <iostream>
#include <vector>

template <typename T>
void print(std::vector<T> vec) {
	for(auto iter = vec.begin(); iter !=vec.end(); ++iter) {
		std::cout << *iter << " ";
	}
	std::cout << std::endl;
}
