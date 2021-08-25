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

#define SRV_PORT 9999

void sys_err(char *str)
{
    perror(str);
    exit(1);
}
void *supth(void *argv)
{
    int* cfd=(int *)argv;
    char buf[4069];
    int ret,i;
    while(1){
        ret=read(*cfd,&buf,4069);
        if(ret==0)
        {
            pthread_exit(NULL);
        }
        for(i=0;i<ret;i++){
            buf[i]=toupper(buf[i]);
            }
        printf("[INFO]Success Toupper Strings:%s");
        write(*cfd,&buf,ret);
    }
}
int main(int argc, char** argv)
{
    int lfd,cfd[BUFSIZ],ret,i=0;
    char buf[BUFSIZ];
    struct sockaddr_in srv_addr,clt_addr;
    socklen_t len=sizeof(clt_addr);
    pthread_t tid;

    srv_addr.sin_family=AF_INET;
    srv_addr.sin_port=htons(SRV_PORT);
    srv_addr.sin_addr.s_addr=htonl(INADDR_ANY);

    
    lfd = socket(AF_INET,SOCK_STREAM,0);
    
    ret = bind(lfd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));

    ret = listen(lfd,128);

    while(1){
        cfd[i]=accept(lfd,(struct sockaddr*)&clt_addr,&len);
        pthread_create(&tid,NULL,supth,(void *)&cfd[i]);
        pthread_detach(tid);
        i++;
    }
}