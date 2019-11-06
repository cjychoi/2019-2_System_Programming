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


DIR* opendir_(char dir[]); // opendir 을 하면서 error check 를 하기 위해 함수로 별도 구현
void make_path(char path[], char dir[], struct dirent* dirent_p); // 경로를 구성해주기 위한 함수 구현 , search_dir create 함수 전 등에서 사용
int search_directory(struct dirent* dirent_p, char dir_path[], char dir[], struct stat* st_info); // directory 내부를 탐색하며 . 과 .. 을 만나면 false return 
void check_subdir(DIR* src_ptr, char dir[]); // directory 내부에 subdir 이 있는지 check 해주는 함수, 있으면 error 처리하고 종료
void cp_r(char src[], char des[]); // directory 내부에 파일들을 옮기는 함수 
void oops(char* s1, char* s2); // error printing 함수


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

	src_ptr = opendir_(src); // source directory 에 대한 opendir 
	check_subdir(src_ptr, src);
	des_ptr = opendir_(des);// source directory 에 대한 opendir 

	while ((direntp = readdir(src_ptr)) != NULL)
	{
		if (search_directory(direntp, src_path, src, &info) == 0)
			continue;

		if ((info.st_mode & S_IRUSR) || (info.st_mode & S_IRGRP) || (info.st_mode & S_IROTH))
		{
			if ((in_fd = open(src_path, O_RDONLY)) == -1)
				continue;

			make_path(des_path, des, direntp);
			if ((out_fd = creat(des_path, info.st_mode)) == -1) // file 을 생성할 때는 기존의 st_mode info 를 바탕으로 생성해야한다.
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
int search_directory(struct dirent* dirent_p, char dir_path[], char dir[],struct stat *st_info) // .과 .. 일 경우에는 false 를 return 한다.
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
