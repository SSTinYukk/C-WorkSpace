#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>>

int main(void)
{
    int fd;
    int ret;
    fd=open("AAA.txt",O_WRONLY|O_CREAT|O_TRUNC,0644);
    if(fd==-1)
        printf("OPen ErroR\n");
    write(fd,"hello",5);
    ret=lseek(fd,1024*1024*1024,SEEK_CUR);
    if(ret==-1)
        printf("lseek error");
    write(fd,"world",5);
    ret=lseek(fd,SEEK_CUR,SEEK_SET);
    printf("当前:%d\n");
    close(fd);
    return 0;

}