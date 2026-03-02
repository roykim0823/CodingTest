Testing a Function:
The testing is usually limited to validating input and output.
You should discuss any assumptions with your interviewer, 
particularly with respect to how to handle specific situations.

Test: sort(int[] array), which sorts an array of integers
Testing Steps:
1.	Define the test case
	a. Normal case: Does it generate the correct output for typical inputs? 
	   Remember to think about potential issues here. 
	   e.g.,  Sorting with a partition -> check it works with both even and odd number of element
	b. Extreme case
	   e.g., an empty array, a very small (one element), or a very large array
	c. Nulls and “illegal’ input: negative array length argument
	d. Strange input: already sorted array, a sorted array in reverse order

2.	Define the expected result: It is obviously the right output. However, in some cases, 
    you might want to validate additional aspects. Ex) If the sort method returns a new sorted copy of 
	the array, you should probably validate that the original array has not been touched.
3.	Write test code.
