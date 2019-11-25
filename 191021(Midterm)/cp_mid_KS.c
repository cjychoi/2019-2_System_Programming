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
void do_cpy(char *, char *, int, int);
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
    struct stat dir_info;
    struct stat info; //struct stat to make new destination directory
    int exist; 

    // to check whether destination directory exists
    exist=access(dirname2,0);
    stat(dirname1, &dir_info);

    if(exist==0) // if exists
    {
        strcat(dirname2, "/");
        strcat(dirname2, dirname1);
        mkdir(dirname2, dir_info.st_mode);
    }
    
    else // if no exists
    {
        mkdir(dirname2, dir_info.st_mode);
    }
    
    

    if((dir_ptr=opendir(dirname1))==NULL)
    {
        fprintf(stderr, "Cannot open %s\n", dirname1);
    }

    
    else
    {
        while((direntp=readdir(dir_ptr))!=NULL){
            //읽을 파일 이름
            char src[50]="";
            strcat(src, dirname1);
            strcat(src, "/");
            strcat(src, direntp->d_name);

            //복사할 파일 이름
            char des[50]="";
            strcat(des, dirname2);
            strcat(des, "/");
            strcat(des, direntp->d_name);

            //file의 status 저장
            stat(src, &info);

            //현재 directory와 이전 directory는 무시한다
            if(strcmp(direntp->d_name, ".")==0 || strcmp(direntp->d_name, "..")==0)
                continue;

            // subdirectory가 있으면 recursive
            if(S_ISDIR(info.st_mode)) 
            {
                enter_dir(src, des);   
            }

            //file copy
            do_cpy(src, des, info.st_mode, S_ISDIR(info.st_mode));
        }
    }

}

void do_cpy(char *filename, char *des, int mode, int option)
{
    int in_fd, out_fd, n_chars;
    char buf[BUFFERSIZE];
    DIR *dirp;
    struct dirent *direntp;

    //if file is directory
    if(option==1)
        return;
    
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