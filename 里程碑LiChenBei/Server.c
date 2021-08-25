#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <assert.h>
#include <signal.h>
#include<pthread.h>
#include "threadpool.h"
#include<cjson/cJSON.h>
#include "Data.h"
#include<mysql/mysql.h>

void *threadpool_thread(void *threadpool);

void *adjust_thread(void *threadpool);

int is_thread_alive(pthread_t tid);
int threadpool_free(threadpool_t *pool);

//threadpool_create(3,100,100);

#define MAX_EVENTS 1024
#define BUFLEN 1024
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

MYSQL mysqlx, *mysql = &mysqlx;
int g_efd;
struct myevent_s g_events[MAX_EVENTS + 1];

void database_init()
{
    mysql_init(mysql);
    mysql_library_init(0, NULL, NULL);
    mysql_real_connect(mysql, "127.0.0.1", "root", "123456", "MICQ", 0, NULL, 0);
    mysql_set_character_set(mysql, "utf8");
}

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

void log_in(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char cmdStr[128];
    cJSON *UserID = cJSON_GetObjectItem(json, "uid");
    cJSON *Password = cJSON_GetObjectItem(json, "password");
    sprintf(cmdStr, "SELECT password FROM userinfo Where uid=%d;", (int)cJSON_GetNumberValue(UserID));
    mysql_query(mysql, cmdStr);
    result = mysql_store_result(mysql);
    row = mysql_fetch_row(result);
    if (row&&(strcmp(row[0], Password->valuestring) == 0))
    {
        cJSON_AddNumberToObject(Reply, "flag", RLY_LOG_IN);
        cJSON_AddNumberToObject(Reply, "result", 1);
        sprintf(cmdStr, "UPDATE userinfo SET online_stat = %d,ufd=%d WHERE uid=%d;", 1, ev->fd, (int)UserID->valuedouble);
        mysql_query(mysql, cmdStr);
    }
    else
    {
        cJSON_AddNumberToObject(Reply, "flag", RLY_LOG_IN);
        cJSON_AddNumberToObject(Reply, "result", 0);
    }
}
void reg(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    cJSON_AddNumberToObject(Reply,"flag",RLY_REG);
    char cmdStr[128];
    cJSON *UserID = cJSON_GetObjectItem(json, "uid");
    cJSON *Username = cJSON_GetObjectItem(json,"uname");
    cJSON *Password = cJSON_GetObjectItem(json, "password");
    sprintf(cmdStr,"SELECT uid FROM userinfo where uid=%d",(int)UserID->valuedouble);
    mysql_query(mysql,cmdStr);
    result=mysql_store_result(mysql);
    row=mysql_fetch_row(result);
    if(row==0){
        sprintf(cmdStr, "INSERT INTO userinfo SET uid=%d,uname=\"%s\",password=\"%s\",ufd=%d,online_stat=%d;", (int)UserID->valuedouble,Username->valuestring,Password->valuestring,ev->fd,1);
        mysql_query(mysql, cmdStr);
        cJSON_AddNumberToObject(Reply,"result",1);
    }else{
        cJSON_AddNumberToObject(Reply, "result", 0);
    }

}
void request_add(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    cJSON_AddNumberToObject(Reply, "flag", RLY_ADDQUEST_BY_SRV);
    char cmdStr[128]={0};
    cJSON * uid,*fid;
    uid=cJSON_GetObjectItem(json,"uid");
    fid=cJSON_GetObjectItem(json,"fid");

    char uname[20];
    sprintf(cmdStr, "SELECT uname FROM userinfo WHERE uid=%d;", (int)fid->valuedouble);
    mysql_query(mysql, cmdStr);
    result = mysql_store_result(mysql);
    row = mysql_fetch_row(result);
    strcpy(uname, row[0]);

    sprintf(cmdStr,"INSERT INTO friendlist SET uid=%d,fid=%d,black_stat=0,apply_stat=0,fname=\"%s\";",(int)uid->valuedouble,(int)fid->valuedouble,uname);
    mysql_query(mysql,cmdStr);
}

void look_addlist(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    cJSON * uid;
    cJSON_AddNumberToObject(Reply, "flag", RLY_LOOK_ADD);
    cJSON *Array=cJSON_AddArrayToObject(Reply,"uidarray");
    char cmdStr[128];
    uid=cJSON_GetObjectItem(json,"uid");
    sprintf(cmdStr,"SELECT uid FROM friendlist WHERE fid=%d and apply_stat=0;",(int)uid->valuedouble);
    mysql_query(mysql,cmdStr);
    result=mysql_store_result(mysql);
    while(row=mysql_fetch_row(result))
    {
        cJSON_AddItemToArray(Array,cJSON_CreateNumber(atoi(row[0])));
    }
}

