#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#define INPUTLEN 100

int main(int ac, char *av[])
{
	void inthandler(int);
	void quithandler(int);
	char input[INPUTLEN];
	int nchars;

	/* set handler */
	signal(SIGINT, inthandler);
	signal(SIGQUIT, quithandler);

	do {
		printf("\nType a message\n");
		nchars = read(0, input, (INPUTLEN - 1));
		if (nchars == -1)
			perror("read returned an error");
		else {
			input[nchars] = '\0';
			printf("You typed: %s", input);
		}
	} while (strncmp(input, "quit", 4) != 0);

	return 0;
}

void inthandler(int s)
{
	// static int num = 0;
	//  signal(SIGINT, inthandler);
	// printf("inthandler %d\n", ++num);

	printf("  Received signal %d .. waiting\n", s);
	sleep(2);
	printf("   Leaving inthandler \n");
}

void quithandler(int s)
{
	printf("  Received signal %d .. waiting\n", s);
	sleep(3);
	printf("  Leaving quithandler\n ");
}