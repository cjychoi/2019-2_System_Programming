#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main(void)
{

	signal(SIGINT, SIG_IGN);
	printf("you can't stop me!\n");

	while (1) {
		sleep(1);
		printf("haha\n");
	}

	return 0;
}
