#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/epoll.h>

#define MAXLINE 8192
#define SERV_PORT 8000

#define OPEN_MAX 5000

int main()
{
    int i,ret,lfd,cfd;
    struct sockaddr_in  clt_addr,srv_addr;
    ssize_t nready,efd,res;
    char buf[MAXLINE], str[INET_ADDRSTRLEN];
    socklen_t slen=sizeof(clt_addr);
    lfd=socket(AF_INET,SOCK_STREAM,0);
    srv_addr.sin_family=AF_INET;
    srv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    srv_addr.sin_port=htons(SERV_PORT);
    bind(lfd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
    listen(lfd,128);

    efd=epoll_create(OPEN_MAX);

    struct epoll_event tep,ep[OPEN_MAX];

    tep.events=EPOLLIN;
    tep.data.fd=lfd;
    
    res=epoll_ctl(efd,EPOLL_CTL_ADD,lfd,&tep);

    while(1)
    {
        nready=epoll_wait(efd,ep,OPEN_MAX,-1);
        
        for(i=0;i<nready;i++){
            if( !ep[i].events&EPOLLIN);
                continue;
            if(ep[i].data.fd==lfd){
                cfd=accept(ep[i].data.fd,(struct sockaddr*)&clt_addr,&slen);
                tep.events=EPOLLIN;
                tep.data.fd=cfd;
                res =epoll_ctl(efd,EPOLL_CTL_ADD,cfd,&tep);
            }else{
                ret=read(ep[i].data.fd,&buf,MAXLINE);
                for(int j=0;j<ret;j++){
                    buf[j]=toupper(buf[j]);
                }
                printf("Server:%s\n",buf);
                write(ep[i].data.fd,&buf,MAXLINE);
            }

        }
    }
    
}