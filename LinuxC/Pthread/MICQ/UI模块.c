#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<mysql/mysql.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cjson/cJSON.h>
#include<pthread.h>
#include<string.h>

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
int uid,cfd;
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
        memset(sendbuf,0,sizeof(sendbuf));
        cJSON_AddNumberToObject(json,"flag",SEND_FRIEND_MESSAGE);
        cJSON_AddNumberToObject(json,"uid",uid);
        cJSON_AddNumberToObject(json,"fid",*fid);
        cJSON_AddStringToObject(json,"msg",msg);
        strcpy(sendbuf,cJSON_Print(json));
        send(cfd,sendbuf,sizeof(sendbuf),0);
        printf("[已发送]\n\n");
        
    }while(msg[0]!='\\'&&msg[1]!='q');
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
        getchar();
        friendlist();
        break;
    case 3:
        addfriend();
        getchar();
        friendlist();
        break;
    default:
        printf("输入序号:%d有误请重新输入...\n", key);
        break;
    }
}

void pri_msg_menu()
{
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
        searchfriend();
        getchar();
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
    default:
        printf("输入序号:%d有误请重新输入...\n", key);
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

void group_list()
{
    cJSON *json=cJSON_CreateObject();
    cJSON_AddNumberToObject(json,"flag",)
    printf("\033c");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("                      ☃ 欢迎进入MICQ群聊 ☃       \n");
    printf("                                                \n");
    printf("                            群名称                 \n");
    printf("                                                \n");
    printf("                        1.   折叠群列表           \n");
    printf("                        2.   进入群聊         \n");
    printf("                        3.   加入群组            \n");
    printf("                        4.   退出群组              \n");
    printf("                        5.   创建群组              \n");
    printf("                        6.   我管理的群            \n");
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
        log_ui();
        break;
    case 2:
        reg_ui();
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
    default:
        printf("输入序号:%d有误请重新输入...\n", key);
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
    printf("                        6.   我管理的群            \n");
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
        log_ui();
        break;
    case 2:
        reg_ui();
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