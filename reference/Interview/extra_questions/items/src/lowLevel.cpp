//=======================================================================
//Find a duplicate number in a given array. The numbers are  1 ... n
void printRepeating(int arr[], int size)
{
  int i;
  printf("The repeating elements are: \n");
  for (i = 0; i < size; i++)
  {
    if (arr[abs(arr[i])] >= 0) // accessed changed the sign
      arr[abs(arr[i])] = -arr[abs(arr[i])];
    else // if it is already changed the sign
      printf(" %d ", abs(arr[i]));
  }
}
//============================================================
//implement strlen function
unsigned short strlen4(char *str)
{
    for (int i = 0; ; i++) if (str[i] == '\0') return i;
}
//===========================================
//find n-th element from end of a singly linked list 
//- solution, used circular buffer.
//====================================

//Write implementation of strcpy method.  
//str copy function self made
char *strcpynew(char *d, char *s){
   char *saved = d;
   while ((*d++ = *s++) != '\0');

   return saved; //returning starting address of s1
}

//===============================
//If there're n points, how many distinct lines can you draw
//n choose 2
 
/* ====================================================================================
A producer continuously produces a stream of characters. There are multiple consumer threads which read the chars and match to one of known strings and increment the counter for that pattern. Write the consumer function. Show how you will synchronize and maintain parallelism. 

Ex: Producer: abcdegabccaabbaacv ...... 
Known strings[] = {"ab", "aa", "fff" ... } 
patternMatchCount[] = {3, 2, 0 ... }

A.
strings = { "ab" : 0 , "aa" : 1 , "fff" : 2 , ... }
counters = [ 3, 2, 0, ...]

def increment_counter( s ){
   idx = strings[ s ]
   #atomic{
      // make atomic operation : increment 
      counters[idx] += 1
   }
}
*/
//===============================================================================	
//Write a function in C to create a new BST which is the mirror image of a given tree.

node* mirrorBST(node *bst1 , node *bst2) 
{ 
	if(bst1 != NULL) 
	{ 
		bst2 = (node*)malloc(sizeof(node)); 
		bst2->val = bst1->val; 
		bst2->left = NULL; 
		bst2->right = NULL; 
		//printf("v = %d\n",root2->val ); 
		
		bst2->right = mirrorBST(bst1->left,bst2->right); 
		bst2->left = mirrorBST(bst1->right,bst2->left); 
		return bst2; 
	} else 	{ 
		return NULL; 
	} 
}
/*
------------------------------------------------------------------------------------
Q. Consider the statement 
result = a ? b : c; 
Implement the above statement without using any conditional statements.


!a -> 0 if a!=0
!a -> 1 if a==0

!!a -> 1 if a!=0, 
!!a -> 0 if a==0

if a b c and result are int
*/
int conditional(int b, int c) {
	int temp[]= { b, c}
	result = temp[!a];
	return result;
}

//======================
/* angle between hands of the clock.. the interviewer was expecting me to have a formula memorized. i told him i dont know any formula so i derived an equation which worked. he was still very skeptical abt it, until i did some testing n showed him it works.

abs(h*(360/12)+(m/60)*(360/12) - m*6)
*/
