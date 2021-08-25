#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<mysql/mysql.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cjson/cJSON.h>
#include<pthread.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct _MessageReminder{
    int friend_msg_num;
    int grp_msg_num;
    int request_num;

} MessageReminder;

MessageReminder moss;

#include  "Data.h"

#define SERV_PORT 9999
    void
    frendlist();
void log_ui();
void searchfriend(); 
void pri_msg_menu();
void reg_ui();
void grp_menu();
void user_menu();
void queryFriends();
void friendlist();
void addfriend();
void sendfile();
int uid, cfd;
char temp[256];
char sendbuf[1024],recvbuf[1024];
MYSQL mysqlx,*mysql=&mysqlx;
int fd;
void log_menu(){

    printf("\033c");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("                      ☃ 欢迎进入MICQ聊天室 ☃       \n");
    printf("                                                \n");
    printf("                        1.   登录账号           \n");
    printf("                        2.   注册账号           \n");
    printf("                                                \n");
    printf("                                                 \n");
    printf("                        0.   退出程序              \n");

    int key;
    printf("请输入序号:");
    scanf("%d",&key);
    switch (key)
    {
    case 0:
        exit(1);
        break;
    case 1:
        log_ui();
        break;
    case 2:
        reg_ui();
    default:
        printf("输入序号:%d有误请重新输入...\n",key);
        sleep(1);
        break;
    }
}

void reg_ui()
{
    cJSON * json=cJSON_CreateObject();
    cJSON *REC = cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"flag",REGISTER);
    char username[20];
    char password[20];
    printf("\033c");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("                      ☃ 欢迎进入MICQ注册程序 ☃       \n");
    printf("                                                \n");
    printf("                     用户ID(限定10位以内数字):");
    scanf("%d",&uid);
    cJSON_AddNumberToObject(json,"uid",uid);
    printf("                     用户名(20位以内):");
    scanf("%s",username);
    cJSON_AddStringToObject(json,"uname",username);
    printf("                     密码(20位以内:");
    scanf("%s",password);
    cJSON_AddStringToObject(json, "password", password);
    strcpy(sendbuf,cJSON_Print(json));
    send(cfd,sendbuf,sizeof(sendbuf),0);
    memset(recvbuf,0,sizeof(recvbuf));

    if (recv(cfd, recvbuf, sizeof(recvbuf), 0)>0)
    {
        cJSON* result;
        REC=cJSON_Parse(recvbuf);
        result=cJSON_GetObjectItem(REC,"result");
        if((int)result->valuedouble==1)
        {
        printf("注册成功！即将进入用户菜单...\n");
        sleep(1);
        user_menu();
        }else{
            printf("用户名已经被占用，请重新选择输入\n");
            sleep(1);
            reg_ui();
        }
    }
}

void log_ui()
{
    cJSON *login =cJSON_CreateObject();
    cJSON *REC= cJSON_CreateObject(); 
    cJSON_AddNumberToObject(login,"flag",LOG_IN);
    char password[20];
    printf("\033c");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("                      ☃ 欢迎进入MICQ登录程序 ☃       \n");
    printf("                                                \n");
    printf("                     用户ID:");
    scanf("%d",&uid);
    cJSON_AddNumberToObject(login,"uid",uid);
    printf("                     密码:");
    scanf("%s", password);
    cJSON_AddStringToObject(login,"password",password);
    strcpy(sendbuf,cJSON_Print(login));
    send(cfd,sendbuf,sizeof(sendbuf),0);
    if(recv(cfd,recvbuf,sizeof(recvbuf),0)>0){
        REC=cJSON_Parse(recvbuf);
        cJSON * result=cJSON_GetObjectItem(REC,"result");
        if ((int)result->valuedouble)
        {
            printf("登录成功！即将进入用户菜单...");
            getchar();
            user_menu();
        }
    }
    
    
    
}

