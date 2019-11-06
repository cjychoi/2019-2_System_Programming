#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//int mkdir(const char *pathname, mode_t mode);

int main()
{
	char path[1024];
	int end = 0;
	int result = 0;

	write(1, "input: ", 7);
	read(0, path, 1024);

	while (path[end] != '\n')
		end++;
	path[end] = 0;

	result = mkdir(path, 0755);
	if (result < 0)
		write(1, "failed\n", 7);
	else
		write(1, "success\n", 8);

	return 0;
}