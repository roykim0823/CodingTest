# Good Sorting Algorithm
  - What is the most efficient sorting algorithm for each of the following situations
1. A large array whose entries are random numbers.
	- In general, quicksort is considered one of the most efficient sorting algorithms since it has 
	  an average case run time of O(nlogn) and it sorts in-place. For a large set of random integers,
	  quicksort would be the choice.
2. A small array of numbers.
 	- a simple implementataion such as insertion sort is easier to code, and run faster for a small arry
	  in practice
3. A large array of numbers that is already almost sorted.
 	- If every element is known to be at most k places from its final location, a min-heap can be used 
	  to get get an O(nlogk) algorithm.
4. A large collection of integers that are drawn from a small range.
   	- counter sort, or BST where the keys are the numbers and the values are their frequencies.
5. A large collection of numbers most of which are duplicates.
 	- BST then perform in-order traverse
6. Stability is required, i.e., the relative order of two records that
   have the same sorting key should not be changed.
    - Most sorting algorithms are not stable. Mergesort can be made stable.
	  Another solution is to add the index as an integer rank ot the keys to break ties.