void user_menu(){

    printf("\033c");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("                      ☃ 欢迎进入MICQ聊天室 ☃       \n");
    printf("                                                \n");
    printf("                        1.   好友私聊           \n");
    printf("                        2.   MICQ群聊           \n");
    printf("                        3.   修改个人信息          \n");
    printf("                                                 \n");
    printf("                        0.   退出登录                \n");

    int key;

        printf("请输入序号:");
        scanf("%d", &key);
        switch (key)
        {
        case 0:
            log_menu();
            break;
        case 1:
            pri_msg_menu();
            break;
        case 2:
            grp_menu();
        default:
            printf("输入序号:%d有误请重新输入...\n", key);
            user_menu();
            break;
    }
}


void searchfriend()
{
    cJSON * json=cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"flag",FIND_FRIENDS);
    
    char friendName[20];
    printf("请输入你要查找的好友名:");
    scanf("%s",friendName);
    cJSON_AddStringToObject(json, "friend_name", friendName);
    
}

void addfriend()
{
    cJSON *json = cJSON_CreateObject();
    cJSON *REC =cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "flag", REQUEST_TO_ADD_FRIENDS);
    int fid;
    printf("请输入你要添加的好友ID:");
    scanf("%d",&fid);
    cJSON_AddNumberToObject(json, "fid", fid);
    cJSON_AddNumberToObject(json,"uid",uid);
    strcpy(sendbuf,cJSON_Print(json));
    send(cfd,sendbuf,sizeof(sendbuf),0);
    recv(cfd,recvbuf,sizeof(recvbuf),0);
    REC=cJSON_Parse(recvbuf);
    cJSON * flag;
    flag=cJSON_GetObjectItem(REC,"flag");
    if((int)flag->valuedouble==RLY_ADDQUEST_BY_SRV){
        printf("好友请求发送成功...\n");
    }
   
}

void lookrequest()
{
    cJSON *json = cJSON_CreateObject();
    cJSON *REC = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "flag", LOOK_ADDQUESET);
    cJSON_AddNumberToObject(json,"uid",uid);
    strcpy(sendbuf,cJSON_Print(json));
    send(cfd,sendbuf,sizeof(sendbuf),0);
    recv(cfd,recvbuf,sizeof(recvbuf),0);
    REC=cJSON_Parse(recvbuf);
    cJSON* Array=cJSON_GetObjectItem(REC,"uidarray");
    int Arraysize=cJSON_GetArraySize(Array);
    int i=0;
    for(i=0;i<Arraysize;i++){
        printf("好友申请发送者ID:%d\n",(int)cJSON_GetArrayItem(Array,i)->valuedouble);
    }
}

void friendquest()
{
    int fid;
    char Key[3];
    lookrequest();
    printf("请输入你要处理的好友请求的ID:");
    scanf("%d",&fid);
    cJSON *json = cJSON_CreateObject();
    cJSON *REC = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "flag", AGREE_TO_ADD_FRIENDS);
    cJSON_AddNumberToObject(json,"uid",uid);
    cJSON_AddNumberToObject(json,"fid",fid);
    printf("是否同意好友请求(Y/N):");
    scanf("%s",Key);
    if(Key[0]=='Y'||Key[0]=='y'){
        cJSON_AddNumberToObject(json,"result",1);
    }else{
        cJSON_AddNumberToObject(json, "result", 0);
    }
    strcpy(sendbuf,cJSON_Print(json));
    send(cfd,sendbuf,sizeof(sendbuf),0);
    recv(cfd,recvbuf,sizeof(recvbuf),0);
    REC=cJSON_Parse(recvbuf);
    cJSON* flags=cJSON_GetObjectItem(REC,"flag");
    if((int)flags->valuedouble){
        printf("添加好友成功\n");
        sleep(1);
    }else{
        printf("添加好友失败\n");
        sleep(1);
    }
}

void *recvchat(void *arg){
    int* fid =(int*)arg;
    while(1){
        cJSON *json=cJSON_CreateObject();

        cJSON_AddNumberToObject(json,"flag",RECV_FRIEND_MESSAGE);
        cJSON_AddNumberToObject(json,"uid",uid);
        cJSON_AddNumberToObject(json,"fid",*fid);
        strcpy(sendbuf,cJSON_Print(json));
        send(cfd,sendbuf,sizeof(sendbuf),0);
        recv(cfd,recvbuf,sizeof(recvbuf),0);
        cJSON *REC=cJSON_Parse(recvbuf);
        cJSON* ARRAY=cJSON_GetObjectItem(REC,"msglist");
        int ArraySize=cJSON_GetArraySize(ARRAY);
        for(int i=0;i<ArraySize;i=i+2){
            printf("%s:\n%s\n\n",cJSON_GetArrayItem(ARRAY,i)->valuestring,cJSON_GetArrayItem(ARRAY,i+1)->valuestring);
        }
        sleep(4);
    }
}


