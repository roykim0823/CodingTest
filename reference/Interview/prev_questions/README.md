# Constructing a bianry tree from traversal sequences
Given two binary tree traversal sequences, construct a binary tree if
1. Inorder and Preorder
2. Inorder and Postorder
3. Inorder and level-order

A binary tree cannot be constructed with other combinations.
ex) even if three of them (preorder, postorder, and level-order) 
    are given, the gree cannot be constructed

# Construct Tree from given Inorder and Preorder traversals
Let us consider the below traversals:

Inorder sequence: 	D B E A F C
Preorder sequence: 	A B D E C F
Postorder sequence: D E B F C A

In a Preorder sequence, leftmost element is the root of the tree. 
So we know ‘A’ is root for given sequences. 
By searching ‘A’ in Inorder sequence, 
we can find out all elements on left side of ‘A’ are 
in left subtree and elements on right are in right subtree. 
So we know below structure now.

```
				      A
	                /   \
   	               /     \
		      D B E      F C
```

We recursively follow above steps and get the following tree.

```
					  A
  			        /   \
  			       /     \
  			      B       C
	       	     / \     /
	            /   \   /
  			   D    E   F
```
Algorithm: buildTree()
1. Pick an element from Preorder. Increment a Preorder Index Variable 
   (preIndex in below code) to pick next element in next recursive call.
2. Create a new tree node tNode with the data as picked element.
3. Find the picked element’s index in Inorder. Let the index be inIndex.
4. Call buildTree for elements before inIndex and make the built tree as left subtree of tNode.
5. Call buildTree for elements after inIndex and make the built tree as right subtree of tNode.
6. return tNode.

# Construct Tree from given Inorder and Postorder traversals
Examples:

Input : 
in[]   = {2, 1, 3}
post[] = {2, 3, 1}

Output : Root of below tree
      1
    /   \
   2     3 


Input : 
in[]   = {4, 8, 2, 5, 1, 6, 3, 7}
post[] = {8, 4, 5, 2, 6, 7, 3, 1} 

Output : Root of below tree
          1
       /     \
     2        3
   /    \   /   \
  4     5   6    7
    \
      8


We have already discussed construction of tree from Inorder and Preorder traversals. The idea is similar.

Let us see the process of constructing tree from in[] = {4, 8, 2, 5, 1, 6, 3, 7} and post[] = {8, 4, 5, 2, 6, 7, 3, 1}

1) We first find the last node in post[]. The last node is “1”, we know this value is root as root always appear in the end of postorder traversal.

2) We search “1” in in[] to find left and right subtrees of root. Everything on left of “1” in in[] is in left subtree and everything on right is in right subtree.

         1
       /    \
[4, 8, 2, 5]   [6, 3, 7] 
3) We recur the above process for following two.
….b) Recur for in[] = {6, 3, 7} and post[] = {6, 7, 3}
…….Make the created tree as right child of root.
….a) Recur for in[] = {4, 8, 2, 5} and post[] = {8, 4, 5, 2}.
…….Make the created tree as left child of root.

Below is the implementation of above idea. One important observation is, we recursively call for right subtree before left subtree as we decrease index of postorder index whenever we create a new node.


