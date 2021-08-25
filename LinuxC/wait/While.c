#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>

int main(){
    int i;
    pid_t pid,tmpid,wpid;
    for(i=0;i<5;i++){
        if((pid=fork())==0)
            break;
        if(i==2){
            tmpid=pid;
            printf("i will wait pid:%d\n",tmpid);
        }
    }
    if(i==5){
        printf("i'm father pid=%d,the child %d will be wait\n",getpid(),tmpid);
        wpid=waitpid(tmpid,0,0);
        if(wpid==-1){
            perror("wait");
            exit(1);
        }else{
            printf("wait succeed pid:%d wait:%d\n",tmpid,wpid);
        }
    }else{
        printf("Im %dth child pid:%d\n",i,getpid());
    }
}