void friendlist(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    cJSON *uid;
    cJSON_AddNumberToObject(Reply, "flag", RLY_FRIENDS_LIST);
    cJSON *Array = cJSON_AddArrayToObject(Reply, "friendlist");
    char cmdStr[128];
    uid = cJSON_GetObjectItem(json, "uid");
    sprintf(cmdStr, "SELECT userinfo.uid,userinfo.uname,userinfo.online_stat FROM userinfo join friendlist WHERE friendlist.uid=%d and friendlist.fid=userinfo.uid;", (int)uid->valuedouble);
    mysql_query(mysql, cmdStr);
    result = mysql_store_result(mysql);
    while (row = mysql_fetch_row(result))
    {
        cJSON_AddItemToArray(Array, cJSON_CreateNumber(atoi(row[0])));
        cJSON_AddItemToArray(Array, cJSON_CreateString(row[1]));
        cJSON_AddItemToArray(Array, cJSON_CreateNumber(atoi(row[2])));
    }
}

void agree_to_add(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    cJSON *uid,*fid;
    char cmdStr[128];
    char uname[20];
    cJSON_AddNumberToObject(Reply, "flag",RLY_ADDQUEST_BY_FRD);
    uid=cJSON_GetObjectItem(json,"uid");
    fid=cJSON_GetObjectItem(json,"fid");

    sprintf(cmdStr, "SELECT uname FROM userinfo WHERE uid=%d;", (int)fid->valuedouble);
    mysql_query(mysql, cmdStr);
    result=mysql_store_result(mysql);
    row=mysql_fetch_row(result);
    
    strcpy(uname,row[0]);

    sprintf(cmdStr,"UPDATE friendlist SET apply_stat=1 WHERE fid=%d and uid=%d;",(int)uid->valuedouble,(int)fid->valuedouble);
    mysql_query(mysql,cmdStr);
    sprintf(cmdStr, "INSERT INTO friendlist SET black_stat =0,apply_stat=1,uid=%d,fid=%d,fname=\"%s\";",
                        (int)uid->valuedouble, (int)fid->valuedouble,uname);
    mysql_query(mysql, cmdStr);
}

void sendmsg(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    cJSON *uid, *fid;
    char cmdStr[128];
    cJSON_AddNumberToObject(Reply, "flag", RLY_RECV_FRIEND_MESSAGE);
    uid = cJSON_GetObjectItem(json, "uid");
    fid = cJSON_GetObjectItem(json, "fid");
    cJSON *Array = cJSON_AddArrayToObject(Reply, "msglist");
    sprintf(cmdStr, "SELECT friendmsg.send_name,friendmsg.msg FROM friendmsg join friendlist WHERE friendmsg.recv_id=%d and friendmsg.send_id=%d and friendmsg.unread=1 and friendlist.apply_stat=1 and friendlist.black_stat=0;;", (int)uid->valuedouble, (int)fid->valuedouble);
    mysql_query(mysql, cmdStr);
    result=mysql_store_result(mysql);
    while (row = mysql_fetch_row(result))
    {
        cJSON_AddItemToArray(Array, cJSON_CreateString(row[0]));
        cJSON_AddItemToArray(Array, cJSON_CreateString(row[1]));
    }

    sprintf(cmdStr, "UPDATE friendmsg SET unread=0 WHERE recv_id=%d and send_id=%d and unread=1;", (int)uid->valuedouble, (int)fid->valuedouble);
    mysql_query(mysql, cmdStr);
}

void savemsg(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    cJSON *uid, *fid,*msg,*sendname;
    char cmdStr[128];
    char uname[20];
    //cJSON_AddNumberToObject(Reply, "flag", RLY_RECV_FRIEND_MESSAGE);
    uid = cJSON_GetObjectItem(json, "uid");
    fid = cJSON_GetObjectItem(json, "fid");
    msg = cJSON_GetObjectItem(json, "msg");
    sprintf(cmdStr,"SELECT uname FROM userinfo WHERE uid=%d;",(int)uid->valuedouble);
    mysql_query(mysql,cmdStr);
    result=mysql_store_result(mysql);
    row=mysql_fetch_row(result);
    strcpy(uname,row[0]);
    memset(cmdStr,0,sizeof(cmdStr));
    sprintf(cmdStr,"INSERT INTO friendmsg SET recv_id=%d,send_id=%d,send_name=\"%s\",msg=\"%s\",unread=1;",(int)fid->valuedouble,(int)uid->valuedouble,uname,msg->valuestring);
    mysql_query(mysql,cmdStr);
}

