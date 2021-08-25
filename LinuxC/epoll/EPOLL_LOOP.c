#include<stdio.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>

#define MAX_EVENTS 1024
#define BUFLEN 4069
#define SERV_PORT 9999
#define TIME_OUT 1000

void recvdata(int fd,int events, void * arg);
void senddata(int fd,int events,void *arg);

struct myevent_s{
    int fd;
    int events;
    void *arg;
    void(*call_back)(int fd,int events,void *arg);
    int status;
    char buf[BUFLEN];   
    int len;
    long last_active;
}; 

void my_err(int ret,char *str)
{
    if(ret<0){
        perror(str);
        exit(1);
    }
    return;
}

int g_efd;
struct myevent_s g_events[MAX_EVENTS+1];

void eventset(struct myevent_s *ev,int fd,void(*call_back)(int,int,void*),void *arg)
{
    ev->fd = fd;
    ev->call_back=call_back;
    ev->events=0;
    ev->len=0;
    ev->arg=arg;
    ev->status=0;
    ev->last_active=time(NULL);
    return;
}

void eventadd(int efd, int events, struct myevent_s *ev)
{
    struct epoll_event epv;
    int op;
    epv.events=ev->events=events;
    epv.data.ptr=ev;

    if(ev->status==0){
        op=EPOLL_CTL_ADD;
        ev->status=1;
    }

    if (epoll_ctl(efd, op, ev->fd, &epv) < 0) //实际添加/修改
        printf("[INFO] event add failed [fd=%d], events[%d]\n", ev->fd, events);
    else
        printf("[INFO] event add OK [fd=%d], op=%d, events[%0X]\n", ev->fd, op, events);

    return;
}

void eventdel(int efd,struct myevent_s *ev)
{
    struct epoll_event epv;
    if(ev->status!=1){
        return;
    }
    epv.data.ptr=NULL;
    ev->status = 0;

    epoll_ctl(efd,EPOLL_CTL_DEL,ev->fd,&epv);
    return;
}

void accpetnonblock(int lfd,int events,void* arg)
{
    struct sockaddr_in cin;
    socklen_t slen=sizeof(cin);
    int cfd,i;

    cfd=accept(lfd,(struct sockaddr*)&cin,&slen);
    
    do{
        for(i=0;i<MAX_EVENTS;i++){
            if(g_events[i].status==0){
                break;
            }
        }
        if (i >= MAX_EVENTS)
        {
            printf("%s: max connect limit[%d]\n", __func__, MAX_EVENTS);
            break; //跳出do while(0) 不执行后续代码
        }
    }while(0);

    eventset(&g_events[i],cfd,recvdata,&g_events[i]);
    eventadd(g_efd,EPOLLIN,&g_events[i]);
}

void initlistensocket(int efd,short port)
{
    struct sockaddr_in sin;
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    fcntl(lfd,F_SETFL,O_NONBLOCK);

    memset(&sin,0,sizeof(sin));

    sin.sin_family=AF_INET;
    sin.sin_addr.s_addr=htonl(INADDR_ANY);
    sin.sin_port=htons(SERV_PORT);

    bind(lfd,(struct sockaddr*)&sin,sizeof(sin));
    
    listen(lfd,20);


    eventset(&g_events[MAX_EVENTS],lfd,accpetnonblock,&g_events[MAX_EVENTS]);
    eventadd(efd,EPOLLIN,&g_events[MAX_EVENTS]);
    return;
}

void recvdata(int fd,int events,void *arg)
{
    struct myevent_s *ev = (struct myevent_s *)arg;
    int len;

    len = read(fd,ev->buf,sizeof(ev->buf));
    
    eventdel(g_efd,ev);
    
    if(len>0){
        /*一顿操作*/
        for(int i=0;i<len;i++){
            ev->buf[i]=toupper(ev->buf[i]);
        }

        eventset(ev,fd,senddata,ev);
        eventadd(g_efd,EPOLLOUT,ev);
    }else if(len==0){
        close(ev->fd);
        printf("[INFO] Clinet exit\n");
    }else{
        close(ev->fd);
        printf("[INFO] Clinet exit\n");
    }
}

void senddata(int fd, int events, void *arg)
{
    struct myevent_s *ev = (struct myevent_s *)arg;
    int len;
    lseek(fd,SEEK_SET,0);
    len = write(ev->fd, ev->buf,sizeof(ev->buf));
    eventdel(g_efd, ev);

    if (len > 0)
    {
        printf("[INFO]send[fd=%d], [%d]%s\n", fd, len, ev->buf);
        eventset(ev,fd,recvdata,ev);
        eventadd(g_efd, EPOLLIN, ev);
    }
    else
    {
        close(fd);
    }
}

int main(int argc,char **argv)
{
    int i,cnt;
    g_efd=epoll_create(MAX_EVENTS+1);
    if(g_efd<0){
        fprintf(stderr,"epoll_create error\n");
    }
    initlistensocket(g_efd,(unsigned short)SERV_PORT);
    struct epoll_event events[MAX_EVENTS+1];

    while(1){
        //printf("[INFO] %dth event\n",cnt++);
        int nfd = epoll_wait(g_efd, events, MAX_EVENTS, TIME_OUT);
        if(nfd<0){
            fprintf(stderr,"epoll_wait error\n");
            break;
        }
        for(i=0;i<nfd;i++){
            struct myevent_s *ev =(struct myevent_s *)events[i].data.ptr;
            if((events[i].events&EPOLLIN)&&(ev->events&EPOLLIN)){
                ev->call_back(ev->fd,events[i].events,ev->arg);
            }
            if((events[i].events&EPOLLOUT)&&(ev->events&EPOLLOUT)){
                ev->call_back(ev->fd,events[i].events,ev->arg);
            }
        }
    }
}
