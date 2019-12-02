#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE* fp;
	char buf[100];
	int i = 0;

	fp = popen("who|sort", "r");		// open the command
	
	while (fgets(buf, 100, fp) != NULL)		// read from command
		printf("%3d %s", i++, buf);				// print data

	pclose(fp);
	return 0;
}