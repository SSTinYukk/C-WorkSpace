#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "threadpool.h"

#define DEFAULT_TIME 10        /*10s检测一次*/
#define MIN_WAIT_TASK_NUM 10   /*如果queue_size > MIN_WAIT_TASK_NUM 添加新的线程到线程池*/
#define DEFAULT_THREAD_VARY 10 /*每次创建和销毁线程的个数*/

#define true 1
#define false 0

#define THREAD_MAX  128
#define THREAD_MIN  16
#define THREAD_QUEUE_MAX 128

typedef struct
{
    void *(*function)(void *); /* 函数指针，回调函数 */
    void *arg;                 /* 上面函数的参数 */
} threadpool_task_t;           /* 各子线程任务结构体 */

/* 描述线程池相关信息 */

struct threadpool_t
{
    pthread_mutex_t lock;           /* 用于锁住本结构体 */
    pthread_mutex_t thread_counter; /* 记录忙状态线程个数de琐 -- busy_thr_num */

    pthread_cond_t queue_not_full;  /* 当任务队列满时，添加任务的线程阻塞，等待此条件变量 */
    pthread_cond_t queue_not_empty; /* 任务队列里不为空时，通知等待任务的线程 */

    pthread_t *threads;            /* 存放线程池中每个线程的tid。数组 */
    pthread_t adjust_tid;          /* 存管理线程tid */
    threadpool_task_t *task_queue; /* 任务队列(数组首地址) */

    int min_thr_num;       /* 线程池最小线程数 */
    int max_thr_num;       /* 线程池最大线程数 */
    int live_thr_num;      /* 当前存活线程个数 */
    int busy_thr_num;      /* 忙状态线程个数 */
    int wait_exit_thr_num; /* 要销毁的线程个数 */

    int queue_front;    /* task_queue队头下标 */
    int queue_rear;     /* task_queue队尾下标 */
    int queue_size;     /* task_queue队中实际任务数 */
    int queue_max_size; /* task_queue队列可容纳任务数上限 */

    int shutdown; /* 标志位，线程池使用状态，true或false */
};

void *threadpool_thread(void *threadpool);

void *adjust_thread(void *threadpool);

int is_thread_alive(pthread_t tid);
int threadpool_free(threadpool_t *pool);

int main(void)
{
    threadpool_t *thp;
    thp=threadpool_create(THREAD_MIN,THREAD_MAX,THREAD_QUEUE_MAX);
    printf("[INFO] Poll Create\n");
    threadpool_add;
    threadpool_destroy(thp);
    return 0;
    
}

threadpool_t *threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size)
{
    int i;
    threadpool_t *pool=NULL;

    do{
        if((pool = (threadpool_t *)malloc(sizeof(threadpool_t)))==NULL){
            printf("malloc threadpool fail");
            break;
        }
        pool->min_thr_num = min_thr_num;
        pool->busy_thr_num=0;
        pool->live_thr_num=min_thr_num;
        pool->wait_exit_thr_num=0;
        pool->queue_size=0;
        pool->queue_max_size=queue_max_size;
        pool->queue_front=0;
        pool->queue_rear=0;
        pool->shutdown=false;

        pool->threads=(pthread_t*)malloc(sizeof(pthread_t)*max_thr_num);
        if(pool->threads==NULL){
            printf("malloc threads fail\n");
            break;
        }
        memset(pool->threads, 0, sizeof(pthread_t) * max_thr_num);

        pool->task_queue = (threadpool_task_t *)malloc(sizeof(threadpool_task_t)*queue_max_size);
        if(pool->task_queue==NULL){
            printf("malloc task_queue fail");
            break;
        }

        if(pthread_mutex_init(&(pool->lock),NULL)!=0
            ||pthread_mutex_init(&(pool->thread_counter),NULL)!=0
            ||pthread_cond_init(&(pool->queue_not_empty),NULL)!=0
            ||pthread_cond_init(&(pool->queue_not_full),NULL)!=0)
            {
                printf("init lock cond fail");
                break;
            }
            for(i=0;i<min_thr_num;i++){
                pthread_create(&(pool->threads[i]),NULL,threadpool_thread,(void *)pool);
                printf("start thread 0x%x..\n",(unsigned int)pool->threads[i]);
            }
            printf_create(&(pool->adjust_tid),NULL,adjust_thread,(void *)pool);
    }while(0);

    threadpool_free(pool);
    return NULL;
}

void *threadpool_thread(void *threadpool){
    threadpool_t *pool =(threadpool_t *)threadpool;
    threadpool_task_t task;
    while(true){
        pthread_mutex_lock(&(pool->lock));

        while((pool->queue_size==0)&&(!pool->shutdown)){
            printf("thread 0x%x is waiting\n",(unsigned int)pthread_self());
            pthread_cond_wait((&pool->queue_not_empty),&(pool->lock));
        }

        if(pool->wait_exit_thr_num>0){
            pool->wait_exit_thr_num--;
        }

        if(pool->live_thr_num>pool->min_thr_num){
            printf("thread 0x%x is exiting\n",(unsigned int)pthread_self());
            pool->live_thr_num--;
            pthread_mutex_unlock(&(pool->lock));

            pthread_exit(NULL);
        }
    }
}

void *adjust_thread(void *threadpool)
{
    int i;
    threadpool_t *pool = (threadpool_t *)threadpool;
    while(!pool->shutdown){
        sleep(DEFAULT_TIME);

        pthread_mutex_lock(&(pool->lock));
        int queue_size=pool->queue_size;
        int live_thr_num = pool->live_thr_num;
        pthread_mutex_unlock(&pool->lock);

        pthread_mutex_lock(&(pool->thread_counter));
        int busy_thr_num = (&(pool->thread_counter));

        if(queue_size>=MIN_WAIT_TASK_NUM&&)
    }
}