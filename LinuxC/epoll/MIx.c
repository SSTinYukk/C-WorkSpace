#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <assert.h>
#include <signal.h>
#include<pthread.h>
#include "threadpool.h"

void *threadpool_thread(void *threadpool);

void *adjust_thread(void *threadpool);

int is_thread_alive(pthread_t tid);
int threadpool_free(threadpool_t *pool);

//threadpool_create(3,100,100);

#define MAX_EVENTS 1024
#define BUFLEN 4069
#define SERV_PORT 9999
#define TIME_OUT 1000

void *recvdata(void *arg);
void *senddata(void *arg);

#define DEFAULT_TIME 10        /*10s检测一次*/
#define MIN_WAIT_TASK_NUM 10   /*如果queue_size > MIN_WAIT_TASK_NUM 添加新的线程到线程池*/
#define DEFAULT_THREAD_VARY 10 /*每次创建和销毁线程的个数*/
#define true 1
#define false 0

typedef struct
{
    void *(*function)(void *); /* 函数指针，回调函数 */
    void *arg;                 /* 上面函数的参数 */
} threadpool_task_t;           /* 各子线程任务结构体 */

/* 描述线程池相关信息 */

struct threadpool_t
{
    pthread_mutex_t     lock;           /* 用于锁住本结构体 */
    pthread_mutex_t     thread_counter; /* 记录忙状态线程个数de琐 -- busy_thr_num */

    pthread_cond_t      queue_not_full;  /* 当任务队列满时，添加任务的线程阻塞，等待此条件变量 */
    pthread_cond_t      queue_not_empty; /* 任务队列里不为空时，通知等待任务的线程 */

