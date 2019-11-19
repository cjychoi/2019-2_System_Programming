#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include<signal.h>

void makeFile(int ary[], int num);
void ascending_sort(int ary[]);
void descending_sort(int ary[]);

int main(int argc, char* argv[])
{
	struct timeval start_time, end_time;
	int pid1, pid2;
	unsigned array[20000];
	srand(time(NULL));
	for (unsigned int i = 0; i < 20000; i++)
		array[i] = rand() % 20000;

	makeFile(array, 1);

	gettimeofday(&start_time, NULL);

	/* execute the two tasks in parallel using fork() */
	if ((strcmp(argv[1], "0")) == 0) {
		if ( (pid1 = fork()) == -1)
			perror("fork");
		else if (pid1 == 0) {
			if ((pid2 = fork()) == -1)
				perror("fork");
			else if (pid2 == 0) {
				ascending_sort(array);
				exit(1);
			}
			else {
				descending_sort(array);
				wait(NULL);
				exit(1);
			}
		}
		else {
			wait(NULL);
			printf("finished\n");
		}
	}

	/* execute the two tasks in sequential */
	else {
		ascending_sort(array);
		descending_sort(array);

	}
	
	gettimeofday(&end_time, NULL);
	double operating_time =
		(double)(end_time.tv_sec) + (double)(end_time.tv_usec) / 1000000.0 -
		(double)(start_time.tv_sec) - (double)(start_time.tv_usec) / 1000000.0;

	printf("Elapsed: %f seconds\n", (double)operating_time);
	
}


void ascending_sort(int ary[])
{
	int temp;

	for (int i = 0; i < 20000; i++) {
		for (int j = 0; j < 20000; j++) {
			if (ary[i] < ary[j]) {
				temp = ary[i];
				ary[i] = ary[j];
				ary[j] = temp;
			}
		}
	}

	makeFile(ary, 2);
}

void descending_sort(int ary[])
{
	int temp;

	for (int i = 0; i < 20000; i++) {
		for (int j = 0; j < 20000; j++) {
			if (ary[i] > ary[j]) {
				temp = ary[i];
				ary[i] = ary[j];
				ary[j] = temp;
			}
		}
	}

	makeFile(ary, 3);
}

void makeFile(int ary[], int num)
{

	

	if (num == 1) {	// save randomly generated numbers
		FILE* fp1 = fopen("file1.txt", "w");
		for (int i = 0; i < 20000; i++)
			fprintf(fp1, "%d\n", ary[i]);
		fclose(fp1);
	}

	else if(num == 2) {	// save ascending-sorted numbers
		FILE* fp2 = fopen("file2.txt", "w");
		for (int i = 0; i < 20000; i++)
			fprintf(fp2, "%d\n", ary[i]);
		fclose(fp2);

	}

	else if (num == 3) {	// save descending-sorted numbers
		FILE* fp3 = fopen("file3.txt", "w");
		for (int i = 0; i < 20000; i++)
			fprintf(fp3, "%d\n", ary[i]);
		fclose(fp3);

	}

	

}