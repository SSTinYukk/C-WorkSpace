#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

void err_thread(int ret,char *str)
{
    if(ret!=0){
        fprintf(stderr,"%s:%s\n",str,strerror(ret));
        pthread_exit(NULL);
    }
}

struct msg{
    int num;
    struct msg *next;
};

struct msg *head;

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t has_data = PTHREAD_COND_INITIALIZER;
void *produser(void *arg)
{
    for(;;){
        struct msg *pr;
        pr=(struct msg*)malloc(sizeof(struct msg));
        pr->num=rand()%1000+1;
        fprintf(stdout, "Prduction------num=%d\n", pr->num);
        pthread_mutex_lock(&mutex);
        pr->next = head;
        head = pr;
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&has_data);
        sleep(rand()%3);
    }
    return NULL;
}

void *consumer(void *arg)
{
    struct msg *pr;
    for(;;){
        pthread_mutex_lock(&mutex);
        if(head == NULL){
            pthread_cond_wait(&has_data,&mutex);
        }
        pr=head;
        head = pr->next;
        pthread_mutex_unlock(&mutex);
        printf("Consumer------num=%d\n", pr->num);
        free(pr);
        sleep(rand()%3);
    }
    
    
    
}

int main(int argc,char *argv[])
{
    srand(time(NULL));
    int ret;
    pthread_t pid,cid;

    ret = pthread_create(&pid,NULL,produser,NULL);
    if(ret!=0){
        err_thread(ret,"pthread_creater");
    }
    ret = pthread_create(&cid,NULL,consumer,NULL);
    if (ret != 0)
    {
        err_thread(ret, "pthread_consumer");
    }
    pthread_join(pid,NULL);
    pthread_join(cid,NULL);
}