void chatPanel(int* fid)
{
    pthread_t pid;
    pthread_create(&pid,NULL,recvchat,(void*)fid);
    printf("\033c");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("                      ☃ 好友聊天面板 ☃       \n");

    char msg[256]={0};
    do{
        memset(msg,0,sizeof(msg));
        cJSON* json=cJSON_CreateObject();
        scanf("%s",msg);
        if (msg[0] == '#')
        {
            break;
        }
        memset(sendbuf,0,sizeof(sendbuf));
        cJSON_AddNumberToObject(json,"flag",SEND_FRIEND_MESSAGE);
        cJSON_AddNumberToObject(json,"uid",uid);
        cJSON_AddNumberToObject(json,"fid",*fid);
        cJSON_AddStringToObject(json,"msg",msg);
        strcpy(sendbuf,cJSON_Print(json));
        send(cfd,sendbuf,sizeof(sendbuf),0);
        printf("[已发送]\n\n");
        
    }while(msg[0]!='#');
    pthread_join(pid,NULL);
}


void friendlist()
{
    cJSON *json = cJSON_CreateObject();
    cJSON *REC ;
    cJSON_AddNumberToObject(json, "flag", REQUEST_FRIENDS_LIST);
    cJSON_AddNumberToObject(json, "uid", uid);
    strcpy(sendbuf,cJSON_Print(json));
    send(cfd,sendbuf,sizeof(sendbuf),0);
    recv(cfd,recvbuf,sizeof(recvbuf),0);
    REC = cJSON_Parse(recvbuf);
    cJSON *Array=cJSON_GetObjectItem(REC,"friendlist");
    int ArraySize=cJSON_GetArraySize(Array);

    printf("\033c");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("                      ☃ 好友列表 ☃       \n");
    printf("        好友ID      好友名      在线状态    \n");
    for (int i=0; i < ArraySize; i=i+3)
    {
    printf("         %d          %s           %d           \n",
        (int)cJSON_GetArrayItem(Array,i)->valuedouble,cJSON_GetArrayItem(Array,i+1)->valuestring,(int)cJSON_GetArrayItem(Array,i+2)->valuedouble);
    }
    printf("                                                 \n");

    printf("                        1.   折叠好友列表           \n");
    printf("                        2.   进入好友聊天面板           \n");
    printf("                        3.   添加好友            \n");
    printf("                        4.   删除好友         \n");
    printf("                        5.   查看好友请求       \n");
    printf("                        6.   发送文件            \n");
    printf("                        7.   接收文件            \n");
    printf("                                                \n");
    printf("                        0.   返回上一级 \n");
    int key;
    printf("请输入序号:");
    scanf("%d", &key);
    switch (key)
    {
    case 0:
        user_menu();
        break;
    case 1:
        pri_msg_menu();
        break;
    case 2:
        int fid;
        printf("请输入好友ID:");
        scanf("%d",&fid);
        chatPanel(&fid);
        friendlist();
        break;
    case 3:
        addfriend();
        getchar();
        friendlist();
        break;

    case 6:
        sendfile();
        sleep(1);
        friendlist();
        break;
    default:
        printf("输入序号:%d有误请重新输入...\n", key);
        break;
    }
}

