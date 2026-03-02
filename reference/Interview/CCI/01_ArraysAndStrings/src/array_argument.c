// There are three ways to pass a 2D array to a function:

// 1, The parameter is a 2D array
int array[10][10];
void passFunc(int a[][10])
{
    // ...
}
passFunc(array);

//2, The parameter is an array containing pointers
int *array[10];
for(i = 0; i < 10; i++)
    array[i] = new int[10];
void passFunc(int *a[10]) //array containing pointers
{
    // ...
}
passFunc(array); 

//3, The parameter is a pointer to a pointer
int **array;
array = new int *[10];
for(i = 0; i <10; i++)
    array[i] = new int[10];
void passFunc(int **a)
{
    // ...
}
passFunc(array); 

