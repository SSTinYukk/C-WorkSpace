#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include<signal.h>
#include<sys/wait.h>
#include<ctype.h>

#define SRV_PORT 9999

void sys_err(char *str)
{
    perror(str);
    exit(1);
}

void catch_child(int signum)
{
    while(waitpid(NULL,NULL,WNOHANG)>0)
        printf("[INFO] Child Process wait succeed\n");
    return;
}

int main(int argc,char *argv[])
{
    int lfd,cfd;
    pid_t pid;
    struct sockaddr_in srv_addr,clt_addr;
    socklen_t clt_addr_len;
    char buf[BUFSIZ];
    int ret,i;
    bzero(&srv_addr,sizeof(srv_addr));

    srv_addr.sin_family=AF_INET;
    srv_addr.sin_port=htons(SRV_PORT);
    srv_addr.sin_addr.s_addr=htons(INADDR_ANY);
    
    clt_addr_len=sizeof(clt_addr);
    
    lfd=socket(AF_INET,SOCK_STREAM,0);
    if(lfd==-1){
        sys_err("Socket");
    }

    ret=bind(lfd,(struct sockaddr *)&srv_addr,sizeof(srv_addr));
    if(ret==-1){
        sys_err("bind");
    }

    listen(lfd,128);
    while(1){
        cfd=accept(lfd,(struct sockaddr*)&clt_addr,&clt_addr_len);

        pid=fork();
        if (pid < 0)
        {
            sys_err("Fork");
        }
        else if(pid==0){
            close(lfd);
            break;
        }else{
            struct sigaction sig;
            sig.sa_handler=catch_child;
            sigemptyset(&sig.sa_mask);
            sig.sa_flags=0;
            sigaction(SIGCHLD,&sig,NULL);
            close(cfd);
            continue;
        }
    }

    if(pid==0)
    {
        printf("[INFO] Child Process fork succeed\n");
        for(;;){
            ret=read(cfd,&buf,sizeof(buf));
            if(ret==0){
                close(cfd);
                exit(1);
            }
            for(i=0;i<ret;i++){
                buf[i]=toupper(buf[i]);
            }
            printf("[INFO] data:%s", buf);
            write(cfd,&buf,ret);
        }
    }
    return 0;
}