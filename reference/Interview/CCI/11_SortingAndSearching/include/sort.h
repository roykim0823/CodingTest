// Basic Sort algorithm

void exch(int a[], int i, int j) 
{
	int tmp = a[i];
	a[i]=a[j];
	a[j]=a[i];
}

// Selection Sort, not stable
// Find the ith smallest item, then exchange it with the ith entry
// Time: O(N^2) = (N^2)/2 even if the input is sorted
// Space: In place
// Data movement is minimal: linear number of exchange
void selectionSort(int a[], int length) 
{
	for(int i=0; i<length; i++)
	{
		int min=0;
		for(int j=i+1; j<length; j++)
			if(a[j] < a[min])
				min=j;
		//Exchange the min with ith element
		exch(a, i, min);
	}
}

// Insertion sort <-> Bubble sort, stable
// the times to the left of the current index are in sorted,
// So, compare the current index to the left, and move it proper position
// * It is much faster if partial array is alread sorted
// Time: O(N^2) = (N^2)/4 (Average), (N^2)/2 (Worst, des order), N-1 (Best, asc order:q)
// Space: In place
void InsertionSort(int a[], int length)
{
	for(int i=1; i<length; i++) {
		for(int j=i; j>0 && (a[j] < a[j-1]); j--)
			exch(a, j, j-1);
	}
}

// Merge Sort, stable
// Divide and Conquer
// Time: O(NlogN)
// Space: O(N) due to use auxiliary array
// *Stability: a stable sort preserves the relative order of items with equal keys
// stable if equal iterms never move past each other
// not stable due to long distance exchange might move an item past some equal item

void merge(int a[], int aux[], int low, int mid, int hi)
{
	// Copy the array to the auxiliary array
	for(int i=low; i<=hi; i++)
		aux[i] = a[i];
	
	int i=low, j=mid+1;
	for(int k=low; k<=hi; k++) {
		if	(i > mid)		a[k] = aux[j++];
		else if(j > hi)		a[k] = aux[i++];
		else if(aux[j] < aux[i]) 	a[k] = aux[j++];
		else						a[k] = aux[i++];
	}
}

void MergeSort(int a[], int aux[], int low, int hi)
{
	if(hi <= low) return;
	int mid = low + (hi-low) /2;
	MergeSort(a, aux, low, mid);
	MergeSort(a, aux, mid+1, hi);
	// stop merge if already sorted: Optimization_1
	if(a[mid] <= a[mid+1]) return;
	merge(a, aux, low, mid, hi);
}

// Quick Sort, not stable
// Shuffle -> Partition -> Sort each piece
// O(NlogN): best case, Worst Case(N^2): bad parition(shuffle) required
// Faster than mergesort in practice due to less data movement

int partition(int a[], int low, int hi)
{
	int i=low, j=hi+1;
	while(true) {
		while(a[++i] < a[low]) 	// Find item on left to swap
			if(i==hi) break;
		while(a[low] < a[--j])	// find item on right to swap
			if(j==low) break;

		if(i >= j) break;	// Check if pointers cross 
		exch(a, i, j);      // swap
	}

	exch(a, low, j);	// swp with partitioning item
	return j;
}

void quicksort(int a[], int low, int hi);
void QuickSort(int a[], int length)
{
	shuffle(a);
	quicksort(a, 0, length-1);
}

void quicksort(int a[], int low, int hi)
{
	if(hi<=low) return;
	int j=partition(a, low, hi);
	sort(a, low, j-1);
	sort(a, j+1, hi);
}
