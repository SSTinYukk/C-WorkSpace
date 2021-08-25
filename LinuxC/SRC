#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#define FILE_PATH "./test.txt"

int main()
{
    int fd;
    fd=open(FILE_PATH,O_RDWR|O_CREAT|O_EXCL,0666);
    if(fd<0){
        printf("Error\n");
        exit(-1);
    }else{
        printf("Succeed\n");
    }
    return 0;
}