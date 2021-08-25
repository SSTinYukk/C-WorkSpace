#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
int main(void)
{
    char buf[100];
    int num=0;
    if((num=read(STDIN_FILENO,buf,10))==-1){
        printf("READ ERROR\n");
    }else{
        write(STDOUT_FILENO,buf,num);
    }
    return 0;
}
