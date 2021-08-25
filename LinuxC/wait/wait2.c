#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
int main()
{
    int i;
    pid_t pid,wpid,tmpid;
    
    for (i=0;i<5;i++){
        pid=fork();
        if(pid==0){
            break;
        }else if(i==2){
            tmpid=pid;
            printf("--pid=%d\n",tmpid);
        }
    }
    
    if(i==5){
        printf("i m parent before wait pid=%d\n",tmpid);
        wpid=waitpid(tmpid,NULL,0);
         if(wpid==-1){
        perror("wait");
        exit(1);
        }
        printf("Wait Succeed:%d\n",wpid);
    }else{
        printf("i m %dth chd pid=%d\n",i,getpid());
    }
   
}