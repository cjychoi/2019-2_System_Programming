/* system programing hw2 */
/* Tutor */
/*kos_0915@daum.net*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include<sys/stat.h>
#include <stdlib.h>

#define BUFFERSIZE 4096
#define SIZE 50


DIR* opendir_(char dir[]); // opendir �� �ϸ鼭 error check �� �ϱ� ���� �Լ��� ���� ����
void make_path(char path[], char dir[], struct dirent* dirent_p); // ��θ� �������ֱ� ���� �Լ� ���� , search_dir create �Լ� �� ��� ���
int search_directory(struct dirent* dirent_p, char dir_path[], char dir[], struct stat* st_info); // directory ���θ� Ž���ϸ� . �� .. �� ������ false return 
void check_subdir(DIR* src_ptr, char dir[]); // directory ���ο� subdir �� �ִ��� check ���ִ� �Լ�, ������ error ó���ϰ� ����
void cp_r(char src[], char des[]); // directory ���ο� ���ϵ��� �ű�� �Լ� 
void oops(char* s1, char* s2); // error printing �Լ�


int main(int ac, char* av[])
{
	if (ac != 3)
	{
		fprintf(stderr, "usage:%s source destination\n", *av);
		return 0;
	}
	else
		cp_r(av[1], av[2]);
}

void cp_r(char src[], char des[]) // parameter : source directory, destination directory 
{
	struct stat info;
	struct dirent* direntp;
	int in_fd, out_fd, n_chars;
	char buf[BUFFERSIZE];
	char src_path[SIZE];
	char des_path[SIZE];

	DIR* src_ptr; //source directory pointer
	DIR* des_ptr; //destination directory pointer

	src_ptr = opendir_(src); // source directory �� ���� opendir 
	check_subdir(src_ptr, src);
	des_ptr = opendir_(des);// source directory �� ���� opendir 

	while ((direntp = readdir(src_ptr)) != NULL)
	{
		if (search_directory(direntp, src_path, src, &info) == 0)
			continue;

		if ((info.st_mode & S_IRUSR) || (info.st_mode & S_IRGRP) || (info.st_mode & S_IROTH))
		{
			if ((in_fd = open(src_path, O_RDONLY)) == -1)
				continue;

			make_path(des_path, des, direntp);
			if ((out_fd = creat(des_path, info.st_mode)) == -1) // file �� ������ ���� ������ st_mode info �� �������� �����ؾ��Ѵ�.
				oops("can't create out_fd", des_path);

			while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
				write(out_fd, buf, n_chars) != n_chars;
		}
		close(in_fd);
		close(out_fd);
	}
	closedir(src_ptr);
	closedir(des_ptr);

}

DIR* opendir_(char dir[])
{
	DIR* d_ptr;
    if((d_ptr=opendir(dir)) == NULL)
    {
        closedir(d_ptr);
        oops("can't open",dir);
    }
	return d_ptr;
}
void make_path(char path[], char dir[],struct dirent* dirent_p)
{
	strcpy(path, "./");
	strcat(path, dir);
	strcat(path, "/");
	strcat(path, dirent_p->d_name);
}
int search_directory(struct dirent* dirent_p, char dir_path[], char dir[],struct stat *st_info) // .�� .. �� ��쿡�� false �� return �Ѵ�.
{
	if (!strcmp(dirent_p->d_name, ".") || !strcmp(dirent_p->d_name, ".."))
		return 0;
	make_path(dir_path, dir,dirent_p);
	stat(dir_path, st_info);
	return 1;
}
void check_subdir(DIR* src_ptr,char dir[])
{
	DIR* temp = src_ptr;
    char dir_path[SIZE];

    struct dirent* dirent_p;
    struct stat st_info;

    while((dirent_p = readdir(temp)))
    {
		if (search_directory(dirent_p, dir_path, dir,&st_info)==0)
			continue;
        if((st_info.st_mode & S_IFMT) == S_IFDIR)
        {
            closedir(src_ptr);
            oops("subdirectory : ",dirent_p->d_name);
        }
    }
	closedir(src_ptr);
	src_ptr = opendir_(dir);
}	
void oops(char *s1, char *s2) 
{
	fprintf(stderr,"error:%s",s1);
	perror(s2);
	exit(1);
}
