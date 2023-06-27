#pragma once
#define _Def_UDP_PORT      (12345)
#define _Def_TCP_PORT      (54321)
#define _Def_BUFFER_SIZE   (4096)
#define _Def_TCP_SERVER_IP ("10.56.53.247") //无线10.56.6.43 //有线10.71.193.117
#define _Def_NAME_SIZE     (60 )
#define _Def_CONTENT_SIZE  (1024)
#define _DEF_MAX_SIZE       (100)	
#define _DEF_PROTOCAL_COUNT (10)
//协议头
#define _Def_PROTOCAL_BASE (1000)
//上线
#define _DEF_UDP_ONLINE_RQ  (_Def_PROTOCAL_BASE + 1)
#define _DEF_UDP_ONLINE_RS  (_Def_PROTOCAL_BASE + 2)
//聊天				   
#define _DEF_UDP_CHAT_RQ    (_Def_PROTOCAL_BASE + 3)

//下线
#define _DEF_UDP_OFFLINE_RQ (_Def_PROTOCAL_BASE + 4)

//传文件


//注册
#define _DEF_TCP_REGISTER_RQ (_Def_PROTOCAL_BASE + 10)
#define _DEF_TCP_REGISTER_RS (_Def_PROTOCAL_BASE + 11)
//登录
#define _DEF_TCP_LOGIN_RQ (_Def_PROTOCAL_BASE + 12)
#define _DEF_TCP_LOGIN_RS (_Def_PROTOCAL_BASE + 13)
//好友信息
#define _DEF_TCP_FRIEND_INFO (_Def_PROTOCAL_BASE + 14)
//添加好友
#define _DEF_TCP_ADD_FRIEND_RQ (_Def_PROTOCAL_BASE + 15)
#define _DEF_TCP_ADD_FRIEND_RS (_Def_PROTOCAL_BASE + 16)
//聊天
#define _DEF_TCP_CHAT_RQ (_Def_PROTOCAL_BASE + 17)
#define _DEF_TCP_CHAT_RS (_Def_PROTOCAL_BASE + 18)
//下线
#define _DEF_TCP_OFFLINE_RQ (_Def_PROTOCAL_BASE + 19)

//返回结果
//注册请求返回结果
#define register_success (0)
#define user_is_exit    (1)
#define parameter_error (2)
#define register_fail  (3)
//登录请求返回结果
#define login_success    (0)
#define password_error  (1)
#define user_not_exist  (2)
#define parameter_login_error       (3)
//添加好友请求结果
#define add_success      (0)
#define no_this_user    (1)
#define user_refuse     (2)
#define user_offline    (3)
#define add_fail         (4)
//聊天信息返回结果
#define send_success     (0)
#define send_error      (1)
//好友状态
#define status_online   (0)
#define status_offline  (1)

//UDP 结构体
//上线请求&回复结构体，包括：协议头，ip，名字
struct STRU_ONLINE_RQ
{
	STRU_ONLINE_RQ():nType(_DEF_UDP_ONLINE_RQ)
	{
		memset(szName,0,_Def_NAME_SIZE);
	}
	int nType;
	char szName[_Def_NAME_SIZE];
};
//下线请求结构体，包括：协议头
struct STRU_OFFLINE_RQ
{
	STRU_OFFLINE_RQ():nType(_DEF_UDP_OFFLINE_RQ)
	{

	}
	int nType;
};
//聊天请求结构体，包括：协议头,聊天内容
struct STRU_CHAT_RQ
{
	STRU_CHAT_RQ():nType(_DEF_UDP_CHAT_RQ)
	{
		memset(szContent,0,_Def_CONTENT_SIZE);
	};
	int nType;
	char szContent[_Def_CONTENT_SIZE];
};

