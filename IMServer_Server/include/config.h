#pragma once
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<pthread.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/fcntl.h>
#include<signal.h>
#include<arpa/inet.h>
#include<time.h>
#include<sys/epoll.h>
#define TIME_OUT 2
#define EPOLL_MAX 1024
#define PORT 12345
//int epfd;//监听树描述符
//pthread_mutex_t accept_lock;
//任务
typedef struct
{
    void* (*Business)(void*,void*);
    void* arg;
    void* arg1;
}task_t;
//任务队列
typedef struct
{
    int max;
    int cur;
    int rear;
    int front;
    task_t* list;

}container_t;
//线程池
typedef struct
{
    int max;//线程最大数量
    int min;//线程最小数量
    int que_max;//任务队列最大数量
    int thread_alive;//存活线程数量
    int thread_busy;//活动线程数量
    container_t* ct;//任务队列
    pthread_t* customer_tid;//消费者线程数组
    pthread_mutex_t lock;//
    pthread_cond_t NOT_FULL;//未满条件变量
    pthread_cond_t NOT_EMPTY;//未空条件变量
    pthread_t manager_tid;//管理者线程
    int shutdown;//线程池开关
    int exitcode;//退出ma
}thread_pool_t;

//创建线程池
thread_pool_t* thread_pool_create(int,int,int);
//添加任务
int producer_job_add(thread_pool_t*,task_t);
//消费者任务
void* customer_job(void*);
//管理者任务
void* manager_job(void*);
//线程池销毁
int thread_pool_destroy(thread_pool_t*);
//判断线程是否在忙
int thread_if_busy(pthread_t);
//初始化网络
int net_init();
//任务
void* Business(void*);
//创建监听树
int epfd_create(int);
//接受链接
void* accept_business(void*,void*);
//处理请求
void* deal_business(void*,void*);
//监听
void epoll_listen(int,int,thread_pool_t*,void*);



//端口号
#pragma once
#include<stdio.h>
#define DEF_UDP_PORT (54325)
#define DEF_TCP_PORT (54321)
#define DEF_TCP_SERVER_IP ("10.56.53.247")
#define DEF_UDP_SERVER_IP ("10.56.53.247")
//接收缓冲区大小
#define DEF_BUFFER_SIZE (4096)

//密码，电话最大大小
#define DEF_MAX_SIZE       (100)
#define DEF_PROTOCAL_COUNT (10)
#define Def_CONTENT_SIZE    (1024)
//音视频最大大小
#define DEF_MAX_VIDEO_SIZE (20480)
//文件最大大小
#define DEF_MAX_FILE_RQ_SIZE (1024 * 9)
#define DEF_MAX_FILE_SIZE  (1024 * 8)
//名字大小
#define DEF_NAME_SIZE (96)
//聊天内容大小


//协议映射表大小
#define DEF_PROTOCAL_MAX  (100)
#define DEF_PROTOCAL_COUNT (10)
//定义协议头
#define DEF_PROTOCAL_BASE (1000)
//上线
#define DEF_UDP_ONLINE_RQ (DEF_PROTOCAL_BASE+1)
#define DEF_UDP_ONLINE_RS (DEF_PROTOCAL_BASE+2)
//下线
#define DEF_UDP_OFFLINE_RQ (DEF_PROTOCAL_BASE+3)
//聊天
#define DEF_UDP_CHAT_RQ (DEF_PROTOCAL_BASE+4)