void sendfile()
{
    int recv_id=0,offset=0;
    char dirpath[256]={0},filename[256]={0},path[256]={0};
    printf("请输入文件所在目录的绝对路径或相对目录: ");
    scanf("%s",dirpath);
    pid_t pid =fork();
    if(pid==-1){
        perror("fork error");
    }else if(pid==0){
        execlp("ls", dirpath, NULL);
    }
    usleep(1000);
    printf("请输入文件名: ");
    scanf("%s",filename);
    sprintf(path,"%s%s",dirpath,filename);
    
    printf("请输入接受者ID: ");
    scanf("%d",&recv_id);

    
    int ofd;
    ofd=open(path,O_RDONLY);
    if(ofd<0){
        perror("open");
    }
    struct stat buf;
    fstat(ofd,&buf);
    cJSON *json=cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "flag", CLI_READY_SEND_FILE_TO_SRV);
    cJSON_AddNumberToObject(json, "recv_id", recv_id);
    cJSON_AddNumberToObject(json, "send_id", uid);
    cJSON_AddStringToObject(json,"filename",filename);
    cJSON_AddNumberToObject(json, "filesize", (int)buf.st_size);
    memset(sendbuf,0,sizeof(sendbuf));
    strcpy(sendbuf,cJSON_Print(json));
    send(cfd,sendbuf,sizeof(sendbuf),0);
    memset(recvbuf,0,sizeof(recvbuf));
    cJSON *REC,*flag;
    do
    {
        read(cfd, recvbuf, sizeof(recvbuf));
        REC = cJSON_Parse(recvbuf);
        flag = cJSON_GetObjectItem(REC, "flag");
    }
    while ((int)(flag->valuedouble)==990);
    cJSON * srvfd=cJSON_GetObjectItem(REC,"fd");
    int server_fd;
    server_fd=(int)srvfd->valuedouble;

    char readbuf[512];
    int ret=0;
    do{
        memset(readbuf,0,sizeof(readbuf));
        cJSON * sendtosrv=cJSON_CreateObject();
        cJSON_AddNumberToObject(sendtosrv,"flag",CLI_SEND_FILE_TO_SRV);
        ret=read(ofd,readbuf,sizeof(readbuf));
        cJSON_AddNumberToObject(sendtosrv,"fd",server_fd);
        cJSON_AddNumberToObject(sendtosrv,"offset",offset);
        cJSON_AddStringToObject(sendtosrv,"data",readbuf);
        cJSON_AddNumberToObject(sendtosrv,"datasize",ret);
        offset=offset+ret;
        strcpy(sendbuf,cJSON_Print(sendtosrv));
        send(cfd,sendbuf,sizeof(sendbuf),0);
        recv(cfd,recvbuf,sizeof(recvbuf),0);
    }while(ret>0);
    cJSON *Fin=cJSON_CreateObject();
    cJSON_AddNumberToObject(Fin,"flag",FINSHED_TRANS_FILE_TO_SRV);
    cJSON_AddNumberToObject(Fin,"fd",server_fd);
    strcpy(sendbuf,cJSON_Print(Fin));
    send(cfd,sendbuf,sizeof(sendbuf),0);
    recv(cfd,recvbuf,sizeof(recvbuf),0);
    close(ofd);
}

