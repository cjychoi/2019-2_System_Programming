#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

ino_t get_inode(char *);
void inum_to_name(ino_t, char *, int);
void printpathio(ino_t);
void do_ls(char *, int);
void sort_by_inode(struct dirent[], int);
int cnt = 1;
char des[BUFSIZ];
ino_t cur_ino;

int main()
{
	cur_ino = get_inode(".");		// save current directory inode num
	printpathio(get_inode("."));
	putchar('\n');
	printf("The current working directory: %s\n", des);
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
	while ((direntp = readdir(dir_ptr)) != NULL)
		if (direntp->d_ino == inode_to_find) {
			strncpy(namebuf, direntp->d_name, buflen);
			namebuf[buflen - 1] = '\0';		// just in case for filename longer than buffer
			closedir(dir_ptr);
			return;
		}

	fprintf(stderr, "error looking for inum %ld\n", inode_to_find);
	exit(1);
}


void printpathio(ino_t this_inode)
/*		Prints path leading down to an object with this inode
		Recursive
*/
{
	ino_t my_inode;
	char its_name[BUFSIZ];

	/* Print '/' for Root directory */
	if (get_inode("..") == this_inode) {
		printf("%ld\t/\n", this_inode);

		/* print from root dir */
		do_ls(".", ++cnt);
	}


	/* If not Root directory */
	if (get_inode("..") != this_inode) {		// not in root directory
		chdir("..");																// up one dir
		inum_to_name(this_inode, its_name, BUFSIZ);		// get its name
		my_inode = get_inode(".");									// print inode

		printpathio(my_inode);											// recursive

		/* Reached root directory */
		strcat(des, "/");						// append '/' to path(des)
		chdir(its_name);
		if(get_inode(".") != cur_ino)		// stop if current dir.
			do_ls(its_name, ++cnt);		// Load dir. & Sort & Print
		strcat(des, its_name);			// save path(des)
	}


}


void do_ls(char *dirname, int cnt)
{
	DIR *dir_ptr;								// directory
	struct dirent *direntp;				// each entry
	struct dirent dir_ary[BUFSIZ];	// ary for sorting
	struct stat tempfile;
	int dir_cnt = 0;
	int i = 0;

	if ((dir_ptr = opendir(".")) == NULL)
		fprintf(stderr, "Cannot open %s\n", dirname);

	else {
		while ((direntp = readdir(dir_ptr)) != NULL) {

			if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0) {
				/* Skip . and .. */
				continue;
			}

			/* Copy dir. info. */
			// dir_ary[i].d_ino = direntp->d_ino;	
			lstat(direntp->d_name, &tempfile);	
			dir_ary[i].d_ino = tempfile.st_ino;
			strcpy(dir_ary[i].d_name, direntp->d_name);

			i++;				// next dir.
			dir_cnt++;	// count size
		}

		/* Sorting */
		sort_by_inode(dir_ary, dir_cnt);

		/* Print */
		for (i = 0; i < dir_cnt; i++) {
			printf("%ld", dir_ary[i].d_ino);

			for (int j = 0; j < cnt; j++)
				printf("\t");	// printing space

			printf("%s\n", dir_ary[i].d_name);
		}
		closedir(dir_ptr);
	}
}


void sort_by_inode(struct dirent dir_ary[], int size)
{
	struct dirent temp;
	int i, j;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (dir_ary[i].d_ino < dir_ary[j].d_ino) {
				/* Bubble Sort */
				temp.d_ino = dir_ary[j].d_ino;			strcpy(temp.d_name, dir_ary[j].d_name);
				dir_ary[j].d_ino = dir_ary[i].d_ino;		strcpy(dir_ary[j].d_name, dir_ary[i].d_name);
				dir_ary[i].d_ino = temp.d_ino;			strcpy(dir_ary[i].d_name, temp.d_name);
			}
		}
	}
}