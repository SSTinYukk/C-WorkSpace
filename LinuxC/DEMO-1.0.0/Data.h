#include<cjson/cJSON.h>
#include "b64.h"
#define  LOG_IN                     1       //用户ID+密码
#define  RLY_LOG_IN                 101     //BOOL

#define  REGISTER                   2       //用户ID+用户名+密码
#define  RLY_REG                    102
#define  REQUEST_FRIENDS_LIST       3       //用户ID
#define  RLY_FRIENDS_LIST			103		//数组{好友ID+好友名+在线状态}

#define  REQUEST_TO_ADD_FRIENDS     4       //用户ID+目标ID  发送好友请求
#define	 RLY_ADDQUEST_BY_SRV	    104	 	//空	            服务器回复好友请求结果

#define  AGREE_TO_ADD_FRIENDS       5       //用户ID+目标ID+BOOL    同意添加好友
#define  RLY_ADDQUEST_BY_FRD		105		//用户名+BOOL

#define  LOOK_ADDQUESET				1051	//UID			  查看请求者列表
#define  RLY_LOOK_ADD				1052    //数组{请求者ID}


#define  DELETE_FRIENDS             6       //用户ID+目标ID
#define  RLY_DELETE_FRINEDS     	106		//仅仅标签

#define  RECV_FRIEND_MESSAGE        1071    //UID+FID 问一下服务器有消息没
#define  RLY_RECV_FRIEND_MESSAGE    10710   //是否有消息+FNAME+MSG  回送消息

#define  SEND_FRIEND_MESSAGE         7       //用户ID+目标ID+消息字符串


#define  JOIN_THE_BLACKLIST         8       //用户ID + 目标ID

#define  CHANGE_PASSWORD            9       //用户ID + 密码
#define  LOG_OUT                    10      //用户ID
#define  FIND_FRIENDS               11      //好友名

#define  UPDATA_MSG					999     //用户ID
#define  RLY_UPDATE				    9999    //CNT + CNT + CNT;

#define  CLI_CREATE_GROUP		    12       //创建者ID + 群名
#define  SRV_CREATE_GROUP           1201     //创建成功     

#define  CLI_APPLY_ADD_GROUP		13		 //申请者ID + 群名
#define  SRV_APPLY_ADD_GROUP  		14		 //提交申请成功
#define  CLI_ADMIN_REPLY_APPLY		15		 //申请者ID + 群名 + 成功与否
#define  SRV_ADMIN_REPLY_APPLY		16		 //处理申请成功
#define  CLI_ADMIN_CHECK_APPLY		1501	 //管理者ID
#define  SRV_ADMIN_CHECK_APPLY	    1601	 //数组{群名+请求者ID}

#define  CLI_EXIT_GROUP				17		 //退群申请者ID + 群名
#define  SRV_EXIT_GROUP				18		 //退群申请者ID + 群名

#define  CLI_GROUP_LIST    			19 		 //UID
#define  SRV_GROUP_LIST             20		 //群名

#define  CLI_GROUP_CHAT				21		 //UID+群名   询问有群消息没
#define  SRV_GROUP_CHAT				22  	 //数组{群成员名+消息


#define  CLI_SEND_GROUP_MESSAGE  	23		 //UID+群名+消息
#define  SRV_SEND_GROUP_MESSAGE		24 		 //标志位

#define  CLI_READY_SEND_FILE_TO_SRV 25       //send_id + recv_id + Filename + Filesize
#define  SRV_READY_RECV_FILE_TO_SRV 26       //开始你的表演

#define CLI_SEND_FILE_TO_SRV        27       //文件名+偏移量+数据段

#define FINSHED_TRANS_FILE_TO_SRV   28       // fd



#define CLI_LOOK_TO_RECV           29       //uid
#define SRV_SEND_FILE_INFO 30                //send_id ++ Filename 

#define CLI_READY_TO_RECV         31         //Filename
#define SRV_READY_TO_RECV 32                 //fd+Size

#define CLI_RECV_FILE               33         //fd
#define SRV_SEND_FILE               34        //文件名+偏移量+数据段

#define CLI_FIN_RECV_FILE           35         //fd
#define SRV_FIN_SEND_FILE           37         //990


#define CLI_BLOCK_FRIEND            38       //黑名单 uid+fid
#define SRV_BLOCK_FRIEND            39    

#define CLI_UNBLOCK_FRIEND          3801     //Un黑名单 uid+fid
#define SRV_UNBLOCK_FRIEND          3901     //

#define CLI_DEL_FRIEND              40      //删除uid+fid;
#define SRV_DEL_FRIEND              41        //

#define CLI_SERCH_FRIEND            42      //好友名
#define SRV_SERCH_FRIEND            43       //好友列表



#define CLI_CHAT_RECORD             44     //好友聊天记录uid+fid
#define SRV_CHAT_RECORD             45      //数组{用户名+MSG}

#define CLI_DISBAND_GROUP           46      //uid+gname
#define SRV_DISBAND_GROUP           47      //


#define CLI_GROUP_MEMBER_LIST       50        //gname
#define SRV_GROUP_MEMBER_LIST       51         //数组{gname+name+id+在线状态}

#define MESSAGE_MANAGER             52        //uid
#define SRV_MESSAGE_MANAGER         53   

#define CLI_ADD_ADMIN               54          //添加管理员 uid+gname+oid
#define SRV_ADD_ADMIN               55

#define CLI_DEL_ADMIN               56           //管理员 uid+gname+oid
#define SRV_DEL_ADMIN               57             

#define CLI_KICK_OUT                58      
#define SRV_KICK_OUT                59          

#define CLI_GROUP_RECORD            60       //uid+gname
#define SRV_GROUP_RECORD            61       //{mname time +msg};

#define PERMISSION_VERIFICATION     62      //群权限验证   uid+gname
#define SRV_PERMISSION_VERIFICATION 63      //result

int userid;
char name[10],password[10],msg[10];