void create_group(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    cJSON_AddNumberToObject(Reply, "flag", SRV_CREATE_GROUP);
    char cmdStr[128];
    cJSON *uid = cJSON_GetObjectItem(json, "uid");
    cJSON *gname = cJSON_GetObjectItem(json, "gname");
    char uname[20];
    sprintf(cmdStr, "SELECT uname FROM userinfo WHERE uid=%d;", (int)uid->valuedouble);
    mysql_query(mysql, cmdStr);
    result = mysql_store_result(mysql);
    if(row){
    row = mysql_fetch_row(result);
    strcpy(uname, row[0]);
    sprintf(cmdStr, "INSERT INTO grouplist SET gname=\"%s\",uid=%d,mname=\"%s\",apply_stat=1,msg_id_record=0,position=2;",
            gname->valuestring, (int)uid->valuedouble, uname);
    mysql_query(mysql, cmdStr);
    sprintf(cmdStr,"INSERT INTO groupmsg SET gname=\"%s\",uid=%d,msg_id=0,mname=\"%s\",msg=\"创建成功\"",gname->valuestring,(int)uid->valuedouble,uname);
    }
    mysql_query(mysql,cmdStr);
}

void apply_add_group(cJSON *json, cJSON *Reply, struct myevent_s *ev) 
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    cJSON_AddNumberToObject(Reply,"flag",SRV_APPLY_ADD_GROUP);
    char cmdStr[128];
    cJSON* uid=cJSON_GetObjectItem(json,"uid");
    cJSON* gname=cJSON_GetObjectItem(json,"gname");
    char uname[20];
    sprintf(cmdStr, "SELECT uname FROM userinfo WHERE uid=%d;", (int)uid->valuedouble);
    mysql_query(mysql, cmdStr);
    result = mysql_store_result(mysql);
    row = mysql_fetch_row(result);
    strcpy(uname, row[0]);
    sprintf(cmdStr,"INSERT INTO grouplist SET gname=\"%s\",uid=%d,mname=\"%s\",apply_stat=0,msg_id_record=0,position=0;",
        gname->valuestring,(int)uid->valuedouble,uname);
    mysql_query(mysql, cmdStr);
}

void grouplist(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    cJSON_AddNumberToObject(Reply, "flag", SRV_GROUP_LIST);
    char cmdStr[128];
    cJSON *Array = cJSON_AddArrayToObject(Reply, "grouplist");
    cJSON *uid = cJSON_GetObjectItem(json, "uid");
    sprintf(cmdStr, "SELECT gname FROM grouplist WHERE uid=%d and apply_stat=1;", (int)uid->valuedouble);
    mysql_query(mysql, cmdStr);
    result = mysql_store_result(mysql);
    while (row = mysql_fetch_row(result))
    {
        cJSON_AddItemToArray(Array, cJSON_CreateString(row[0]));
    }
}

void group_msg_into_database(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result1,*result2;
    MYSQL_ROW row1,row2;
    cJSON_AddNumberToObject(Reply, "flag", SRV_SEND_GROUP_MESSAGE);
    char cmdStr[128];
    cJSON *uid,*gname,*msg;
    uid=cJSON_GetObjectItem(json,"uid");
    gname=cJSON_GetObjectItem(json,"gname");
    msg=cJSON_GetObjectItem(json,"msg");
    int max_msg_id;
    char mname[20];

    sprintf(cmdStr, "SELECT MAX(msg_id) FROM groupmsg WHERE gname=\"%s\";",gname->valuestring);
    mysql_query(mysql,cmdStr);
    result1=mysql_store_result(mysql);
    row1 = mysql_fetch_row(result1);
    if (row1)
    {
        max_msg_id = atoi(row1[0]);
    }
    else
    {
        max_msg_id=0;
    }

    sprintf(cmdStr, "SELECT mname FROM grouplist WHERE uid=%d;",(int) uid->valuedouble);
    mysql_query(mysql, cmdStr);
    result2 = mysql_store_result(mysql);
    row2 = mysql_fetch_row(result2);
    if(row2){
    strcpy(mname,row2[0]);
    

    sprintf(cmdStr, "INSERT INTO groupmsg SET gname=\"%s\",uid=%d,mname=\"%s\",msg_id=%d,msg=\"%s\";",
                        gname->valuestring,(int)uid->valuedouble,mname,max_msg_id+1,msg->valuestring);
    mysql_query(mysql, cmdStr);
    }
}

