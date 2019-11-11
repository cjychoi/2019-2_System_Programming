#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

void main()
{
	int fd;
	char line[100];

	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);

	close(0);
	fd = open("/etc/passwd", O_RDONLY);
	if (fd != 0) {
		fprintf(stderr, "Could not open data as fd()\n");
		exit(1);
	}
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);
}