//传文件
// 
// 
// 
//注册
#define DEF_TCP_REGISTER_RQ (DEF_PROTOCAL_BASE + 10)
#define DEF_TCP_REGISTER_RS (DEF_PROTOCAL_BASE + 11)
//登录
#define DEF_TCP_LOGIN_RQ (DEF_PROTOCAL_BASE + 12)
#define DEF_TCP_LOGIN_RS (DEF_PROTOCAL_BASE + 13)
//好友信息
#define DEF_TCP_FRIEND_INFO (DEF_PROTOCAL_BASE + 14)
//添加好友
#define DEF_TCP_ADD_FRIEND_RQ (DEF_PROTOCAL_BASE + 15)
#define DEF_TCP_ADD_FRIEND_RS (DEF_PROTOCAL_BASE + 16)
//聊天
#define DEF_TCP_CHAT_RQ (DEF_PROTOCAL_BASE + 17)
#define DEF_TCP_CHAT_RS (DEF_PROTOCAL_BASE + 18)
//下线
#define DEF_TCP_OFFLINE_RQ (DEF_PROTOCAL_BASE + 19)
//创建群组
#define DEF_TCP_CREATEGROUP_RQ (DEF_PROTOCAL_BASE + 20)
#define DEF_TCP_CREATEGROUP_RS (DEF_PROTOCAL_BASE + 21)
#define DEF_TCP_GROUP_INFO    (DEF_PROTOCAL_BASE + 22)
#define DEF_TCP_GROUP_CHAT_RQ (DEF_PROTOCAL_BASE + 23)
//加入群组
#define DEF_TCP_JOIN_GROUP_RQ (DEF_PROTOCAL_BASE + 24)
#define DEF_TCP_JOIN_GROUP_RS (DEF_PROTOCAL_BASE + 25)
//删除好友
#define DEF_TCP_DELETE_FRIEND_RQ (DEF_PROTOCAL_BASE + 29)
//聊天记录
#define DEF_TCP_CHAT_HISTORY_RQ (DEF_PROTOCAL_BASE + 30)
//视频聊天
#define DEF_UDP_VIDEO_RQ (DEF_PROTOCAL_BASE + 31)
//视频请求
#define DEF_TCP_VIDEO_INFO_RQ (DEF_PROTOCAL_BASE + 32)
#define DEF_TCP_VIDEO_INFO_RS (DEF_PROTOCAL_BASE + 33)

//返回结果
//注册请求返回结果
#define register_success (0)//注册成功
#define user_is_exit    (1)//用户已存在
#define parameter_error (2)//其他错误
#define register_fail  (3)
//登录请求返回结果
#define login_success    (0)//登录成功
#define password_error  (1)//密码错误
#define user_not_exist  (2)//用户不存在
#define parameter_login_error       (3) 
//添加好友请求结果
#define add_success      (0)//添加成功
#define no_this_user    (1)//没有此用户
#define user_refuse     (2)//用户拒绝
#define user_offline    (3)//用户不在线
#define add_fail         (4)
#define already_add    (5)//已经有此好友
//聊天信息返回结果
#define send_success     (0)//发送成功
#define send_error      (1)//发送失败
//好友状态
#define status_online   (1)//用户在线
#define status_offline  (0)//用户不在线
//创建群组请求
#define group_is_exit (0)
#define create_success (1)
#define create_error (2)
//群组聊天返回结果
#define group_send_error (0)
//加入群组返回结果
#define join_group_error (0)
#define join_group_refuse (1)
#define join_group_sucess (2)
#define no_this_group (3)
#define alreday_join (4)
#define creator_offline (5)

//定义结构体
//上线请求（协议头，姓名，头像，个性签名）
struct STRU_UDP_ONLINE 
{
	STRU_UDP_ONLINE():nType(DEF_UDP_ONLINE_RQ)
	{
		memset(nNmae, 0, DEF_NAME_SIZE);
	}
	int nType;
	char nNmae[DEF_NAME_SIZE];
};
//下线请求（协议头）
struct STRU_UDP_OFFLINE_RQ
{
	STRU_UDP_OFFLINE_RQ():nType(DEF_UDP_OFFLINE_RQ)
	{
	}
	int nType;
};
//聊天请求（协议头，内容）
struct STRU_UDP_CHAT_RQ
{
	STRU_UDP_CHAT_RQ() :nType(DEF_UDP_CHAT_RQ)
	{
		memset(nContent, 0, DEF_NAME_SIZE);
	}
	int nType;
	char nContent[DEF_NAME_SIZE];
};


