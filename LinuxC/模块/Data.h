#include<cjson/cJSON.h>
#define  LOG_IN                     1       //用户ID+密码
#define  REP_LOG_IN                 101     //BOOL是否成功
#define  REGISTER                   2       //用户ID+用户名+密码
#define  REQUEST_FRIENDS_LIST       3       //用户ID
#define  REQUEST_TO_ADD_FRIENDS     4       //用户ID+目标ID
#define  AGREE_TO_ADD_FRIENDS       5       //用户ID+目标ID+BOOL
#define  DELETE_FRIENDS             6       //用户ID+目标ID
#define  FRIEND_MESSAGE             7       //用户ID+目标ID+消息字符串+时间
#define  JOIN_THE_BLACKLIST         8       //用户ID+目标ID
#define  CHANGE_PASSWORD            9       //用户ID+密码
#define  LOG_OUT                    10      //用户ID
#define  FIND_FRIENDS               11      //好友名

int userid;
char name[10],password[10],msg[10];
// void pack(void *argv,int flag)
// {
//     cJSON *json = cJSON_CreateObject();
//     cJSON_AddNumberToObject(json,"flag",flag);
//     switch (flag)
//     {
//     case LOG_IN:
//         cJSON_AddStringToObject(json,"username",name);
//         cJSON_AddStringToObject(json,"password",password);
//         cJSON_Print(json);
//         break;
    
//     default:
//         break;
//     }
//     cJSON* userid;
//      cJSON * username;
// }