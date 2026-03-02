// Q. Convert Binary Tree into a double linked list

/* Checks:
Let's approach this recursively, starting with the root
Since the left and right halves of the tree form their own "sub-parts" of the linked list 
(that is, they appear consecutively in the linked list). So, if we recursively converted the left
and right subtrees to a doubly linked list, we could build the final linked list from those parts.

// Pseudocode
BiNode convert(BiNode node) {
	BiNode left = convert(node.left);
	BiNode right = convert(node.right);
	mergeLists(left, node, right);
	return left;	// front of left
}
To actually implement the nitty-gritty details of this, we'll need to get the head and tail
of each linked list. We can do this several different ways.
*/

#include <iostream>
#include <algorithm>
using namespace std;

struct node {
	int data;
	node *left;
	node *right;
};

/*
1. if a left subtree exists, process the left subtree
	a. Recursively convert the left subtree to DLL
	b. Then find inorder predecessor of root in the left subtree
	   -> Inorder predecessor is rightmost node in the left subtree
	c. Make inorder predecessor as previous of root and next of inorder predecessor.
2. if a right subtree exists, process the right subtree similar to the left subtree
3. Find the leftmost node and return it
   -> The leftmost node is always head of converted DLL.
*/
node* binaryTree2DLL(node *root)
{
	// base case
	if(root == NULL) 
		return root;
	
	// Convert the left subtree and link to root
	if(root->left!=NULL) {
		// Convert the left subtree
		node *left = binaryTree2DLL(root->left);

		// Find inorder predecessor. After this loop, left
		// will point to the inorder predecessor
		for(; left->right!=NULL; left=left->right);

		// Make root as next of the predecessor
		left->right=root;

		// Make predecessor as previous of root
		root->left = left;
	}

	// Convert the right subtree and link to root
	if(root->right!=NULL) {
		// Convert the right subtree
		node *right = binaryTree2DLL(root->right);

		// Find inorder successor. After this loop, right
		// will point to the inorder successor
		for(; right->left!=NULL; right= right->left);

		// Make root as previous of successor
		right->left = root;

		// Make successor as next of root
		root->right=right;
	}
	return root;
}

// main function that first calls binaryTree2DLL(), then follows step 3
// of the above algorithm
node *convert(node *root)
{
	// Base case
	if(root==NULL)
		return root;

	// Convert to DLL using binaryTree2DLL()
	root = binaryTree2DLL(root);

	// binaryTree2DLL() returns root node of the converted DLL.
	// We need pointer to the leftmost node which is head of the 
	// constructed DLL, so move to the leftmost node
	while(root->left!=NULL)
		root= root->left;
	
	return root;
}

// Helper function that allocates a new node with the given data
// and NULL left and right pointers
node *newNode(int data) 
{
	node *new_node = new node;
	new_node->data = data;
	new_node->left = new_node->right=NULL;
	return new_node;
}

// Function to print nodes in a given doubly linked list
void printList(node *node) 
{
	while(node!=NULL) {
		cout << node->data << " ";
		node= node->right;
	}
	cout << endl;
}

// Driver program to test above functions
int main()
{
	node *root 		= newNode(10);
	root->left		= newNode(12);
	root->right		= newNode(15);
	root->left->left  	= newNode(25);
	root->left->right  	= newNode(30);
	root->right->left  	= newNode(36);

	node *head = convert(root);

	printList(head);

	return 0;
}
