#include <unistd.h>	// for pause()
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>	// for exit()

int set_ticker(int);
void countdown(int);

int main()
{
	signal(SIGALRM, countdown);
	if (set_ticker(500) == -1)
		perror("set_ticker");
	else
		while (1)
			pause();
	return 0;
}

void countdown(int signum)
{
	static int num = 10;
	printf("%d ..", num--);
	fflush(stdout);
	if (num < 0) {
		printf("DONE!\n");
		exit(0);
	}
}

int set_ticker(int n_msecs)
/* set_ticker( number_of_milliseconds
* arranges for interval timer to issue SIGALRM's at regular intervals
* returns -1 on error, 0 for ok
* arg in milliseconds, converted into whole seconds and microseconds
* note: set_ticker(0) trns off ticker
*/
{
	struct itimerval new_timeset;
	long n_sec, n_usecs;

	n_sec = n_msecs / 1000;						// int part
	n_usecs = (n_msecs % 1000) * 1000L;	// remainder

	new_timeset.it_interval.tv_sec	= n_sec;		// set reload	
	new_timeset.it_interval.tv_usec = n_usecs;	// new ticker value
	new_timeset.it_value.tv_sec		= n_sec;		// store this
	new_timeset.it_value.tv_usec	= n_usecs;	// and this

	return setitimer(ITIMER_PROF, &new_timeset, NULL);
}