#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define INPUTLEN 100

int main()
{
	struct sigaction newhandler;
	sigset_t blocked;
	void inthandler();
	char x[INPUTLEN];

	/* load these two members first */
	newhandler.sa_handler = inthandler;	// handler function
	newhandler.sa_flags = SA_RESETHAND | SA_RESTART;	// options

	/* then build the list of blocked signals */
	sigemptyset(&blocked);				// clear all bits
	sigaddset(&blocked, SIGQUIT);		// add SIGQUIT to list
	newhandler.sa_mask = blocked;	// store blockmask

	if (sigaction(SIGINT, &newhandler, NULL) == -1)
		perror("sigaction");
	else
		while (1) {
			fgets(x, INPUTLEN, stdin);
			printf("input: %s", x);
		}

	return 0;
}

void inthandler(int s)
{
	printf("Called with signal %d\n", s);
	sleep(s);
	printf("done handling signal %d\n", s);
}