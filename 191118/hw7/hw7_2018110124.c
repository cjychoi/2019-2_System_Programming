/* SYSPRG hw7 - 2018110124 최준영
	<Multiple Pipes>
	program should work as the sequence of multiple commands
	with the “|” operator.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define oops(m,x) { perror(m); exit(x); }

void doPipe(char**, int, int);

int main(int ac, char* av[]) {

	if (ac < 1) {
		fprintf(stderr, "usage : pipe cmd1 cmd2 ... NULL\n");
		exit(1);
	}

	doPipe(av, 1, 0);
}

void doPipe(char* av[], int n, int pid)
{
	int thepipe[2],		// two file descriptors
		newfd,				// useful for pipes
		retval;

	/* Todo */
	/* wait for last child process */
	if (!strcmp(av[n], "NULL")) {	// if last command
		waitpid(pid, &retval, 0);			// first operand is pid, not NULL
		return;
	}

	/* get a pipe */
	if (pipe(thepipe) == -1)
		oops("Cannot get a pipe", 1);

	/* ------------------------------------------------------------ */
	/* now we have a pipe, now let's get two processes*/
	if ((pid = fork()) == -1)
		oops("Cannot fork", 2);

	/* ------------------------------------------------------------ */
	/* Right here, there are two processes */
	if (pid > 0) {					// parent will exec av[2]
		close(thepipe[1]);      // parent doesn't write to pipe

		if (dup2(thepipe[0], 0) == -1)      //read를 0으로 
			oops("could not redirect stdin", 3);

		close(thepipe[0]);

		/* Todo */
		/* Iterative */
		doPipe(av, n + 1, pid);
		oops(av[n], 4);
	}

	/* Todo */
	/* Child Process */
	if (!strcmp(av[n + 1], "NULL")) {   //다음 명령어가 NULL이면
		execlp(av[n], av[n], NULL);      //stdout으로 바로출력
		oops(av[n], 5);
	}

	/* child execs av[1] and writes into pipe */
	close(thepipe[0]);		// child doesn't read from pipe
	if (dup2(thepipe[1], 1) == -1)      // stdout->pipe
		oops("could not redirect stdout", 4);

	close(thepipe[1]);
	execlp(av[n], av[n], NULL);      // Execute
	oops(av[1], 6);
}