void group_chat(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row,row2,row3;
    cJSON_AddNumberToObject(Reply, "flag", SRV_GROUP_CHAT);
    char cmdStr[128];
    cJSON *uid, *gname, *msg;
    uid = cJSON_GetObjectItem(json, "uid");
    gname = cJSON_GetObjectItem(json, "gname");

    
    sprintf(cmdStr, "SELECT MAX(msg_id) FROM groupmsg WHERE gname=\"%s\";", gname->valuestring);
    mysql_query(mysql, cmdStr);
    result = mysql_store_result(mysql);
    row = mysql_fetch_row(result);
    int max_msg_id = atoi(row[0]);

    printf("==============%d",max_msg_id);

    sprintf(cmdStr, "SELECT msg_id_record FROM grouplist WHERE uid=%d and gname=\"%s\";",(int)uid->valuedouble,gname->valuestring);
    mysql_query(mysql, cmdStr);
    result = mysql_store_result(mysql);
    row2 = mysql_fetch_row(result);
    int msg_id_reocord = atoi(row2[0]);
    printf("==============%d", msg_id_reocord);

    sprintf(cmdStr, "UPDATE grouplist SET msg_id_record=%d  WHERE uid = %d and gname =\"%s\";", max_msg_id,(int)uid->valuedouble, gname->valuestring);
    mysql_query(mysql, cmdStr);
    printf("%s\n",cmdStr);

    sprintf(cmdStr, "SELECT mname,msg FROM groupmsg WHERE gname =\"%s\" and (msg_id>%d and msg_id<=%d) and uid!=%d;", 
                      gname->valuestring,msg_id_reocord,max_msg_id,(int)uid->valuedouble);
    mysql_query(mysql,cmdStr);
    printf("%s\n",cmdStr);
    result = mysql_store_result(mysql);

    cJSON *Array=cJSON_AddArrayToObject(Reply,"msglist");
    while(row3 =mysql_fetch_row(result))
    {
        cJSON_AddItemToArray(Array,cJSON_CreateString(row3[0]));
        cJSON_AddItemToArray(Array,cJSON_CreateString(row3[1]));
    }
}
void check_group_apply(cJSON *json, cJSON *Reply, struct myevent_s *ev) 
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    cJSON_AddNumberToObject(Reply, "flag", SRV_ADMIN_CHECK_APPLY);
    char cmdStr[128];
    cJSON *uid, *gname, *msg;
    uid = cJSON_GetObjectItem(json, "uid");
    sprintf(cmdStr, "SELECT gname FROM grouplist WHERE uid=%d  and (position = 2 or position = 1);",(int)uid->valuedouble);
    mysql_query(mysql,cmdStr);
    result=mysql_store_result(mysql);
    int gname_num=0;
    char char_g_name[20][20];
    while(row=mysql_fetch_row(result))
    {
        strcpy(char_g_name[gname_num++],row[0]);
    }
    cJSON* Array=cJSON_AddArrayToObject(Reply,"namelist");
    for(int i=0;i<gname_num;i++){
        sprintf(cmdStr, "SELECT gname,mname FROM grouplist WHERE gname=\"%s\" and apply_stat=0;", char_g_name[i]);
        mysql_query(mysql, cmdStr);
        result = mysql_store_result(mysql);
        while(row=mysql_fetch_row(result)){
            cJSON_AddItemToArray(Array,cJSON_CreateString(row[0]));
            cJSON_AddItemToArray(Array, cJSON_CreateString(row[1]));
        }
    }
}

void reply_group_apply(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    printf("333333333333\n");
    MYSQL_RES *result;
    MYSQL_ROW row;
    cJSON_AddNumberToObject(Reply, "flag", SRV_ADMIN_REPLY_APPLY);
    char cmdStr[128];
    cJSON *uname, *gname;
    uname=cJSON_GetObjectItem(json,"uname");
    gname=cJSON_GetObjectItem(json,"gname");
    printf("44444444444\n");
    sprintf(cmdStr,"UPDATE grouplist SET apply_stat = 1 where gname=\"%s\" and mname=\"%s\";",gname->valuestring,uname->valuestring);
    mysql_query(mysql,cmdStr);
    printf("%s\n",cmdStr);
    printf("22222222\n");
}

void ready_to_recv_file(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    cJSON_AddNumberToObject(Reply, "flag", SRV_READY_RECV_FILE_TO_SRV);
    char cmdStr[128];
    cJSON * recv_id,*send_id,*filesize,*filename;
    recv_id=cJSON_GetObjectItem(json,"recv_id");
    send_id=cJSON_GetObjectItem(json,"send_id");
    filename=cJSON_GetObjectItem(json,"filename");
    filesize=cJSON_GetObjectItem(json,"filesize");


    int fd=open(filename->valuestring,O_WRONLY|O_CREAT,0644);
    ftruncate(fd,(int)filesize->valuedouble);
    cJSON_AddNumberToObject(Reply,"fd",fd);

    sprintf(cmdStr,"INSERT INTO filelist SET send_id=%d,recv_id=%d,file_name=\"%s\",file_size=%d,srv_recv=1,cli_recv=0;",(int)send_id->valuedouble,(int)recv_id->valuedouble,filename->valuestring,(int)filesize->valuedouble);
    printf("%s\n",cmdStr);
    mysql_query(mysql,cmdStr);
}

