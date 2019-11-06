#include <stdio.h>
#include <unistd.h>

void main()
{
	char* arg[2];
	arg[0] = "./after";
	arg[1] = 0;
	
	pid_t pid = getpid();
	printf("Before execvp(): %d\n", pid);
	execvp(arg[0], arg);

	return;
}