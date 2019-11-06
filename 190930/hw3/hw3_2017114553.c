#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

ino_t get_inode(char *);
void print_sub(ino_t);
void inum_to_name(ino_t, char *, int);
void do_ls(char *, int);
void sort_inode(struct dirent[], int);
int cnt=1;
char des[BUFSIZ];

int main(void)
{
    print_sub(get_inode("."));
    putchar('\n');
    printf("The current working directory: %s\n", des);
    return 0;
}

ino_t get_inode(char *fname)
{
    struct stat info;

    if(stat(fname, &info)==-1)
    {
        fprintf(stderr, "Cannot stat ");
        perror(fname);
        exit(1);
    }

    return info.st_ino;
}

void inum_to_name(ino_t inode_to_find, char *namebuf, int buflen)
{
    DIR *dir_ptr;
    struct dirent *direntp;
    dir_ptr=opendir(".");
    if(dir_ptr==NULL)
    {
        perror(".");
        exit(1);
    }

    while((direntp=readdir(dir_ptr))!=NULL) {
        if(direntp->d_ino == inode_to_find)
        {
            strncpy(namebuf, direntp->d_name, buflen);
            namebuf[buflen-1]='\0';
            closedir(dir_ptr);
            return;
        }
    }

    fprintf(stderr, "error looking for inum %ld\n", inode_to_find);
    exit(1);
}

void print_sub(ino_t this_inode)
{
    ino_t my_inode;
    char its_name[BUFSIZ];
    
	/* Root Directory */
    if(get_inode("..")==this_inode)
    {
        printf("%ld\t/\n", this_inode);
    }

    else if(get_inode("..")!=this_inode)
    {
        chdir("..");
        inum_to_name(this_inode, its_name, BUFSIZ);
        my_inode=get_inode(".");		// get inode of current directory
        
        print_sub(my_inode);
        strcat(des, "/");
        do_ls(des, ++cnt);
        strcat(des, its_name);
    }
}

void do_ls(char *dirname, int cnt)
{
    DIR *dir_ptr;
    struct dirent *direntp;
    struct dirent dir_arr[BUFSIZ];
    int dir_cnt=0;
    int i=0;
    if((dir_ptr = opendir(dirname))==NULL)
    {
        fprintf(stderr, "Cannot open %s\n", dirname);
    }


    else
    {
        while((direntp=readdir(dir_ptr))!=NULL){
			/* skip . and .. */
            if(strcmp(direntp->d_name, ".")==0 || strcmp(direntp->d_name, "..")==0)
                continue;
		 
            dir_arr[i].d_ino=direntp->d_ino;
            strcpy(dir_arr[i].d_name, direntp->d_name);
            i++;
            dir_cnt++;		// count size
        }

        sort_inode(dir_arr, dir_cnt);

		/* Print */
        for(i=0; i<dir_cnt; i++){
            printf("%ld", dir_arr[i].d_ino);
            for(int i=0; i<cnt; i++){
              printf("\t");  
            }
            printf("%s\n", dir_arr[i].d_name);
        }

        closedir(dir_ptr);
    }
      
}

void sort_inode(struct dirent dir_arr[], int size)
{
    struct dirent temp;
    for(int i=0; i<size; i++){
        for(int j=i; j<size; j++){
            if(dir_arr[i].d_ino > dir_arr[j].d_ino)
            { /* Bubble Sort*/
                temp.d_ino=dir_arr[i].d_ino;
                strcpy(temp.d_name, dir_arr[i].d_name);
                dir_arr[i].d_ino=dir_arr[j].d_ino;
                strcpy(dir_arr[i].d_name, dir_arr[j].d_name);
                dir_arr[j].d_ino=temp.d_ino;
                strcpy(dir_arr[j].d_name, temp.d_name);
            }
        }
    }
}