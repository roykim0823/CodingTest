// Q. Describe an algorithm to find the smallest one million numbers in one billion numbers.
//    Assume that the computer memory can hold all one billion numbers.

/* Solutions:
1. Sorting: O(nlogn)
2. Min Heap: O(nlogm): m is the number of values we are looking for
3. Selection Rank Algorithm: Using QuickSort (if you can modify the original array)-> O(n)
	Selection Rank is a well-known algorithm in computer science to find the ith smallest (or largest)
	element in an array in liner time.
	If the element are "unique", you can find the ith smallest element in expected O(n) time.
	1. Pick a random element in the array and use it as a "pivot." Partition elements around the pivot,
	   keeping track of the number of elements on the left side of the partition.
	2. If there are exactly i elements on the left, then you just return the biggest element on the left
	3. If the left side is bigger than i, repeat the algorithm on just the left part of the array.
	4. If the left side is smaller than i, repeat the algorithm on the right, but look for the element
	   with rank i-leftSize.
*/
	
int partition(int *array, int left, int right, int pivot) {
	while(true) {
		while(left<=right && array[left]<=pivot) {
			left++;
		}

		while(left<=right && array[right]>pivot) {
			right--;
		}

		if(left>right)
			return left -1;

		swap(array, left, right);
	}
}

int rank(int *array, int left, int right, int rank) {
	int pivot = array[random(left, right)];

	// partition and return end of left partition
	int leftEnd = partition(array, left, right, pivot);

	int leftSize = leftEnd - left +1;
	if(leftSize == rank+1) { 	// leftSize is same as rank (rank+1)
		return max(array, left, leftEnd);
	} else if (rank < leftSize) {
		return rank(array, left, leftEnd, rank);
	} else {
		return rank(array, leftEnd+1, right, rank-leftSize);
	}
}
