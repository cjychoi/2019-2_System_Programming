#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#define BUFFERSIZE 4096

void enter_dir(char *, char *);
void do_cpy(char *, char *, int);
void oops(char *, char *);

int main(int ac, char *av[])
{
    if(ac!=3)
    {
        fprintf(stderr, "usage: %s source destination\n", *av);
        exit(1);
    }

    enter_dir(av[1], av[2]);
    
}

void enter_dir(char dirname1[], char dirname2[])
{
    DIR *dir_ptr;
    struct dirent *direntp;
    
    struct stat info;
    int tmode;

    if((dir_ptr=opendir(dirname1))==NULL)
    {
        fprintf(stderr, "Cannot open %s\n", dirname1);
    }
	
    else
    {
        //directory를 한 번 읽어서 sub directory있으면 terminate
        while((direntp=readdir(dir_ptr))!=NULL){
           
            //읽을 파일 이름
            char src[50];
            strcpy(src, dirname1);
            strcat(src, "/");
            strcat(src, direntp->d_name);

            //file의 status 저장
            stat(src, &info);

            //현재 directory와 이전 directory는 무시한다
            if(strcmp(direntp->d_name, ".")==0 || strcmp(direntp->d_name, "..")==0)
                continue;

            // subdirectory가 있으면 파일 복사를 하지 않음
            if(S_ISDIR(info.st_mode)) 
            {
                printf("[Error] There is a subdirectory in the source directory\n");
                exit(1);
            }
        }

        //directory 읽기 위치를 처음으로 이동시킨다
        rewinddir(dir_ptr);

        //directory 다시 읽으면서 파일 복사
        while((direntp=readdir(dir_ptr))!=NULL){
            
            //읽을 파일 이름
            char src[50];
            strcpy(src, dirname1);
            strcat(src, "/");
            strcat(src, direntp->d_name);

            //복사할 파일 이름
            char des[50];
            strcpy(des, dirname2);
            strcat(des, "/");
            strcat(des, direntp->d_name);
            
            //debugging
            /*printf("%s\n", direntp->d_name);
            printf("%s\n", des);*/

            //file의 status 저장
            stat(src, &info);

            //현재 directory와 이전 directory는 무시한다
            if(strcmp(direntp->d_name, ".")==0 || strcmp(direntp->d_name, "..")==0)
                continue;

            //debugging
            /*printf("file mode: %o\n", info.st_mode);
            printf("%d\n", (info.st_mode&S_IRUSR) || (info.st_mode&S_IRGRP) || (info.st_mode&S_IROTH));*/
            
            // user, group, others 모두 파일을 읽을 수 없으면 복사를 하지 않음
            if(!((info.st_mode&S_IRUSR) || (info.st_mode&S_IRGRP) || (info.st_mode&S_IROTH)))
                continue;

            //file copy
            do_cpy(src, des, info.st_mode);
            
        }
        closedir(dir_ptr);
    }
}

void do_cpy(char *filename, char *des, int mode)
{
    int in_fd, out_fd, n_chars;
    char buf[BUFFERSIZE];
    //copy start
    if((in_fd=open(filename, O_RDONLY))==-1) //file open, handling error
    {
        oops("cannot open", filename);
    }
    
    if((out_fd=creat(des, mode))==-1) //file create, handling error
    {
        oops("Cannot creat", des);
    }

    while((n_chars=read(in_fd, buf, BUFFERSIZE))>0){
        if(write(out_fd, buf, n_chars)!= n_chars)
            oops("Write error to ", des);
    }

    if(n_chars==-1)
        oops("Read error from ", filename);

    if(close(in_fd)==-1 || close(out_fd)==-1)
        oops("Error closing files", "");
            
}

void oops(char *s1, char *s2)
{
    fprintf(stderr, "Error: %s ", s1);
    perror(s2);
    exit(1);
}