    pthread_t           *threads;            /* 存放线程池中每个线程的tid。数组 */
    pthread_t           adjust_tid;          /* 存管理线程tid */
    threadpool_task_t   *task_queue; /* 任务队列(数组首地址) */

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


struct myevent_s
{
    int         fd; 
    int         events;
    void        *arg;
    void        *(*call_back)(void *arg);
    int         status;
    char        buf[BUFLEN];
    int         len;
    long        last_active;
};

int g_efd;
struct myevent_s g_events[MAX_EVENTS + 1];

void my_err(int ret, char *str)
{
    if (ret < 0)
    {
        perror(str);
        exit(1);
    }
    return;
}

void eventset(struct myevent_s *ev, int fd, void *(*call_back)(void *), void *arg)
{
    ev->fd = fd;
    ev->call_back = call_back;
    ev->events = 0;
    ev->len = 0;
    ev->arg = arg;
    ev->status = 0;
    ev->last_active = time(NULL);
    return;
}

void eventadd(int efd, int events, struct myevent_s *ev)
{
    struct epoll_event epv;
    int op;
    epv.events = ev->events = events;
    epv.data.ptr = ev;

    if (ev->status == 0)
    {
        op = EPOLL_CTL_ADD;
        ev->status = 1;
    }

    if (epoll_ctl(efd, op, ev->fd, &epv) < 0) //实际添加/修改
        printf("[INFO] event add failed [fd=%d], events[%d]\n", ev->fd, events);
    else
        printf("[INFO] event add OK [fd=%d], op=%d, events[%0X]\n", ev->fd, op, events);

    return;
}

void eventdel(int efd, struct myevent_s *ev)
{
    struct epoll_event epv;
    if (ev->status != 1)
    {
        return;
    }
    epv.data.ptr = NULL;
    ev->status = 0;

    epoll_ctl(efd, EPOLL_CTL_DEL, ev->fd, &epv);
    return;
}

void *accpetnonblock( void *arg)
{
    struct myevent_s *ev = (struct myevent_s *)arg;
    struct sockaddr_in cin;
    socklen_t slen = sizeof(cin);
    int cfd, i;

    cfd = accept(ev->fd, (struct sockaddr *)&cin, &slen);

    do
    {
        for (i = 0; i < MAX_EVENTS; i++)
        {
            if (g_events[i].status == 0)
            {
                break;
            }
        }
        if (i >= MAX_EVENTS)
        {
            printf("%s: max connect limit[%d]\n", __func__, MAX_EVENTS);
            break; //跳出do while(0) 不执行后续代码
        }
    } while (0);

    eventset(&g_events[i], cfd, *recvdata, &g_events[i]);
    eventadd(g_efd, EPOLLIN|EPOLLET, &g_events[i]);
}

void initlistensocket(int efd, short port)
{
    struct sockaddr_in sin;
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(lfd, F_SETFL, O_NONBLOCK);

    memset(&sin, 0, sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(SERV_PORT);

    bind(lfd, (struct sockaddr *)&sin, sizeof(sin));

    listen(lfd, 20);
    eventset(&g_events[MAX_EVENTS], lfd, *accpetnonblock, &g_events[MAX_EVENTS]);
    eventadd(efd, (EPOLLIN|EPOLLET), &g_events[MAX_EVENTS]);
    return;
}

void *recvdata(void *arg)
{
    struct myevent_s *ev = (struct myevent_s *)arg;
    int len;

    len = read(ev->fd, ev->buf, sizeof(ev->buf));

    eventdel(g_efd, ev);

    if (len > 0)
    {
        /*一顿操作*/
        for (int i = 0; i < len; i++)
        {
            ev->buf[i] = toupper(ev->buf[i]);
        }

        eventset(ev, ev->fd, *senddata, ev);
        printf("[INFO] Running successful\n");
        eventadd(g_efd, EPOLLOUT|EPOLLET, ev);
    }
    else if (len == 0)
    {
        close(ev->fd);
        printf("[INFO] Clinet exit\n");
    }
    else
    {
        close(ev->fd);
        printf("[INFO] Clinet exit\n");
    }
}

void *senddata(void *arg)
{
    struct myevent_s *ev = (struct myevent_s *)arg;
    int len;
    lseek(ev->fd, SEEK_SET, 0);
    printf("[INFO] STDOUT:%s", ev->buf);
    len = write(ev->fd, ev->buf, sizeof(ev->buf));
    memset(ev->buf,0,BUFLEN);
    eventdel(g_efd, ev);

    if (len > 0)
    {
        printf("[INFO]send[fd=%d], [%d]%s\n", ev->fd, len, ev->buf);
        eventset(ev, ev->fd, *recvdata, ev);
        eventadd(g_efd, EPOLLIN|EPOLLET, ev);
    }
    else
    {
        close(ev->fd);
    }
}
int incnt=0,outcnt=0;
int main(int argc, char **argv)
{
    threadpool_t *thp = threadpool_create(30, 100, 100); /*创建线程池，池里最小3个线程，最大100，队列最大100*/
    int i, cnt;
    g_efd = epoll_create(MAX_EVENTS + 1);
    if (g_efd < 0)
    {
        fprintf(stderr, "[ERROR]epoll_create error\n");
    }
    initlistensocket(g_efd, (unsigned short)SERV_PORT);
    struct epoll_event events[MAX_EVENTS + 1];

    while (1)
    {
        //printf("[INFO] %dth event\n",cnt++);
        int nfd = epoll_wait(g_efd, events, MAX_EVENTS, TIME_OUT);
        if (nfd < 0)
        {
            fprintf(stderr, "[ERROR]epoll_wait error\n");
            break;
        }
        for (i = 0; i < nfd; i++)
        {
            struct myevent_s *ev = (struct myevent_s *)events[i].data.ptr;
            if ((events[i].events & EPOLLIN) && (ev->events & EPOLLIN))
            {
                printf("入口计数=============%d\n",incnt++);
                threadpool_add(thp, ev->call_back,(void*)ev);
            }
            if ((events[i].events & EPOLLOUT) && (ev->events & EPOLLOUT))
            {
                printf("出口计数=============%d\n", outcnt++);
                threadpool_add(thp, ev->call_back,(void*)ev);
            }
        }
    }
}

threadpool_t *threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size)
{
    int i;
    threadpool_t *pool = NULL; /* 线程池 结构体 */

    do
    {
        if ((pool = (threadpool_t *)malloc(sizeof(threadpool_t))) == NULL)
        {
            printf("malloc threadpool fail");
            break; /*跳出do while*/
        }

        pool->min_thr_num = min_thr_num;
        pool->max_thr_num = max_thr_num;
        pool->busy_thr_num = 0;
        pool->live_thr_num = min_thr_num; /* 活着的线程数 初值=最小线程数 */
        pool->wait_exit_thr_num = 0;
        pool->queue_size = 0;                  /* 有0个产品 */
        pool->queue_max_size = queue_max_size; /* 最大任务队列数 */
        pool->queue_front = 0;
        pool->queue_rear = 0;
        pool->shutdown = false; /* 不关闭线程池 */

        /* 根据最大线程上限数， 给工作线程数组开辟空间, 并清零 */
        pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * max_thr_num);
        if (pool->threads == NULL)
        {
            printf("malloc threads fail");
            break;
        }
        memset(pool->threads, 0, sizeof(pthread_t) * max_thr_num);

        /* 给 任务队列 开辟空间 */
        pool->task_queue = (threadpool_task_t *)malloc(sizeof(threadpool_task_t) * queue_max_size);
        if (pool->task_queue == NULL)
        {
            printf("malloc task_queue fail");
            break;
        }

        /* 初始化互斥琐、条件变量 */
        if (pthread_mutex_init(&(pool->lock), NULL) != 0 || pthread_mutex_init(&(pool->thread_counter), NULL) != 0 || pthread_cond_init(&(pool->queue_not_empty), NULL) != 0 || pthread_cond_init(&(pool->queue_not_full), NULL) != 0)
        {
            printf("init the lock or cond fail");
            break;
        }

        /* 启动 min_thr_num 个 work thread */
        for (i = 0; i < min_thr_num; i++)
        {
            pthread_create(&(pool->threads[i]), NULL, threadpool_thread, (void *)pool); /*pool指向当前线程池*/
            printf("start thread 0x%x...\n", (unsigned int)pool->threads[i]);
        }
        pthread_create(&(pool->adjust_tid), NULL, adjust_thread, (void *)pool); /* 创建管理者线程 */

        return pool;

    } while (0);

