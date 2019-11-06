#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h> 
#include <sys/types.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

int make_dir(const char *pathname, mode_t mode);
void oops(char *, char *);

void main(int ac, char *av[])
{
	int in_fd, out_fd, n_chars;
	char buf[BUFFERSIZE];

	if (ac != 3) {		/* exit if input is not 3 */
		fprintf(stderr, "usage: %s source destination\n", *av);
		exit(1);
	}

	if ((in_fd = open(av[1], O_RDONLY)) == -1)
		oops("Cannot open ", av[1]);
	if ((out_fd = creat(av[2], COPYMODE)) == -1)
		oops("Cannot creat", av[2]);

	if (make_dir(out_fd, 0755) == -1)					// return word size
		oops("Write error to ", av[2]);

	/*
	while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)	// n_chars: size of word
		if (make_dir(out_fd, 0755) == -1)					// return word size
			oops("Write error to ", av[2]);
	*/

	if (n_chars == -1)
		oops("Read error from ", av[1]);

	if (close(in_fd) == -1 || close(out_fd) == -1)
		oops("Error closing files", "");



}

int make_dir(const char *pathname, mode_t mode)
{
	int ret = -1;
	char *q, *r = NULL, *path = NULL, *up = NULL;

	if (strcmp(pathname, ".") == 0 || strcmp(pathname, "/") == 0)
		return 0;
	if ((path = strdup(pathname)) == NULL)
		exit(1);
	if ((q = strdup(pathname)) == NULL)
		exit(1);
	if ((r = (char*)dirname(q)) == NULL)
		goto out;
	if ((up = strdup(r)) == NULL)
		exit(1);
	if ((make_dir(up, mode) == -1) && (errno != EEXIST))
		goto out;
	if ((mkdir(path, mode) == -1) && (errno != EEXIST))
		ret = -1;
	else
		ret = 0;

out:
	if (up != NULL)
		free(up);
	free(q);
	free(path);
	return ret;

}

void oops(char *s1, char  *s2)
{
	fprintf(stderr, "Error: %s ", s1);
	perror(s2);
	exit(1);
}
