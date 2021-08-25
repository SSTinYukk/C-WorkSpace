#include <unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>

int main(void)
{
    int fd;
    fd=open("ps.out",O_RDWR|O_CREAT|O_TRUNC,0644);
    if(fd<0){
        perror("Open Error");
    }
    dup2(fd,STDOUT_FILENO);
    execlp("ps","ps","aux",NULL);
    close(fd);

    return 0;

}