void to_recv_file(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{

    MYSQL_RES *result;
    MYSQL_ROW row;
    char cmdStr[128];

    cJSON_AddNumberToObject(Reply, "flag", 990);
    cJSON *fd=cJSON_GetObjectItem(json,"fd");
    cJSON *offset=cJSON_GetObjectItem(json,"offset");
    cJSON *data=cJSON_GetObjectItem(json,"data");
    cJSON *datasize=cJSON_GetObjectItem(json,"datasize");
    
    write((int)fd->valuedouble,data->valuestring,(int)datasize->valuedouble);


}

void closefd(cJSON *json, cJSON *Reply, struct myevent_s *ev){
    cJSON_AddNumberToObject(Reply, "flag", 990);
    cJSON* ffd=cJSON_GetObjectItem(json,"fd");
    int fd=(int)ffd->valuedouble;
    close(fd);
}

void cli_look_to_filelist(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    cJSON_AddNumberToObject(Reply, "flag", SRV_SEND_FILE_INFO);
    char cmdStr[128];
    cJSON *recv_id, *send_id, *filesize, *filename,*uid;
    uid=cJSON_GetObjectItem(json,"uid");
    sprintf(cmdStr, "SELECT send_id,file_name from filelist where recv_id=%d and cli_recv=0;", (int)uid->valuedouble);
    mysql_query(mysql,cmdStr);
    printf("%s\n",cmdStr);
    result=mysql_store_result(mysql);
    cJSON *Array =cJSON_AddArrayToObject(Reply,"filelist");
    while(row=mysql_fetch_row(result)){
        cJSON_AddItemToArray(Array,cJSON_CreateString(row[0]));
        cJSON_AddItemToArray(Array, cJSON_CreateString(row[1]));
    }
}
void srv_send_file_info_to_cli(cJSON *json, cJSON *Reply, struct myevent_s *ev) 
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    cJSON_AddNumberToObject(Reply, "flag", SRV_READY_TO_RECV);
    char cmdStr[128],path[256];
    cJSON *filename=cJSON_GetObjectItem(json,"filename");
    sprintf(cmdStr,"UPDATE TABLE filelist SET cli_recv = 1 where filename=\"%s\"",filename->valuestring);
    sprintf(path,"./%s",filename->valuestring);
    int fd = open(path,O_RDONLY);
    struct stat buf;
    fstat(fd,&buf);
    cJSON_AddNumberToObject(Reply,"fd",fd);
    cJSON_AddNumberToObject(Reply,"filesize",buf.st_size);

}
void sendfile(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    cJSON_AddNumberToObject(Reply, "flag", SRV_SEND_FILE);
    cJSON *fd=cJSON_GetObjectItem(json,"fd");
    char readbuf[512];
    int ret=read((int)fd->valuedouble,readbuf,sizeof(readbuf));
    cJSON_AddStringToObject(Reply,"data",readbuf);
}

void fin_sendfile(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    cJSON_AddNumberToObject(Reply, "flag", SRV_FIN_SEND_FILE);
    cJSON *fd = cJSON_GetObjectItem(json, "fd");
    close((int)fd->valuedouble);
}

void friend_chat_record(cJSON *json, cJSON *Reply, struct myevent_s *ev) 
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char cmdStr[128];
    cJSON_AddNumberToObject(Reply, "flag", SRV_CHAT_RECORD);
    cJSON *fid,*uid;
    fid=cJSON_GetObjectItem(json,"fid");
    uid=cJSON_GetObjectItem(json,"uid");
    sprintf(cmdStr,"SELECT send_name,time,msg from friendmsg where send_id=%d or send_id=%d ORDER BY time ASC;"
            ,(int)uid->valuedouble,(int)fid->valuedouble);
    mysql_query(mysql,cmdStr);
    printf("%s\n",cmdStr);
    cJSON *Array=cJSON_AddArrayToObject(Reply,"msglist");
    result=mysql_store_result(mysql);
    while(row=mysql_fetch_row(result)){
        cJSON_AddItemToArray(Array,cJSON_CreateString(row[0]));
        cJSON_AddItemToArray(Array, cJSON_CreateString(row[1]));
        cJSON_AddItemToArray(Array, cJSON_CreateString(row[2]));
    }
}
void blockfriend(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char cmdStr[128];
    cJSON_AddNumberToObject(Reply, "flag", SRV_BLOCK_FRIEND);
    cJSON *fid, *uid;
    fid = cJSON_GetObjectItem(json, "fid");
    uid = cJSON_GetObjectItem(json, "uid");

    sprintf(cmdStr, "UPDATE friendlist SET black_stat=1 WHERE uid=%d and fid=%d;",(int)uid->valuedouble,(int)fid->valuedouble);
    mysql_query(mysql,cmdStr);
}

