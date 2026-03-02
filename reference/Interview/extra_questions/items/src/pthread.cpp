/*
Write a multi threaded C code with one thread printing all even numbers and the other all odd numbers. The output should always be in sequence 
ie. 0,1,2,3,4....etc
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

bool printOddNum;
int amountOfNums = 30;
pthread_mutex_t theMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv;

void* printEven(void* vargp)
{
	for (int i = 1; i < amountOfNums / 2; i++)
	{
		pthread_mutex_lock(&theMutex);
		
		while (printOddNum)
		{
			pthread_cond_wait(&cv, &theMutex);
		}

		printf("%d\n", 2 * i);

		printOddNum = true;

		pthread_cond_signal(&cv);
		pthread_mutex_unlock(&theMutex);
	}
	
	return NULL;
}

void* printOdd(void* vargp)
{
	for(int i = 1; i < amountOfNums / 2; i++)
	{
		pthread_mutex_lock(&theMutex);

		while (!printOddNum)
		{
			pthread_cond_wait(&cv, &theMutex);
		}

		printf("%d\n", 2 * i - 1);

		printOddNum = false;

		pthread_cond_signal(&cv);
		pthread_mutex_unlock(&theMutex);
	}
	
	return NULL;
}

int main()
{
	pthread_t evenThreadID;
	pthread_t oddThreadID;
	printOddNum = true;

	pthread_mutex_init(&theMutex, NULL);
	pthread_cond_init(&cv, NULL);

	pthread_create(&oddThreadID, NULL, printOdd, NULL);
	pthread_create(&evenThreadID, NULL, printEven, NULL);
	
	pthread_join(evenThreadID, NULL);
	pthread_join(oddThreadID, NULL);

	pthread_mutex_destroy(&theMutex);
	pthread_cond_destroy(&cv);

	system("pause");
	return 0;
}
