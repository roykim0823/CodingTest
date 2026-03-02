#include <iostream>
#include <memory>

template <typename T>
struct btNode { 
	T data; 
	std::unique_ptr<btNode<T>> l, r;
};

template <typename T>
std::unique_ptr<btNode<T>> new_bt_Node(T data) {
	auto node = std::unique_ptr<btNode<T>>(new btNode<T>{data, nullptr, nullptr});
	return node;
}

// return the index of value in arr
template <typename T>
int search(T arr[], int strt, int end, T value) {
	for(int i=strt; i<=end; i++) {
		if(arr[i] == value)
			return i;
	}
	return -1;	// not found
}
	
template <typename T>
void printInorder(std::unique_ptr<btNode<T>>&node) 
{ 
	if (node == nullptr) 
		return; 
	printInorder(node->l); 
	std::cout << node->data << " ";
	printInorder(node->r); 
}

template <typename T>
void printPreorder(std::unique_ptr<btNode<T>>&node) 
{ 
	if (node == nullptr) 
		return; 
	std::cout << node->data << " ";
	printPreorder(node->l); 
	printPreorder(node->r); 
}

template <typename T>
void printPostorder(std::unique_ptr<btNode<T>>&node) 
{ 
	if (node == nullptr) 
		return; 
	printPostorder(node->l); 
	printPostorder(node->r); 
	std::cout << node->data << " ";
}
