/* inprint.c - one thread incremets, the other prints */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#define NUM 5

int counter = 0;
void* print_count(void*);		//its function

void main()
{
	pthread_t t1;
	int i;

	//create a thread
	pthread_create(&t1, NULL, print_count, NULL);
	for (i = 0; i < NUM; i++) {
		counter++;
		sleep(1);
	}
	// wait for a thread to be completed
	pthread_join(t1, NULL);
	return 0;
}

void* print_count(void* m)
{
	int i;
	for (i = 0; i < NUM; i++) {
		printf("count = %d\n", counter);
		sleep(2);
	}
	return NULL;
}