//TCP结构体
typedef int PackType;
//注册,协议头，昵称，密码，手机号
struct STRU_TCP_REGISTER_RQ
{
	STRU_TCP_REGISTER_RQ() :type(DEF_TCP_REGISTER_RQ)
	{
		memset(name, 0, DEF_MAX_SIZE);
		memset(tel, 0, DEF_MAX_SIZE);
		memset(password, 0, DEF_MAX_SIZE);
	}
	PackType type;
	char name[DEF_MAX_SIZE];
	char tel[DEF_MAX_SIZE];
	char password[DEF_MAX_SIZE];
};
//注册回复，协议头,注册结果
struct STRU_TCP_REGISTER_RS
{
	STRU_TCP_REGISTER_RS() :type(DEF_TCP_REGISTER_RS), result(register_success)
	{
	}
	PackType type;
	int result;
};
//登录：协议头，电话号码，密码
struct STRU_TCP_LOGIN_RQ
{
	STRU_TCP_LOGIN_RQ() :type(DEF_TCP_LOGIN_RQ)
	{
		memset(tel, 0, DEF_MAX_SIZE);
		memset(password, 0, DEF_MAX_SIZE);
	}
	PackType type;
	char tel[DEF_MAX_SIZE];
	char password[DEF_MAX_SIZE];
};
//登录回复：协议头，登录结果,用户id
struct STRU_TCP_LOGIN_RS
{
	STRU_TCP_LOGIN_RS() :type(DEF_TCP_LOGIN_RS), result(login_success), userId(0)
	{
	}
	PackType type;
	int result;
	int userId;
};
//好友信息,协议头，昵称，用户状态，个性签名，头像，用户id
struct STRU_TCP_FRIEND_INFO
{
	STRU_TCP_FRIEND_INFO() :type(DEF_TCP_FRIEND_INFO), state(status_online), iconId(0), userId(0)
	{
		memset(name, 0, DEF_MAX_SIZE);
		memset(feling, 0, DEF_MAX_SIZE);
	}
	PackType type;
	char name[DEF_MAX_SIZE];
	char feling[DEF_MAX_SIZE];
	int state;
	int iconId;
	int userId;

};
//添加好友请求：协议头，用户昵称，用户id，被添加的用户昵称
struct STRU_TCP_ADD_FRIEND_RQ
{
	STRU_TCP_ADD_FRIEND_RQ() :type(DEF_TCP_ADD_FRIEND_RQ), userId(0), userIcon(0)
	{
		memset(userPhone, 0, DEF_MAX_SIZE);
		memset(friendPhone, 0, DEF_MAX_SIZE);
		memset(userName, 0, DEF_MAX_SIZE);
		memset(userFeel, 0, DEF_MAX_SIZE);
	}
	PackType type;
	char userPhone[DEF_MAX_SIZE];
	char friendPhone[DEF_MAX_SIZE];
	char userName[DEF_MAX_SIZE];
	char userFeel[DEF_MAX_SIZE];
	int userIcon;
	int userId;
};
//添加好友回复：协议头，被添加用户的昵称和id,添加结果,主动发起添加用户的id
struct STRU_TCP_ADD_FRIEND_RS
{
	STRU_TCP_ADD_FRIEND_RS() :type(DEF_TCP_ADD_FRIEND_RS), friendId(0), userId(0), result(add_success), friendIcon(0)
	{
		memset(friendName, 0, DEF_MAX_SIZE);
		memset(friendFeel, 0, DEF_MAX_SIZE);
	}
	PackType type;
	int result;
	char friendName[DEF_MAX_SIZE];
	char friendFeel[DEF_MAX_SIZE];
	int friendId;
	int friendIcon;
	int userId;
};
//聊天请求，协议头，聊天内容，接收者的id，发送者的id
struct STRU_TCP_CHAT_RQ
{
	STRU_TCP_CHAT_RQ() :type(DEF_TCP_CHAT_RQ), friendId(0), result(0), userId(0)
	{
		memset(content, 0, DEF_MAX_SIZE);
	}
	PackType type;
	int result;
	char content[Def_CONTENT_SIZE];
	int friendId;
	int userId;
	char time[Def_CONTENT_SIZE];
};
//聊天回复，发送结果，发送者和接受者的id
struct STRU_TCP_CHAT_RS
{
	STRU_TCP_CHAT_RS() :type(DEF_TCP_CHAT_RS), friendId(0), result(send_success), userId(0)
	{
	}
	PackType type;
	int result;
	int friendId;
	int userId;
};
//下线请求，协议头，用户id
struct STRU_TCP_OFFLINE_RQ
{
	STRU_TCP_OFFLINE_RQ() :type(DEF_TCP_OFFLINE_RQ), userId(0),result()
	{
	}
	PackType type;
	int result;
	int userId;
};
//创建群组请求
struct STRU_TCP_CREATEGROUP_RQ
{
	STRU_TCP_CREATEGROUP_RQ() :type(DEF_TCP_CREATEGROUP_RQ), userId(0)
	{
		memset(groupName, 0, DEF_MAX_SIZE);
	}
	PackType type;
	char groupName[DEF_MAX_SIZE];
	int userId;
};
///创建群组回复
struct STRU_TCP_CREATEGROUP_RS
{
	STRU_TCP_CREATEGROUP_RS() :type(DEF_TCP_CREATEGROUP_RS), result(2), groupId(-1), creatorId(0), groupIcon(0)
	{
		memset(groupName, 0, DEF_MAX_SIZE);
	}
	PackType type;
	int groupId;
	int result;
	char groupName[DEF_MAX_SIZE];
	int creatorId;
	int groupIcon;
};
//群组信息
struct STRU_TCP_GROUP_INFO
{
	STRU_TCP_GROUP_INFO() :type(DEF_TCP_GROUP_INFO), groupId(0),creatorId(0), groupIcon(0)
	{
		memset(groupName, 0, DEF_MAX_SIZE);
	}
	PackType type;
	int groupId;
	char groupName[DEF_MAX_SIZE];
	int creatorId;
	int groupIcon;
};
//群组聊天
struct STRU_TCP_GROUP_CHAT_RQ
{
	STRU_TCP_GROUP_CHAT_RQ() :type(DEF_TCP_GROUP_CHAT_RQ), groupId(0), userId(0)
	{
		memset(content, 0, DEF_MAX_SIZE);
		memset(userName, 0, DEF_MAX_SIZE);
	}
	PackType type;
	int groupId;
	int userId;
	char userName[DEF_MAX_SIZE];
	char content[DEF_MAX_SIZE];
};
//加入群组请求
struct STRU_TCP_JOIN_GROUP_RQ
{
	STRU_TCP_JOIN_GROUP_RQ() :type(DEF_TCP_JOIN_GROUP_RQ), userId(0), groupId(0)
	{
		memset(userName, 0, DEF_MAX_SIZE);
		memset(groupName, 0, DEF_MAX_SIZE);
	}
	PackType type;
	int userId;
	int groupId;
	char userName[DEF_MAX_SIZE];
	char groupName[DEF_MAX_SIZE];

};
//加入群组回复
struct STRU_TCP_JOIN_GROUP_RS
{
	STRU_TCP_JOIN_GROUP_RS() :type(DEF_TCP_JOIN_GROUP_RS), result(0), userId(0), groupId(0),groupIcon(0), creatorId(0)
	{
		memset(groupName, 0, DEF_MAX_SIZE);
	}
	PackType type;
	int result;
	int userId;
	int groupId;
	int groupIcon;
	char groupName[DEF_MAX_SIZE];
	int creatorId;
};


