#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include<pthread.h>

#define LUV_U "Love you ❤\n"
struct super 
{
    int iii;
    char sss[256];
    struct super* next;
};

void sys_err(const char *str)
{
    perror(str);
    exit(1);
}
void pexit(void)
{
    pthread_exit(NULL);
    return;
}
void *thd(void* arg){
    struct super *a;
    a=(struct super*)malloc(sizeof(a));
    int* i=(int *)arg;
    printf("I'm %dth pthread, pid=%d pthid=%lu\n",i,getpid(),pthread_self());
    a->iii=10001231;
    strcpy(a->sss,LUV_U);
    return (void *)a;
}

int main(void)
{   
    pthread_attr_t attr;
    pid_t pid;
    pthread_t tid;
    struct super *r;
    int ret;
    ret=pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    if(ret!=0){
        fprintf(stderr,"attr_setdetachstate error:%s\n",strerror(ret));
        exit(1);
    }
    for(int i=0;i<5;i++)
    {
        pthread_create(&tid, &attr, thd, (void *)i);
        if(ret==-1)
        sys_err("pthread creat"); 
    }
     ret = pthread_attr_destroy(&attr);
    if(ret!=0){
        fprintf(stderr,"pthread_attr_destroy error:%s\n",strerror(ret));
    }
    printf("Pid=%d Main Tid=%lu\n", getpid(), pthread_self());
    //pthread_join(tid,(void **)&r);
    printf("Return %s %d\n",r->sss,r->iii);
}