    threadpool_free(pool); /* 前面代码调用失败时，释放poll存储空间 */

    return NULL;
}

/* 向线程池中 添加一个任务 */
//threadpool_add(thp, process, (void*)&num[i]);   /* 向线程池中添加任务 process: 小写---->大写*/

int threadpool_add(threadpool_t *pool, void *(*function)(void *arg), void *arg)
{
    pthread_mutex_lock(&(pool->lock));

    /* ==为真，队列已经满， 调wait阻塞 */
    while ((pool->queue_size == pool->queue_max_size) && (!pool->shutdown))
    {
        pthread_cond_wait(&(pool->queue_not_full), &(pool->lock));
    }

    if (pool->shutdown)
    {
        pthread_cond_broadcast(&(pool->queue_not_empty));
        pthread_mutex_unlock(&(pool->lock));
        return 0;
    }

    /* 清空 工作线程 调用的回调函数 的参数arg */
    if (pool->task_queue[pool->queue_rear].arg != NULL)
    {
        pool->task_queue[pool->queue_rear].arg = NULL;
    }

    /*添加任务到任务队列里*/
    pool->task_queue[pool->queue_rear].function = function;
    pool->task_queue[pool->queue_rear].arg = arg;
    pool->queue_rear = (pool->queue_rear + 1) % pool->queue_max_size; /* 队尾指针移动, 模拟环形 */
    pool->queue_size++;

    /*添加完任务后，队列不为空，唤醒线程池中 等待处理任务的线程*/
    pthread_cond_signal(&(pool->queue_not_empty));
    pthread_mutex_unlock(&(pool->lock));

    return 0;
}