//文件信息
#define DEF_TCP_FILE_INFO_RQ (DEF_PROTOCAL_BASE + 26)
//文件块
#define DEF_TCP_FILE_BLOCK_RQ (DEF_PROTOCAL_BASE + 27)
#define DEF_TCP_FILE_INFO_RS (DEF_PROTOCAL_BASE + 28)
#define MAX_FILE_PATH (512)
#define MAX_FILE_CONTENT (8*1024)



#define wait_file (0)//等待响应
#define aggre_file (1)//同意接收
#define refuse_file (2)//拒绝接收
#define not_login (3)//不在线
//文件信息请求
struct STRU_FILE_INFO_RQ
{
	STRU_FILE_INFO_RQ():type(DEF_TCP_FILE_INFO_RQ), fileSize(0),friendId(0),userId(0)
	{
		memset(fileId, 0, MAX_FILE_PATH);
		memset(fileName, 0, MAX_FILE_PATH);
	}
	PackType type;//协议头
	char fileId[MAX_FILE_PATH];//文件标识符
	char fileName[MAX_FILE_PATH];//文件名
	long long fileSize;//文件大小
	int friendId;
	int userId;
};
//文件块
struct STRU_FILE_BLOCK_RQ
{
	STRU_FILE_BLOCK_RQ() :type(DEF_TCP_FILE_BLOCK_RQ), fileBlockSize(0),friendId(0),userId(0),result(0)
	{
		memset(fileId, 0, MAX_FILE_PATH);
		memset(fileContent, 0, MAX_FILE_CONTENT);
		memset(SHA_256, 0, 32);
	}
	PackType type;//协议头
	char fileId[MAX_FILE_PATH];//文件标识符
	char fileContent[MAX_FILE_CONTENT];
	int fileBlockSize;
	int friendId;
	int userId;
	char SHA_256[1024];
	int result;
};
//接收文件回复
struct STRU_FILE_INFO_RS
{
	STRU_FILE_INFO_RS() :type(DEF_TCP_FILE_INFO_RS), result(0), userId(0), friendId(0)
	{
	}
	PackType type;
	int result;
	int userId;
	int friendId;
};
//本地文件信息:文件标志，文件名，文件路径，当前位置，总大小，文件指针
struct FileInfo
{
	FileInfo() :pos(0), nFileSize(0),pFile(NULL)
	{
		memset(fileId, 0, MAX_FILE_PATH);
		memset(fileName, 0, MAX_FILE_PATH);
		memset(filePath, 0, MAX_FILE_PATH);
	}
	char fileId[MAX_FILE_PATH];//文件标识符
	char fileName[MAX_FILE_PATH];//文件名
	char filePath[MAX_FILE_PATH];
	long long pos;
	long long nFileSize;
	FILE *pFile;
};









