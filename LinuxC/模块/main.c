#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<cjson/cJSON.h>

#define SERV_PORT 9999
#define BUFLEN 4069

int cfd;

char recvbuf[BUFLEN];

void *recvdata(void *arg)
{
    do{    
        recv(cfd, recvbuf, BUFLEN, 0);
        //收到数据然后解析
        cJSON *json = cJSON_Parse(recvbuf);
        
    } while (1);


}
void senddata(cJSON * data){
    
}