/* 线程池中各个工作线程 */
void *threadpool_thread(void *threadpool)
{
    threadpool_t *pool = (threadpool_t *)threadpool;
    threadpool_task_t task;

    while (true)
    {
        /* Lock must be taken to wait on conditional variable */
        pthread_mutex_lock(&(pool->lock));
        /*刚创建出线程，等待任务队列里有任务，否则阻塞等待任务队列里有任务后再唤醒接收任务*/

        /*queue_size == 0 说明没有任务，调 wait 阻塞在条件变量上, 若有任务，跳过该while*/
        while ((pool->queue_size == 0) && (!pool->shutdown))
        {
            printf("thread 0x%x is waiting\n", (unsigned int)pthread_self());
            pthread_cond_wait(&(pool->queue_not_empty), &(pool->lock));

            /*清除指定数目的空闲线程，如果要结束的线程个数大于0，结束线程*/
            if (pool->wait_exit_thr_num > 0)
            {
                pool->wait_exit_thr_num--;

                /*如果线程池里线程个数大于最小值时可以结束当前线程*/
                if (pool->live_thr_num > pool->min_thr_num)
                {
                    printf("thread 0x%x is exiting\n", (unsigned int)pthread_self());
                    pool->live_thr_num--;
                    pthread_mutex_unlock(&(pool->lock));

                    pthread_exit(NULL);
                }
            }
        }

        /*如果指定了true，要关闭线程池里的每个线程，自行退出处理---销毁线程池*/
        if (pool->shutdown)
        {
            pthread_mutex_unlock(&(pool->lock));
            printf("thread 0x%x is exiting\n", (unsigned int)pthread_self());
            pthread_detach(pthread_self());
            pthread_exit(NULL); /* 线程自行结束 */
        }

        /*从任务队列里获取任务, 是一个出队操作*/
        task.function = pool->task_queue[pool->queue_front].function;
        task.arg = pool->task_queue[pool->queue_front].arg;

        pool->queue_front = (pool->queue_front + 1) % pool->queue_max_size; /* 出队，模拟环形队列 */
        pool->queue_size--;

        /*通知可以有新的任务添加进来*/
        pthread_cond_broadcast(&(pool->queue_not_full));

        /*任务取出后，立即将 线程池琐 释放*/
        pthread_mutex_unlock(&(pool->lock));

        /*执行任务*/
        printf("thread 0x%x start working\n", (unsigned int)pthread_self());
        pthread_mutex_lock(&(pool->thread_counter)); /*忙状态线程数变量琐*/
        pool->busy_thr_num++;                        /*忙状态线程数+1*/
        pthread_mutex_unlock(&(pool->thread_counter));

        (*(task.function))(task.arg); /*执行回调函数任务*/
        //task.function(task.arg);                                              /*执行回调函数任务*/

        /*任务结束处理*/
        printf("thread 0x%x end working\n", (unsigned int)pthread_self());
        pthread_mutex_lock(&(pool->thread_counter));
        pool->busy_thr_num--; /*处理掉一个任务，忙状态数线程数-1*/
        pthread_mutex_unlock(&(pool->thread_counter));
    }

    pthread_exit(NULL);
}

