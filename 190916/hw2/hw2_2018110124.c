#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#define BUFFERSIZE 4096

void cpy(char*, char *, int);
void oops(char *, char *);
void do_ls1(char *, char *);

int main(int ac, char *av[])
{
	/* check args */
	if (ac != 3) {
		fprintf(stderr, "usage: %s source destination\n", *av);
		exit(1);
	}

	do_ls1(av[1], av[2]);
}


void cpy(char *name, char *copied, int mode)
{
    int in_fd, out_fd, n_chars;
    char buf[BUFFERSIZE];

    /* open files */
    if( (in_fd = open(name, O_RDONLY) ) == -1)
        oops("Cannot open ", name);

    if( (out_fd = creat(copied, mode) ) == -1)
        oops("Cannot creat", copied);

    while( (n_chars = read(in_fd, buf, BUFFERSIZE) ) > 0)
        if(write(out_fd, buf, n_chars) != n_chars)
            oops("Write error to ", copied);
    
    if(n_chars == -1)
        oops("Read error from ", name);

    /* close files */
    if(close(in_fd) == -1 || close(out_fd) == -1)
        oops("Error closing files", "");
}

void oops(char *s1, char *s2)
{
    fprintf(stderr, "Error: %s ", s1);
    perror(s2);
    exit(1);
}

void do_ls1(char dirname1[], char dirname2[])
{
	DIR *dir_ptr;
	struct dirent *direntp;

	struct stat read;
	int tmode;


	if ( (dir_ptr = opendir(dirname1)) == NULL)
		fprintf(stderr, "Cannt open %s\n", dirname1);


	else {
		while ( (direntp = readdir(dir_ptr)) != NULL) {

			// Reading File
			char fullname[50];
			strcpy(fullname, dirname1);
			strcat(fullname, "/");
			strcat(fullname, direntp->d_name);

			stat(fullname, &read);

			// Ignore current directory and parent directory
			if (!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, ".."))
				continue;

			// Do not copy if there is a subdirectory
			if (S_ISDIR(read.st_mode)) {
				printf("[Error] There is a subdirectory in the source directory\n");
				exit(1);
			}
		}

			// Move back to the first
		rewinddir(dir_ptr);

		while ((direntp = readdir(dir_ptr)) != NULL) {
				
				// Reading File
				char fullname[50];
				strcpy(fullname, dirname1);
				strcat(fullname, "/");
				strcat(fullname, direntp->d_name);

				// Copying File
				char copyname[50];
				strcpy(copyname, dirname2);
				strcat(copyname, "/");
				strcat(copyname, direntp->d_name);

				stat(fullname, &read);

				// Ignore current directory and parent directory
				if (!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, ".."))
					continue;

				// Do not copy non-readable files(no read permission)
				if (!((read.st_mode & S_IRUSR) || (read.st_mode & S_IRGRP) || (read.st_mode & S_IROTH)))
					continue;

				// Copy File
				cpy(fullname, copyname, read.st_mode);
		}
		closedir(dir_ptr);
	}
}