#include "bTree.h"
#include <unordered_map>
#include <stack>
#include <set>

/* Recursive function to construct binary of size len from 
   Inorder traversal in[] and Preorder traversal pre[]. 
   Initially: inBeg=0, inEnd=len-1
   T= O(n^2), worst case occurs when tree is left skewed
   worst case preOrder = {A, B, C, D}, inOrder {D, C, B, A}
*/
template <typename T>
std::unique_ptr<btNode<T>> buildTree(T in[], T aux[], int inStrt, int inEnd,  
			int &pIndex, std::unordered_map<T, int> &hash, bool preOrder=true) // pIndex value must be maintained during recursive calls
{ 

	if (inStrt > inEnd) 
		return nullptr; 

	int curr;
	if(preOrder)
		curr = aux[pIndex++];  	// aux is preOrder
	else
		curr = aux[pIndex--];		// aux is postOrder

	auto tNode = new_bt_Node<T>(curr); 

	/* If this node has no children then return */
	if (inStrt == inEnd) 
		return tNode; 

	/* Else find the index of this node in Inorder traversal */
	int inIndex;
	if(hash.empty()) {
		inIndex = search<T>(in, inStrt, inEnd, tNode->data);  	// Search time: O(n)
	} else {
		inIndex = hash[curr];                                  	// Search time: O(1)
	}

	/* Using index in Inorder traversal, construct left and right subtress */
	if(preOrder) {
		tNode->l = buildTree(in, aux, inStrt, inIndex - 1, pIndex, hash, preOrder); 
		tNode->r = buildTree(in, aux, inIndex + 1,  inEnd, pIndex, hash, preOrder); 
	} else {
		tNode->r = buildTree(in, aux, inIndex + 1,  inEnd, pIndex, hash, preOrder); 
		tNode->l = buildTree(in, aux, inStrt, inIndex - 1, pIndex, hash, preOrder); 
	}
	return tNode; 
}

// Efficient Approach with hashing
// hash stores indexes of inorder traversal
// search can be done O(1) time
template <typename T> 
std::unordered_map<T, int> buildHash(T in[], int len) {
	std::unordered_map<T, int> hash;
	for(int i=0; i<len; i++) 
		hash[in[i]] = i;
	return hash;
}

// Wrapper function to cover preOrder and postOrder traversal w/o hash
template <typename T>
std::unique_ptr<btNode<T>> constructTree(T in[], T aux[], int n, bool preOrder=true, bool useHash=false) {
    std::unordered_map<T, int> hash;
	
	if(useHash==true)
		hash = buildHash<T>(in, n);
	int pIndex=0;
	if(preOrder==false)	 // use postOrder  
		pIndex=n-1;

	return buildTree(in, aux, 0, n-1, pIndex, hash, preOrder);
}

/* Driver program to test above functions */
int main() 
{ 
	// Test inOrder and preOrder traversals
	char in[] = { 'D', 'B', 'E', 'A', 'F', 'C' }; 
	char pre[] = { 'A', 'B', 'D', 'E', 'C', 'F' }; 
	int len = sizeof(in) / sizeof(in[0]); 

	/* Let us test the built tree by printing Insorder traversal */
	auto root = constructTree<char>(in, pre, len); 
	std::cout << "Inorder traversal of the constructed tree is" << std::endl; 
	printInorder<char>(root); std::cout << std::endl;
	printPreorder<char>(root); 
	std::cout << std::endl;

	auto root2 = constructTree<char>(in, pre, len, true, true);
	std::cout << "Inorder traversal of the constructed tree with hash is" << std::endl; 
	printInorder<char>(root2); std::cout << std::endl;
	printPreorder<char>(root2);
	std::cout << std::endl;

	// Test inOrder and postOrder traversals
	int in2[] = { 4, 8, 2, 5, 1, 6, 3, 7 }; 
   	int post[] = { 8, 4, 5, 2, 6, 7, 3, 1 }; 
  	int n = sizeof(in2) / sizeof(in2[0]); 
	
	auto root3 = constructTree<int>(in2, post, n, false, false); 
	/* Let us test the built tree by printing Insorder traversal */
	std::cout << "Inorder traversal of the constructed tree is" << std::endl; 
	printInorder<int>(root3); std::cout << std::endl;
	printPostorder<int>(root3); std::cout << std::endl;  
	printPreorder<int>(root3);   std::cout << std::endl;  
	std::cout << std::endl;

	auto root4 = constructTree<int>(in2, post, n, false, true);
	std::cout << "Inorder traversal of the constructed tree with hash is" << std::endl; 
	printInorder<int>(root4); std::cout << std::endl;
	printPostorder<int>(root4);  std::cout << std::endl;  
	printPreorder<int>(root4);    std::cout << std::endl;  
	std::cout << std::endl;
} 