void unblockfriend(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char cmdStr[128];
    cJSON_AddNumberToObject(Reply, "flag", SRV_UNBLOCK_FRIEND);
    cJSON *fid, *uid;
    fid = cJSON_GetObjectItem(json, "fid");
    uid = cJSON_GetObjectItem(json, "uid");

    sprintf(cmdStr, "UPDATE friendlist SET black_stat=0 WHERE uid=%d and fid=%d;", (int)uid->valuedouble, (int)fid->valuedouble);
    mysql_query(mysql, cmdStr);
}

void serch_friend(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char cmdStr[128];
    cJSON_AddNumberToObject(Reply, "flag", SRV_SERCH_FRIEND);
    cJSON *fname, *uid;
    fname =cJSON_GetObjectItem(json,"fname");
    uid=cJSON_GetObjectItem(json,"uid");
    sprintf(cmdStr,"SELECT fid,fname FROM friendlist WHERE fname=\"%s\" AND uid=%d;",fname->valuestring,(int)uid->valuedouble);
    mysql_query(mysql, cmdStr);
    printf("%s\n",cmdStr);
    result=mysql_store_result(mysql);
    cJSON *Array =cJSON_AddArrayToObject(Reply,"friendlist");
    while(row=mysql_fetch_row(result))
    {
            cJSON_AddItemToArray(Array,cJSON_CreateString(row[0]));
            cJSON_AddItemToArray(Array, cJSON_CreateString(row[1]));
    }
}

void delfriend(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char cmdStr[128];
    cJSON_AddNumberToObject(Reply, "flag", SRV_DEL_FRIEND);
    cJSON *fid, *uid;
    fid = cJSON_GetObjectItem(json, "fid");
    uid = cJSON_GetObjectItem(json, "uid");

    sprintf(cmdStr, "DELETE FROM friendlist WHERE (uid=%d and fid=%d) or (uid=%d and fid=%d);",
                 (int)uid->valuedouble, (int)fid->valuedouble,(int)fid->valuedouble, (int)uid->valuedouble);
    mysql_query(mysql, cmdStr);
}
void exitgroup(cJSON *json, cJSON *Reply, struct myevent_s *ev) 
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char cmdStr[128];
    cJSON_AddNumberToObject(Reply, "flag", SRV_EXIT_GROUP);
    cJSON * uid = cJSON_GetObjectItem(json,"uid");
    cJSON * gname = cJSON_GetObjectItem(json,"gname");
    sprintf(cmdStr,"DELETE FROM grouplist WHERE uid=%d,gname=\"%s\";",(int)uid->valuedouble,gname->valuestring);
    mysql_query(mysql,cmdStr);
    printf("%s\n",cmdStr);
}

void member_list(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char cmdStr[128];
    cJSON_AddNumberToObject(Reply, "flag", SRV_GROUP_MEMBER_LIST);
    cJSON *uid = cJSON_GetObjectItem(json, "uid");
    cJSON *gname = cJSON_GetObjectItem(json, "gname");
    sprintf(cmdStr, "SELECT uid,mname FROM grouplist WHERE gname=\"%s\" and apply_stat=1;",  gname->valuestring);
    mysql_query(mysql, cmdStr);
    printf("%s\n", cmdStr);
    cJSON * Array=cJSON_AddArrayToObject(Reply,"memberlist");
    result=mysql_store_result(mysql);
    while(row=mysql_fetch_row(result)){
        cJSON_AddItemToArray(Array,cJSON_CreateString(row[0]));
        cJSON_AddItemToArray(Array, cJSON_CreateString(row[1]));
    }

}

void add_admin(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char cmdStr[128];
    cJSON_AddNumberToObject(Reply, "flag", SRV_ADD_ADMIN);
    cJSON *uid = cJSON_GetObjectItem(json, "uid");
    cJSON *gname = cJSON_GetObjectItem(json, "gname");
    cJSON *oid = cJSON_GetObjectItem(json,"oid");
    sprintf(cmdStr,"SELECT position FROM grouplist WHERE uid=%d and gname=\"%s\";",(int)uid->valuedouble,gname->valuestring);
    mysql_query(mysql,cmdStr);
    result=mysql_store_result(mysql);
    row=mysql_fetch_row(result);
    
    if(atoi(row[0])==2){
    sprintf(cmdStr, "UPDATE grouplist SET position =1 WHERE uid=%d;",(int)oid->valuedouble);
    mysql_query(mysql, cmdStr);
    printf("%s\n", cmdStr);
    cJSON_AddNumberToObject(Reply, "result", 1);
    }else{
        cJSON_AddNumberToObject(Reply, "result", 0);
    }

}

