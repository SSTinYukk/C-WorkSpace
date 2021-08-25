#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

#define LUV_U "Love you ❤\n"

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int  main(int argc,char *argv[])
{
    int ret;
    int fd[2];
    pid_t pid;
    char* buf[BUFSIZ];

    ret = pipe(fd);
    if(ret==-1){
        sys_err("pipe");
    }
    pid=fork();
    if(pid>0){
        close(fd[0]);
        write(fd[1],LUV_U,strlen(LUV_U));
    }else if(pid==0){
        close(fd[1]);
        ret=read(fd[0],buf,sizeof(buf));
        write(STDOUT_FILENO,buf,ret);
        close(fd[0]);
    }else{
        sys_err("fork");
    }
}