////发送文件请求
//struct STRU_TCP_SEND_FILE_RQ
//{
//	STRU_TCP_SEND_FILE_RQ() :type(DEF_TCP_SEND_FILE_RQ), friendId(0), userId(0), ifend(false), ifbegin(false), fileSize(0), allSize(0)
//	{
//		memset(fileBuffer, 0, DEF_MAX_FILE_RQ_SIZE);
//		memset(fileName, 0, DEF_MAX_SIZE);
//	}
//	PackType type;
//	char fileBuffer[DEF_MAX_FILE_RQ_SIZE];
//	int friendId;
//	int userId;
//	bool ifend;
//	bool ifbegin;
//	char fileName[DEF_MAX_SIZE];
//	long long fileSize;
//	long long allSize;
//};

////文件信息请求
//struct STRU_TCP_SEND_FILE_INFO
//{
//	STRU_TCP_SEND_FILE_INFO() :type(DEF_TCP_SEND_FILE_INFO), fileSize(0),userId(0),friendId(0)
//	{
//		memset(fileName, 0, DEF_MAX_SIZE);
//	}
//	PackType type;
//	char fileName[DEF_MAX_SIZE];
//	long long fileSize;
//	int userId;
//	int friendId;
//};
//struct STRU_TCP_SEND_FILE_RQ
//{
//	STRU_TCP_SEND_FILE_RQ() :type(DEF_TCP_SEND_FILE_RQ), nBlockSize(0)
//	{
//		memset(fileBuffer, 0, DEF_MAX_FILE_SIZE);
//	}
//	PackType type;
//	char fileBuffer[DEF_MAX_FILE_SIZE];
//	int nBlockSize;
//};



#define delete_fail (0)
#define delete_success (1)
//删除好友请求
struct STRU_TCP_DELETE_FRIEND_RQ
{
	STRU_TCP_DELETE_FRIEND_RQ() :type(DEF_TCP_DELETE_FRIEND_RQ), userId(-1), friendId(-1),result(0)
	{

	}
	PackType type;
	int userId;
	int friendId;
	int result;
};
//聊天记录结构体
struct STRU_TCP_CHAT_HISTORY_RQ
{
	STRU_TCP_CHAT_HISTORY_RQ() :type(DEF_TCP_CHAT_HISTORY_RQ), userid1(0), userid2(0)
	{
		memset(infotime, 0, Def_CONTENT_SIZE);
		memset(info, 0, Def_CONTENT_SIZE);
	}
	PackType type;
	int userid1;
	int userid2;
	char infotime[Def_CONTENT_SIZE];
	char info[Def_CONTENT_SIZE];
};
//视频结构体
struct STRU_UDP_VIDEO_RQ
{
	STRU_UDP_VIDEO_RQ() :type(DEF_UDP_VIDEO_RQ), userid1(0), userid2(0)
	{
		memset(videoinfo, 0, DEF_MAX_VIDEO_SIZE);
	}
	PackType type;
	int userid1;
	int userid2;
	char videoinfo[DEF_MAX_VIDEO_SIZE];
};
struct STRU_TCP_VIDEO_INFO_RQ
{
	STRU_TCP_VIDEO_INFO_RQ() :type(DEF_TCP_VIDEO_INFO_RQ), userid1(0), userid2(0)
	{

	}
	PackType type;
	int userid1;
	int userid2;
};
struct STRU_TCP_VIDEO_INFO_RS
{
	STRU_TCP_VIDEO_INFO_RS() :type(DEF_TCP_VIDEO_INFO_RS), userid1(0), userid2(0), result(0)
	{

	}
	PackType type;
	int userid1;
	int userid2;
	int result;
};