void kickout(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char cmdStr[128];
    cJSON_AddNumberToObject(Reply, "flag", SRV_KICK_OUT);
    cJSON *uid = cJSON_GetObjectItem(json, "uid");
    cJSON *gname = cJSON_GetObjectItem(json, "gname");
    cJSON *oid = cJSON_GetObjectItem(json, "oid");
    sprintf(cmdStr, "SELECT position FROM grouplist WHERE uid=%d and gname=\"%s\";", (int)uid->valuedouble, gname->valuestring);
    mysql_query(mysql, cmdStr);
    result = mysql_store_result(mysql);
    row = mysql_fetch_row(result);

    if (atoi(row[0]) >0)
    {
        sprintf(cmdStr, "DELETE FROM grouplist WHERE uid=%d and gname=\"%s\";", (int)oid->valuedouble,gname->valuestring);
        mysql_query(mysql, cmdStr);
        printf("%s\n", cmdStr);
        cJSON_AddNumberToObject(Reply, "result", 1);
    }
    else
    {
        cJSON_AddNumberToObject(Reply, "result", 0);
    }
}

void group_record(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char cmdStr[128];
    cJSON_AddNumberToObject(Reply, "flag", SRV_GROUP_RECORD);
    cJSON *gname, *uid;
    gname = cJSON_GetObjectItem(json, "gname");
    uid = cJSON_GetObjectItem(json, "uid");
    sprintf(cmdStr, "SELECT mname,time,msg from groupmsg where gname=\"%s\" ORDER BY time ASC;",gname->valuestring);
    mysql_query(mysql, cmdStr);
    printf("%s\n", cmdStr);
    cJSON *Array = cJSON_AddArrayToObject(Reply, "msglist");
    result = mysql_store_result(mysql);
    while (row = mysql_fetch_row(result))
    {
        cJSON_AddItemToArray(Array, cJSON_CreateString(row[0]));
        cJSON_AddItemToArray(Array, cJSON_CreateString(row[1]));
        cJSON_AddItemToArray(Array, cJSON_CreateString(row[2]));
    }
}

void disbandgroup(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char cmdStr[128];
    cJSON_AddNumberToObject(Reply, "flag", SRV_DISBAND_GROUP);
    cJSON *uid = cJSON_GetObjectItem(json, "uid");
    cJSON *gname = cJSON_GetObjectItem(json, "gname");
    sprintf(cmdStr, "SELECT position FROM grouplist WHERE uid=%d and gname=\"%s\";", (int)uid->valuedouble, gname->valuestring);
    mysql_query(mysql, cmdStr);
    result = mysql_store_result(mysql);
    row = mysql_fetch_row(result);

    if (atoi(row[0]) == 2)
    {
        sprintf(cmdStr, "DELETE FROM grouplist  WHERE gname=\"%s\";",gname->valuestring);
        mysql_query(mysql, cmdStr);
        printf("%s\n", cmdStr);
        sprintf(cmdStr, "DELETE FROM groupmsg  WHERE gname=\"%s\";", gname->valuestring);
        mysql_query(mysql, cmdStr);
        printf("%s\n", cmdStr);
        cJSON_AddNumberToObject(Reply, "result", 1);
    }
    else
    {
        cJSON_AddNumberToObject(Reply, "result", 0);
    }
}

void messagemanager(cJSON *json, cJSON *Reply, struct myevent_s *ev)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    char cmdStr[128];
    int msg_num=0,file_num=0;
    cJSON_AddNumberToObject(Reply, "flag", SRV_MESSAGE_MANAGER);
    cJSON *uid = cJSON_GetObjectItem(json, "uid");
    sprintf(cmdStr,"SELECT * FROM friendmsg where recv_id=%d and unread=1",(int)uid->valuedouble);
    mysql_query(mysql,cmdStr);
    result=mysql_store_result(mysql);
    while(row=mysql_fetch_row(result)){
        msg_num++;
    }
    sprintf(cmdStr, "SELECT * FROM filelist where recv_id=%d and cli_recv=0", (int)uid->valuedouble);
    mysql_query(mysql, cmdStr);
    result = mysql_store_result(mysql);
    while (row = mysql_fetch_row(result))
    {
        file_num++;
    }

    cJSON_AddNumberToObject(Reply,"msg_num",msg_num);
    cJSON_AddNumberToObject(Reply,"file_num",file_num);
}

