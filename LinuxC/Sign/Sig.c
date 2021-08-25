#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>

#define LUV_U "Love you ❤\n"

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}
void catch_child(int signo)
{
    int status;
    pid_t wpid;
    while((wpid=waitpid(-1,&status,0))!=-1)
    {
    printf("catch child id:%d\n",wpid);
    }
    return;
}

int main(int argc,char *argv[])
{   
    pid_t pid;

    int i;
    for(i=0;i<5;i++)
        if((pid=fork())==0)
            break;
    if(5==i){
        printf("I'm parent,pid = %d\n",getpid());
        struct sigaction act;
        act.sa_handler=catch_child;
        sigemptyset(&act.sa_mask);
        act.sa_flags=0;
        sigaction(SIGCHLD,&act,NULL);
        //signal(SIGCHLD,catch_child);    
        while(1);
    }else{
        printf("I'm child pid =%d\n",getpid());
        sleep(1);
    }
}