void filerecv(){
    cJSON* json1 =cJSON_CreateObject();
    cJSON_AddNumberToObject(json1,"flag",CLI_LOOK_TO_RECV);
    cJSON_AddNumberToObject(json1,"uid",uid);
    strcpy(sendbuf,cJSON_Print(json1));
    send(cfd,sendbuf,sizeof(sendbuf),0);
    recv(cfd,recvbuf,sizeof(recvbuf),0);
    cJSON* REC1= cJSON_Parse(recvbuf);
    cJSON* Array=cJSON_GetObjectItem(REC1,"filelist");
    int Arraysize = cJSON_GetArraySize(Array);
    for(int i=0;i<Arraysize;i=i+2)
    {
        printf("发送者ID               文件名\n");
        printf("%-10s%-10s\n", cJSON_GetArrayItem(Array, i)->valuestring, cJSON_GetArrayItem(Array, i+1)->valuestring);
    }
    char filename[256];
    printf("请选择要接收的文件名:");
    scanf("%s",filename);
    cJSON * json2=cJSON_CreateObject();
    cJSON_AddNumberToObject(json2,"flag",CLI_READY_TO_RECV);
    cJSON_AddStringToObject(json2,"filename",filename);
    stpcpy(sendbuf,cJSON_Print(json2));
    send(cfd,sendbuf,sizeof(sendbuf),0);
    recv(cfd,recvbuf,sizeof(recvbuf),0);
    cJSON * REC2=cJSON_Parse(recvbuf);
    cJSON * filefd=cJSON_GetObjectItem(REC2,"fd");
    cJSON * filesize=cJSON_GetObjectItem(REC2,"filesize");

    char path[256];
    sprintf(path,"./%s",filename);
    int ofd = open(path, O_WRONLY | O_CREAT|O_TRUNC,0644);
    int offset = 0, size = (int)filesize->valuedouble;
    cJSON * json3=cJSON_CreateObject();
    cJSON_AddNumberToObject(json3, "flag", CLI_RECV_FILE);
    cJSON_AddNumberToObject(json3,"fd",(int)filefd->valuedouble);

    char writebuf[512];
    do{
        strcpy(sendbuf, cJSON_Print(json3));
        send(cfd,sendbuf,sizeof(sendbuf),0);
        recv(cfd,recvbuf,sizeof(recvbuf),0);
        cJSON *REC3=cJSON_Parse(recvbuf);
        strcpy(writebuf,cJSON_GetObjectItem(REC3,"data")->valuestring);
        int ret=write(ofd,writebuf,sizeof(writebuf));
        offset=offset+ret;
    
    }while(offset<size);
    cJSON *json4=cJSON_CreateObject();
    cJSON_AddNumberToObject(json4, "flag", CLI_FIN_RECV_FILE);
    cJSON_AddNumberToObject(json4,"fd",(int)filefd->valuedouble);
    strcpy(sendbuf,cJSON_Print(json4));
    send(cfd,sendbuf,sizeof(sendbuf),0);
    recv(cfd,recvbuf,sizeof(recvbuf),0);
    cJSON* REC4 =cJSON_Parse(recvbuf);
    if ((int)cJSON_GetObjectItem(REC4, "flag")->valuedouble == SRV_FIN_SEND_FILE){
        printf("文件接收成功！！\n");
    }
    close(ofd);
}

void pri_msg_menu()
{
    int fid;
    printf("\033c");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("                      ☃ 欢迎进入MICQ好友私聊 ☃       \n");
    printf("                                                \n");
    printf("                        1.   展开好友列表           \n");
    printf("                        2.   进入好友聊天面板       \n");
    printf("                        3.   添加好友            \n");
    printf("                        4.   删除好友         \n");
    printf("                        5.   查看好友请求       \n");
    printf("                        6.   发送文件            \n");
    printf("                        7.   接收文件            \n");
    printf("                                                \n");
    printf("                        0.   返回上一级 \n");
    int key;
    printf("请输入序号:");
    scanf("%d", &key);
    switch (key)
    {
    case 0:
        user_menu();
        break;
    case 1:
        friendlist();
        break;
    case 2:
        printf("请输入好友ID:");
        scanf("%d", &fid);
        chatPanel(&fid);
        pri_msg_menu();
        break;
    case 3:
        addfriend();
        getchar();
        pri_msg_menu();
        break;
    case 5:
        friendquest();
        pri_msg_menu();
        break;
    case 6:
        sendfile();
        sleep(1);
        pri_msg_menu();
        break;
    case 7:
        filerecv();
        sleep(1);
        pri_msg_menu();
        break;
    
    default : printf("输入序号:%d有误请重新输入...\n", key);
        break;
    }
}

void create_group()
{
    char groupname[20];
    cJSON *json = cJSON_CreateObject();
    cJSON *REC;
    printf("请输入你要创建的群名:");
    scanf("%s", groupname);
    cJSON_AddNumberToObject(json, "flag", CLI_CREATE_GROUP);
    cJSON_AddNumberToObject(json, "uid", uid);
    cJSON_AddStringToObject(json, "gname", groupname);
    strcpy(sendbuf, cJSON_Print(json));
    send(cfd, sendbuf, sizeof(sendbuf), 0);
    recv(cfd, recvbuf, sizeof(recvbuf), 0);
    REC = cJSON_Parse(recvbuf);
    if((int)(cJSON_GetObjectItem(REC,"flag")->valuedouble))
    {
        printf("创建成功\n");
        sleep(1);
    }
}