//TCP结构体
typedef int PackType;
//注册,协议头，昵称，密码，手机号
struct STRU_TCP_REGISTER_RQ
{
	STRU_TCP_REGISTER_RQ():type(_DEF_TCP_REGISTER_RQ)
	{
		memset(name,0,_DEF_MAX_SIZE);
		memset(tel,0,_DEF_MAX_SIZE);
		memset(password,0,_DEF_MAX_SIZE);
	}
	PackType type;
	char name[_DEF_MAX_SIZE];
	char tel[_DEF_MAX_SIZE];
	char password[_DEF_MAX_SIZE];
};
//注册回复，协议头,注册结果
struct STRU_TCP_REGISTER_RS
{
	STRU_TCP_REGISTER_RS():type(_DEF_TCP_REGISTER_RS),result(register_success)
	{
	}
	PackType type;
	int result;
};
//登录：协议头，电话号码，密码
struct STRU_TCP_LOGIN_RQ
{
    STRU_TCP_LOGIN_RQ():type(_DEF_TCP_LOGIN_RQ)
	{
		memset(tel,0,_DEF_MAX_SIZE);
		memset(password,0,_DEF_MAX_SIZE);
	}
	PackType type;
	char tel[_DEF_MAX_SIZE];
	char password[_DEF_MAX_SIZE];
};
//登录回复：协议头，登录结果,用户id
struct STRU_TCP_LOGIN_RS
{
    STRU_TCP_LOGIN_RS():type(_DEF_TCP_LOGIN_RS),result(login_success),userId(0)
	{
	}
	PackType type;
	int result;
	int userId;
};
//好友信息,协议头，昵称，用户状态，个性签名，头像，用户id
struct STRU_TCP_FRIEND_INFO
{
	STRU_TCP_FRIEND_INFO():type(_DEF_TCP_FRIEND_INFO),state(status_online),iconId(0),userId(0)
	{
		memset(name,0,_DEF_MAX_SIZE);
		memset(feling,0,_DEF_MAX_SIZE);
	}
	PackType type;
	char name[_DEF_MAX_SIZE];
	char feling[_DEF_MAX_SIZE];
	int state;
	int iconId;
	int userId;

};
//添加好友请求：协议头，用户昵称，用户id，被添加的用户昵称
struct STRU_TCP_ADD_FRIEND_RQ
{
	STRU_TCP_ADD_FRIEND_RQ():type(_DEF_TCP_ADD_FRIEND_RQ),userId(0)
	{
		memset(userName,0,_DEF_MAX_SIZE);
		memset(friendName,0,_DEF_MAX_SIZE);
	}
	PackType type;
	char userName[_DEF_MAX_SIZE];
	char friendName[_DEF_MAX_SIZE];
	int userId;
};
//添加好友回复：协议头，被添加用户的昵称和id,添加结果,主动发起添加用户的id
struct STRU_TCP_ADD_FRIEND_RS
{
    STRU_TCP_ADD_FRIEND_RS():type(_DEF_TCP_ADD_FRIEND_RS),friendId(0),userId(0),result(add_success)
	{
		memset(friendName,0,_DEF_MAX_SIZE);
	}
	PackType type;
	int result;
	char friendName[_DEF_MAX_SIZE];
	int friendId;
    int userId;
};
//聊天请求，协议头，聊天内容，接收者的id，发送者的id
struct STRU_TCP_CHAT_RQ
{
	STRU_TCP_CHAT_RQ():type(_DEF_TCP_CHAT_RQ),friendId(0),result(0),userId(0)
	{
		memset(content,0,_DEF_MAX_SIZE);
	}
	PackType type;
	int result;
	char content[_Def_CONTENT_SIZE];
	int friendId;
	int userId;
};
//聊天回复，发送结果，发送者和接受者的id
struct STRU_TCP_CHAT_RS
{
	STRU_TCP_CHAT_RS():type(_DEF_TCP_CHAT_RS),friendId(0),result(send_success),userId(0)
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
	STRU_TCP_OFFLINE_RQ():type(_DEF_TCP_OFFLINE_RQ),userId(0)
	{
	}
	PackType type;
	int result;
	int userId;
};
