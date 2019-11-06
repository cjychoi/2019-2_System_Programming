#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

ino_t get_inode(char *);
void printpathio(ino_t);
void inum_to_name(ino_t, char *, int );

int main()
{
	printpathio(get_inode("."));		// print path to here
	putchar('\n');
	return 0;
}

ino_t get_inode(char *fname)
/* returns inode number of the file */
{
	struct stat info;
	if (stat(fname, &info) == -1) {
		fprintf(stderr, "Cannot stat ");
		perror(fname);
		exit(1);
	}
	return info.st_ino;
}

void printpathio(ino_t this_inode)
/*		Prints path leading down to an object with this inode
		Recursive
*/
{
	ino_t my_inode;
	char its_name[BUFSIZ];

	if (get_inode("..") != this_inode) {		// not in root directory
		chdir("..");																// up one dir
		inum_to_name(this_inode, its_name, BUFSIZ);		// get its name
		my_inode = get_inode(".");									// print head
		printpathio(my_inode);											// recursively
		printf("/%s", its_name);											// now print name of this directory
	}
}

void inum_to_name(ino_t inode_to_find, char *namebuf, int buflen)
/*		Looks through current directory fo a file with this inode number
		and copies its name into namebuf
*/
{
	DIR *dir_ptr;						// the directory
	struct dirent *direntp;		// each entry
	dir_ptr = opendir(".");

	if (dir_ptr == NULL) {
		perror(".");
		exit(1);
	}

	/* Search directory for a file with specified inum */
	while((direntp = readdir(dir_ptr)) != NULL)
		if (direntp->d_ino == inode_to_find) {
			strncpy(namebuf, direntp->d_name, buflen);
			namebuf[buflen - 1] = '\0';		// just in case for filename longer than buffer
			closedir(dir_ptr);
			return;
		}

	fprintf(stderr, "error looking for inum %ld\n", inode_to_find);
	exit(1);
}