/* 管理线程 */
void *adjust_thread(void *threadpool)
{
    int i;
    threadpool_t *pool = (threadpool_t *)threadpool;
    while (!pool->shutdown)
    {

        sleep(DEFAULT_TIME); /*定时 对线程池管理*/

        pthread_mutex_lock(&(pool->lock));
        int queue_size = pool->queue_size;     /* 关注 任务数 */
        int live_thr_num = pool->live_thr_num; /* 存活 线程数 */
        pthread_mutex_unlock(&(pool->lock));

        pthread_mutex_lock(&(pool->thread_counter));
        int busy_thr_num = pool->busy_thr_num; /* 忙着的线程数 */
        pthread_mutex_unlock(&(pool->thread_counter));

        /* 创建新线程 算法： 任务数大于最小线程池个数, 且存活的线程数少于最大线程个数时 如：30>=10 && 40<100*/
        if (queue_size >= MIN_WAIT_TASK_NUM && live_thr_num < pool->max_thr_num)
        {
            pthread_mutex_lock(&(pool->lock));
            int add = 0;

            /*一次增加 DEFAULT_THREAD 个线程*/
            for (i = 0; i < pool->max_thr_num && add < DEFAULT_THREAD_VARY && pool->live_thr_num < pool->max_thr_num; i++)
            {
                if (pool->threads[i] == 0 || !is_thread_alive(pool->threads[i]))
                {
                    pthread_create(&(pool->threads[i]), NULL, threadpool_thread, (void *)pool);
                    add++;
                    pool->live_thr_num++;
                }
            }

            pthread_mutex_unlock(&(pool->lock));
        }

        /* 销毁多余的空闲线程 算法：忙线程X2 小于 存活的线程数 且 存活的线程数 大于 最小线程数时*/
        if ((busy_thr_num * 2) < live_thr_num && live_thr_num > pool->min_thr_num)
        {

            /* 一次销毁DEFAULT_THREAD个线程, 隨機10個即可 */
            pthread_mutex_lock(&(pool->lock));
            pool->wait_exit_thr_num = DEFAULT_THREAD_VARY; /* 要销毁的线程数 设置为10 */
            pthread_mutex_unlock(&(pool->lock));

            for (i = 0; i < DEFAULT_THREAD_VARY; i++)
            {
                /* 通知处在空闲状态的线程, 他们会自行终止*/
                pthread_cond_signal(&(pool->queue_not_empty));
            }
        }
    }

    return NULL;
}

int threadpool_destroy(threadpool_t *pool)
{
    int i;
    if (pool == NULL)
    {
        return -1;
    }
    pool->shutdown = true;

    /*先销毁管理线程*/
    pthread_join(pool->adjust_tid, NULL);

    for (i = 0; i < pool->live_thr_num; i++)
    {
        /*通知所有的空闲线程*/
        pthread_cond_broadcast(&(pool->queue_not_empty));
    }
    for (i = 0; i < pool->live_thr_num; i++)
    {
        pthread_join(pool->threads[i], NULL);
    }
    threadpool_free(pool);

    return 0;
}

int threadpool_free(threadpool_t *pool)
{
    if (pool == NULL)
    {
        return -1;
    }

    if (pool->task_queue)
    {
        free(pool->task_queue);
    }
    if (pool->threads)
    {
        free(pool->threads);
        pthread_mutex_lock(&(pool->lock));
        pthread_mutex_destroy(&(pool->lock));
        pthread_mutex_lock(&(pool->thread_counter));
        pthread_mutex_destroy(&(pool->thread_counter));
        pthread_cond_destroy(&(pool->queue_not_empty));
        pthread_cond_destroy(&(pool->queue_not_full));
    }
    free(pool);
    pool = NULL;

    return 0;
}

int threadpool_all_threadnum(threadpool_t *pool)
{
    int all_threadnum = -1; // 总线程数

    pthread_mutex_lock(&(pool->lock));
    all_threadnum = pool->live_thr_num; // 存活线程数
    pthread_mutex_unlock(&(pool->lock));

    return all_threadnum;
}

int threadpool_busy_threadnum(threadpool_t *pool)
{
    int busy_threadnum = -1; // 忙线程数

    pthread_mutex_lock(&(pool->thread_counter));
    busy_threadnum = pool->busy_thr_num;
    pthread_mutex_unlock(&(pool->thread_counter));

    return busy_threadnum;
}

int is_thread_alive(pthread_t tid)
{
    int kill_rc = pthread_kill(tid, 0); //发0号信号，测试线程是否存活
    if (kill_rc == ESRCH)
    {
        return false;
    }
    return true;
}

/*测试*/

#if 1

#endif
