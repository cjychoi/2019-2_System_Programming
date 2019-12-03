#include <stdio.h>
#include <unistd.h>
#define NUM 5

void print_msg(char*);

void main()
{
	print_msg("hello");
	print_msg("world\n");
}

print_msg(char* m)
{
	for (int i = 0; i < NUM;  i++) {
		printf("%s", m);
		fflush(stdout);
		sleep(1);
	}
}