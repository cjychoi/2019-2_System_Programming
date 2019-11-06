#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h> 
#include <sys/types.h>

int mkdir_recursive(char *org_path, char *cur_path, int mode);
int get_fullpath();


void main(int ac, char *av[])
{
	if (ac != 3) {		/* exit if input is not 3 */
		fprintf(stderr, "usage: %s source destination\n", *av);
		exit(1);
	}

	// get_fullpath(/* leaf node */);
	// get full path of leaf node and make directory recursively


	mkdir_recursive(av[1], av[2], 0755);

}

int mkdir_recursive(char *org_path, char *cur_path, int mode)
{
	int ret;
	char *p;

	if ((p = strchr(cur_path + 1, '/')) != NULL)
		*p = '\0';

	if ((ret = mkdir(org_path, mode)) != 0)
	{
		switch (errno)
		{
		case EEXIST:			break;
		case EFAULT:		return 1;
		case ENOMEM:	return 1;
		case EROFS:			return 1;
		case ELOOP:			return 1;
		case ENOSPC:		return 1;
		case ENAMETOOLONG:     return 1;
		}
	}

	if (p == NULL) return 0;
	*p = '/';
	cur_path = p;

	return mkdir_recursive(org_path, cur_path, mode);
}

int get_fullpath(char *cur_path)
{
	char *fullpath;




	return fullpath;
}