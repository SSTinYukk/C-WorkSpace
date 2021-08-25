#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include<mysql.h>

#define SERV_PORT 8888

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int main(int argc, char *argv[])
{
    int cfd,conter = 10;
    char buf[BUFSIZ];
    struct sockaddr_in serv_addr;
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(SERV_PORT);  
    inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr.s_addr);
    
    cfd = socket(AF_INET,SOCK_STREAM,0);
    if(cfd == -1)
        sys_err("socket error");
    
    int ret = connect(cfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    if(ret != 0)
        sys_err("Connect");
    while(--conter){
        write(cfd,"hello\n",4069);
        sleep(1);
        ret = read(cfd,buf,4069);
        write(STDOUT_FILENO,buf,ret);   
    }
}
//看弹幕好多人学不懂， 估计是没学网络就来学网络编程了。
//三次握手可以参考你打电话，电话接通，你说喂，你能听到吗，
//对方回，我能听到，你能听到吗，，你再回，我也能听到。这时说明你们的连接是正常的