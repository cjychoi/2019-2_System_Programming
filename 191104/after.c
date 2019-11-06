#include <stdio.h>
#include <unistd.h>

void main()
{
	pid_t pid = getpid();
	printf("After execvp(): %d\n", pid);
	return;
}