#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <pthread.h>

#define MAXTHREAD 8
#define oops(m, x) {perror(m), exit(x);}

/* structure for passing multiple arguments to thread*/
struct thread_data
{
	char* buff;						/* memory buffer pointer */
	long long start;			   	   /* start index in the buffer */
	long long block_size;	   /* block size */
	long long counter;          /* counter variable */
};

/* global variables */
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // lock
pthread_cond_t flag = PTHREAD_COND_INITIALIZER;
long long total_words;
long long sum = 0;

void* count_words(void*);
void* accm_num(void*);

int main(int argc, char** argv)
{
	FILE* fp;
	int nthreads = 2;
	int x, id;
	long long block_size, len;
	int i;
	struct thread_data data1[MAXTHREAD], data2[MAXTHREAD];
	pthread_t *threads1, *threads2;
	int child1, child2;
	int pipefd1[2], pipefd2[2];

	/* handle the program arguments */
	if (argc < 2)
	{
		fprintf(stderr, "Usage: ./program_name <file_path>");
		exit(-1);
	}

	/* open file */
	if ((fp = fopen(argv[1], "r")) == NULL)
	{
		printf("Error opening file!\n");
		exit(-1);
	}

	threads1 = malloc(nthreads * sizeof(pthread_t));
	threads2 = malloc(nthreads * sizeof(pthread_t));

	// pthread_mutex_lock(&lock);

	/* check the file size*/
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);

	/* calculate size of the block that will be assigned to each thread */
	block_size = len / nthreads;
	fseek(fp, SEEK_SET, 0);


	/* copy the contents of file to a memory buffer */
	char* buff = (char*)malloc(sizeof(char) * len);
	fread(buff, 1, len, fp);


	/* store the block information in the argument structure */
	for (i = 0; i < nthreads; i++)
	{
		data1[i].buff = buff;
		data1[i].start = block_size * i;
		data1[i].block_size = block_size;
		data1[i].counter = 0;
	}
	for (i = 0; i < nthreads; i++)
	{
		data2[i].buff = buff;
		data2[i].start = block_size * i;
		data2[i].block_size = block_size;
		data2[i].counter = 0;
	}



	/* record the current time */
	struct timeval start_time, end_time;
	gettimeofday(&start_time, NULL);

	if (pipe(pipefd1) == -1)
		oops("cannot get a pipe", 1);	
	if (pipe(pipefd2) == -1)
		oops("cannot get a pipe", 2);

	// pthread_cond_wait(&flag, &lock);


	if ((child1 = fork()) == 0) {	// first child for counting words
			/* create threads and wait until all threads complete their task */
		for (i = 0; i < nthreads; i++)
			pthread_create(&threads1[i], NULL, count_words, (void*)&data1[i]);

		for (i = 0; i < nthreads; i++) {
			pthread_join(threads1[i], NULL);
		}
		write(pipefd1[1], count_words, sizeof(count_words));

	}

	else {
		if ((child2 = fork()) == 0) {	// second child for accumulate number
			for (i = 0; i < nthreads; i++)
				pthread_create(&threads2[i], NULL, accm_num, (void*)&data2[i]);

			for (i = 0; i < nthreads; i++) {
				pthread_join(threads2[i], NULL);
			}
			write(pipefd2[1], sum, sizeof(sum));

		}

		else {		// parent process
			//pthread_cond_signal(&flag);

			//printf("parent\n");
	


		}
	}

	// pthread_cond_signal(&flag);

	free(threads1);
	free(threads2);
	fclose(fp);
	free(buff);

	/* calculate the elapsed time */
	gettimeofday(&end_time, NULL);
	double operating_time = (double)(end_time.tv_sec) + (double)(end_time.tv_usec) / 1000000.0 - (double)(start_time.tv_sec) - (double)(start_time.tv_usec) / 1000000.0;

	/* print the total number of words in the file */
	printf("\nElapsed time: %f seconds\n", (double)operating_time);
	printf("total number of words: %lld\n", total_words);
	printf("accumulated number: %lld\n", sum);

	
}

void* count_words(void* a)
{
	struct thread_data* args = (struct thread_data*)a;
	int prevc = '\0';

	for (int i = args->start; i < (args->start + args->block_size); i++) {
		if (!isalnum(args->buff[i]) && isalnum(prevc))
			args->counter++;
		prevc = args->buff[i];
	}

	pthread_mutex_lock(&lock);
	total_words += args->counter;
	pthread_mutex_unlock(&lock);

	// test
	//printf("[THREAD]  the number of words : %lld\n", (args->counter));
}

void* accm_num(void* a)
{
	struct thread_data* args = (struct thread_data*)a;

	for (int i = args->start; i < (args->start + args->block_size); i++) {
		if (args->buff[i] >= '0' && args->buff[i] <= '9')
			args->counter += atoi(args->buff[i]);
	}

	pthread_mutex_lock(&lock);
	sum += args->counter;
	pthread_mutex_unlock(&lock);

	// test
	//printf("[THREAD]  accumulated number : %lld\n", (args->counter));
}