#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>

int main()
{
    pid_t pid,wpid;
    int status;

    pid =fork();
    if(pid==0){
        printf("my pid:%d  ppid:%d",getpid(),getppid());
    }else if(pid>0)
    {
        wpid=wait(&status);
        if(wpid==-1){
            perror("wait");
            exit(1);
        }
        if(WIFEXITED(status)){
            printf("child exit with%d\n",WEXITSTATUS(status));
        }
    }else{
        perror("fork");
        exit(1);
    }
}