#include<stdio.h>
#include <fcntl.h>
#include "unistd.h"
char c='1';
int main()
{
    int fd=open("./sample.txt",O_RDWR|O_CREAT|O_EXCL,0666);
    for(int i;i<1024*1024*1024;i++)
    {
        write(fd,&c,1);
    }
}