void reply_add_group()
{
    cJSON *json=cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"flag",CLI_ADMIN_CHECK_APPLY);
    cJSON_AddNumberToObject(json,"uid",uid);
    strcpy(sendbuf,cJSON_Print(json));
    send(cfd,sendbuf,sizeof(sendbuf),0);
    recv(cfd,recvbuf,sizeof(recvbuf),0);
    cJSON *REC=cJSON_Parse(recvbuf);
    cJSON *Array = cJSON_GetObjectItem(REC,"namelist");
    int ArraySize = cJSON_GetArraySize(Array);
    for(int i=0;i<ArraySize;i=i+2){
        printf("[群组%s]  用户%10s申请加入群组\n", cJSON_GetArrayItem(Array, i)->valuestring, cJSON_GetArrayItem(Array, i+1)->valuestring);
    }
    json=cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"flag",CLI_ADMIN_REPLY_APPLY);
    char gname[20],uname[20],YorN[20];
    printf("请输入要处理的群组名:");
    scanf("%s", gname);
    cJSON_AddStringToObject(json,"gname",gname);
    printf("请输入要处理的用户名:");
    scanf("%s",uname);
    cJSON_AddStringToObject(json,"uname",uname);
    printf("是否同意该用户加入群组(Y/N):");
    scanf("%s",YorN);
    if(YorN[0]=='Y'||YorN[0]=='y'){
        cJSON_AddNumberToObject(json,"result",1);
    }else{
        cJSON_AddNumberToObject(json, "result", 0);
    }
    strcpy(sendbuf,cJSON_Print(json));
    send(cfd,sendbuf,sizeof(sendbuf),0);
}

void apply_add_group()
{
    char groupname[20];
    cJSON *json=cJSON_CreateObject();
    cJSON *REC;
    printf("请输入你要加入的群名:");
    scanf("%s",groupname);
    cJSON_AddNumberToObject(json,"flag",CLI_APPLY_ADD_GROUP);
    cJSON_AddNumberToObject(json,"uid",uid);
    cJSON_AddStringToObject(json,"gname",groupname);
    strcpy(sendbuf,cJSON_Print(json));
    send(cfd,sendbuf,sizeof(sendbuf),0);
    recv(cfd,recvbuf,sizeof(recvbuf),0);
    REC=cJSON_Parse(recvbuf);
    if((int)(cJSON_GetObjectItem(REC,"flag")->valuedouble))
    {
        printf("申请成功！请等待对方同意\n");
        sleep(1);
    }
}

void *recv_group_msg(void *arg){
    char * gname=(char *)arg;
    char msgsend[1024];
    char msgrecv[1024];
    do
    {
        cJSON *json = cJSON_CreateObject();
        cJSON_AddNumberToObject(json, "flag", CLI_GROUP_CHAT);
        cJSON_AddNumberToObject(json, "uid", uid);
        cJSON_AddStringToObject(json, "gname", gname);
        strcpy(msgsend, cJSON_Print(json));
        send(cfd, msgsend, sizeof(msgsend), 0);
        recv(cfd, msgrecv,sizeof(msgrecv),0);
        cJSON *REC=cJSON_Parse(msgrecv);
        cJSON *flags=cJSON_GetObjectItem(REC,"flag");
        cJSON *Array=cJSON_GetObjectItem(REC,"msglist");
        int ArraySize=cJSON_GetArraySize(Array);
        for(int i=0;i<ArraySize;i=i+2){
            printf("%s:\n%s\n", cJSON_GetArrayItem(Array, i)->valuestring, cJSON_GetArrayItem(Array, i+1)->valuestring);
        }
        sleep(3);
    } while (1);
}

void group_chat()
{
    pthread_t pid;
    char gname[20];
    printf("请选择你要加入的群聊名:");
    scanf("%s",gname);
    printf("\033c");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("                      ☃ 欢迎进入MICQ群聊 ☃       \n");
    printf("                                                \n");
    char msg[256];
    pthread_create(&pid,NULL,recv_group_msg,(void *)gname);
    do{
        cJSON *json =cJSON_CreateObject();
        scanf("%s",msg);
        cJSON_AddNumberToObject(json,"flag",CLI_SEND_GROUP_MESSAGE);
        cJSON_AddNumberToObject(json,"uid",uid);
        cJSON_AddStringToObject(json,"gname",gname);
        cJSON_AddStringToObject(json,"msg",msg);
        strcpy(sendbuf,cJSON_Print(json));
        send(cfd,sendbuf,sizeof(sendbuf),0);
    }while(msg[0]!='#');
    pthread_join(pid,NULL);
}