void *recvdata(void *arg)
{
    struct myevent_s *ev = (struct myevent_s *)arg;
    int len;
    len = read(ev->fd, ev->buf, sizeof(ev->buf));
    char cmdStr[128];
    eventdel(g_efd, ev);

    if (len > 0)
    {

        /*一顿操作*/
        printf("[INFO]服务器已接收JSON包%s\n",ev->buf);
        cJSON * Reply=cJSON_CreateObject();
        cJSON * json=cJSON_Parse(ev->buf);
        cJSON * flag;
        flag=cJSON_GetObjectItem(json,"flag");
        int flags = (int)cJSON_GetNumberValue(flag);
        switch (flags)
        {
        case LOG_IN:
            log_in(json,Reply,ev);
            break;
        case REGISTER:
            reg(json,Reply,ev);
            break;
        case LOOK_ADDQUESET:
            look_addlist(json, Reply, ev);
            break;
        case REQUEST_TO_ADD_FRIENDS :
            request_add(json, Reply, ev);
            break;
        case AGREE_TO_ADD_FRIENDS:
            agree_to_add(json, Reply, ev);
            break;
        case REQUEST_FRIENDS_LIST:
            friendlist(json, Reply, ev);
            break;
        case RECV_FRIEND_MESSAGE:
            sendmsg(json, Reply, ev);
            break;
        case SEND_FRIEND_MESSAGE:
            savemsg(json, Reply, ev);
            break;
        case CLI_APPLY_ADD_GROUP:
            apply_add_group(json, Reply, ev);
            break;
        case CLI_CREATE_GROUP:
            create_group(json, Reply, ev);
            break;
        case CLI_GROUP_LIST:
            grouplist(json, Reply, ev);
            break;
        case CLI_SEND_GROUP_MESSAGE:
            group_msg_into_database(json, Reply, ev);
            break;
        case CLI_GROUP_CHAT:
            group_chat(json, Reply, ev);
            break;
        case CLI_ADMIN_CHECK_APPLY:
            check_group_apply(json, Reply, ev);
            break;
        case CLI_ADMIN_REPLY_APPLY:
            reply_group_apply(json, Reply, ev);
            break;
        case CLI_READY_SEND_FILE_TO_SRV:
            ready_to_recv_file(json, Reply, ev);
            break;
        case CLI_SEND_FILE_TO_SRV:
            to_recv_file(json, Reply, ev);
            break;
        case FINSHED_TRANS_FILE_TO_SRV:
            closefd(json, Reply, ev);
            break;
        case CLI_LOOK_TO_RECV:
            cli_look_to_filelist(json, Reply, ev);
            break;
        case CLI_READY_TO_RECV:
            srv_send_file_info_to_cli(json, Reply, ev);
            break;
        case CLI_RECV_FILE:
            sendfile(json, Reply, ev);
            break;
        case CLI_FIN_RECV_FILE:
            fin_sendfile(json, Reply, ev);
            break;
        case CLI_CHAT_RECORD:
            friend_chat_record(json, Reply, ev);
            break;
        case CLI_BLOCK_FRIEND:
            blockfriend(json, Reply, ev);
            break;
        case CLI_UNBLOCK_FRIEND:
            unblockfriend(json, Reply, ev);
            break;
        case CLI_SERCH_FRIEND:
            serch_friend(json, Reply, ev);
            break;
        case CLI_DEL_FRIEND:
            delfriend(json, Reply, ev);
            break;
        case CLI_EXIT_GROUP:
            exitgroup(json, Reply, ev);
            break;
        case CLI_GROUP_MEMBER_LIST:
            member_list(json, Reply, ev);
        case CLI_ADD_ADMIN:
            add_admin(json, Reply, ev);
            break;
        case CLI_GROUP_RECORD:
            group_record(json, Reply, ev);
            break;
        case CLI_DISBAND_GROUP:
            disbandgroup(json, Reply, ev);
            break;
        case MESSAGE_MANAGER:
            messagemanager(json, Reply, ev);
            break;
        case CLI_KICK_OUT:
            kickout(json, Reply, ev);
            break;
            default:
            break;
        }
        strcpy(ev->buf,cJSON_Print(Reply));
        printf("[INFO]已从服务器发出JSON包%s\n",ev->buf);
        eventset(ev, ev->fd, *senddata, ev);
        eventadd(g_efd, EPOLLOUT|EPOLLET, ev);
    }
    else if (len == 0)
    {
        close(ev->fd);
        sprintf(cmdStr, "UPDATE userinfo SET online_stat =0,ufd=NULL WHERE ufd=%d;", ev->fd);
        mysql_query(mysql, cmdStr);
        printf("[INFO] Clinet exit\n");
    }
    else
    {
        sprintf(cmdStr, "UPDATE userinfo SET online_stat =0,ufd=NULL WHERE ufd=%d;", ev->fd);
        mysql_query(mysql, cmdStr);
        printf("[ERROR]Client Error\n");
        close(ev->fd);
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
    database_init();
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
