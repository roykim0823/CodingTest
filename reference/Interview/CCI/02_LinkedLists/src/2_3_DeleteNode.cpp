// Q. Implement an algorithm to delete a node in the middle of singly linked list, 
//    given only access to that node

// Solutions:
// 1. Copy the data from the next node over to the current node
// 2. Link the next node's next node to current node's next 
// 3. then delete the next node
// * This doesn't work at the last node
// -> Sol: add dummy node at the end

bool deleteNode(node *n) {
	if(n==NULL || n->next==NULL) 
		return false;
	
	node *nextNode = n->next;
	n->data = nextNode->data;
	n->next = nextNode->next;
	delete nextNode;
	return true;
}

int main() {}
