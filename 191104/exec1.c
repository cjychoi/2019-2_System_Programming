/* exec1.c - shows how easy it is for a program to run a program
*/
#include <stdio.h>
#include <unistd.h>

void main()
{
	char* arglist[3];
	arglist[0] = "ls";		// first string is the name of the program
	arglist[1] = "-l";
	arglist[2] = 0;		// last string must have a null pointer
	printf("* * * About to exec ls -l\n");
	execvp("ls", arglist);
	printf("* * * ls is done. bye\n");
}