void group_list()
{
    cJSON *json=cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"flag",CLI_GROUP_LIST);
    cJSON_AddNumberToObject(json,"uid",uid);
    strcpy(sendbuf,cJSON_Print(json));
    send(cfd,sendbuf,sizeof(sendbuf),0);
    recv(cfd,recvbuf,sizeof(recvbuf),0);
    cJSON *REC=cJSON_Parse(recvbuf);
    cJSON *Array =cJSON_GetObjectItem(REC,"grouplist");
    int ArraySize= cJSON_GetArraySize(Array);

    printf("\033c");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("                      ☃ 欢迎进入MICQ群聊 ☃       \n");
    printf("                                                \n");
    printf("                            群名称                 \n");
    for(int i=0;i<ArraySize;i++){
    printf("                            %-10s              \n",cJSON_GetArrayItem(Array,i)->valuestring);
    }
    printf("                                                \n");
    printf("                        1.   折叠群列表           \n");
    printf("                        2.   进入群聊         \n");
    printf("                        3.   加入群组            \n");
    printf("                        4.   退出群组              \n");
    printf("                        5.   创建群组              \n");
    printf("                        6.   群申请列表            \n");
    printf("                                                \n");
    printf("                        0.   返回上一级           \n");

    int key;
    printf("请输入序号:");
    scanf("%d", &key);
    switch (key)
    {
    case 0:
        user_menu();
        break;
    case 1:
        grp_menu();
        break;
    case 2:
        group_chat();
        sleep(1);
        group_list();
        break;
    case 3:
        apply_add_group();
        sleep(1);
        grp_menu();
        break;
    case 5:
        create_group();
        grp_menu();
        break;
    case 6:
        reply_add_group();
        sleep(1);
        group_list();
    default:
        printf("输入序号:%d有误请重新输入...\n", key);
        sleep(1);
        group_list();
        break;
    }
}

void grp_menu()
{
    printf("\033c");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("                      ☃ 欢迎进入MICQ群聊 ☃       \n");
    printf("                                                \n");
    printf("                        1.   展开群列表           \n");
    printf("                        2.   进入群聊         \n");
    printf("                        3.   加入群组            \n");
    printf("                        4.   退出群组              \n");
    printf("                        5.   创建群组              \n");
    printf("                        6.   群申请列表            \n");
    printf("                                                \n");
    printf("                        0.   返回上一级           \n");

    int key;
    printf("请输入序号:");
    scanf("%d", &key);
    switch (key)
    {
    case 0:
        user_menu();
        break;
    case 1:
        group_list();
        break;
    case 2:
        group_chat();
        sleep(1);
        grp_menu();
        break;
    case 3:
        apply_add_group();
        sleep(1);
        grp_menu();
        break;
    case 5:
        create_group();
        sleep(1);
        grp_menu();
        break;
    case 6:
        reply_add_group();
        sleep(1);
        grp_menu();

    default:
        printf("输入序号:%d有误请重新输入...\n", key);
        break;
    }
}



void database_init()
{
    mysql_init(mysql);
    mysql_library_init(0, NULL, NULL);
    mysql_real_connect(mysql, "127.0.0.1", "root", "123456", "MICQ", 0, NULL, 0);
    mysql_set_character_set(mysql, "utf8");
}

void network_init()
{
    int ret;
    struct sockaddr_in srv_addr;
    inet_pton(AF_INET, "127.0.0.1", &srv_addr.sin_addr.s_addr);
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(SERV_PORT);
    cfd=socket(AF_INET,SOCK_STREAM,0);
    ret=connect(cfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if(ret<0){
        perror("conect");
        exit(1);
    }
}
int main()
{
    network_init();
    log_menu();
}