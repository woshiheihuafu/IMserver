#pragma once
#include"INetMediator.h"
#include"CMySql.h"
#include<iostream>
#include"config.h"
#include<map>
#include<string>
//¶¨Òåº¯ÊýÖ¸Õë
class CKernel;
typedef void(CKernel::* Fun)(long lSendIP, char* buf, int len);
class CKernel
{
public:
	CKernel();
	~CKernel();
	bool startServer();
	void closeServer();
	void dealData(long lSendIP, char* buf, int len);
public:
	static CKernel* m_kernel;
private:
	//³õÊ¼»¯Ð­ÒéÓ³Éä±í
	void setProtocalMap();
	//´¦Àí×¢²áÇëÇó
	void dealRegisterRq(long lSendIP, char* buf, int len);
	//´¦ÀíµÇÂ¼ÇëÇó
	void dealLoginRq(long lSendIP, char* buf, int len);
	//´¦ÀíÁÄÌìÇëÇó
	void dealChatRq(long lSendIP, char* buf, int len);
	//´¦ÀíÌí¼ÓºÃÓÑÇëÇó
	void dealAddFriendsRq(long lSendIP, char* buf, int len);
	//´¦ÀíÌí¼ÓºÃÓÑ»Ø¸´
	void dealAddFriendsRs(long lSendIP, char* buf, int len);
	//´¦ÀíÏÂÏßÇëÇó
	void dealOfflineRq(long lSendIP, char* buf, int len);
	//´¦Àí´´½¨Èº×éÇëÇó
	void dealCreateGroupRq(long lSendIP, char* buf, int len);
	//´¦ÀíÈº×éÁÄÌìÇëÇó
	void dealGroupChatRq(long lSendIP, char* buf, int len);
	//´¦Àí¼ÓÈëÈº×éÇëÇó
	void dealJoinGroupRq(long lSendIP, char* buf, int len);
	//´¦Àí¼ÓÈëÈº×é»Ø¸´
	void dealJoinGroupRs(long lSendIP, char* buf, int len);
	//´¦Àí·¢ËÍÎÄ¼þÐÅÏ¢ÇëÇó
	void dealSendFileInfoRq(long lSendIP, char* buf, int len);
	//´¦Àí½ÓÊÜÎÄ¼þÇëÇó
	void dealSendFileRq(long lSendIP, char* buf, int len);
	//´¦Àí½ÓÊÕÎÄ¼þ»Ø¸´
	void dealSendFileInfoRs(long lSendIP, char* buf, int len);
	//´¦ÀíÉ¾³ýºÃÓÑÇëÇó
	void dealDeleteFriendRq(long lSendIP, char* buf, int len);
	//´¦ÀíÒôÊÓÆµÍ¨»°ÇëÇó
	void dealVideoRq(long lSendIP, char* buf, int len);
	//´¦ÀíÊÇ·ñ½ÓÊÜÊÓÆµÍ¨»°
	void dealVideoInfoRq(long lSendIP, char* buf, int len);
	void dealVideoInfoRs(long lSendIP, char* buf, int len);
private:

	//²éÑ¯ºÃÓÑÐÅÏ¢
	void getFriends(int id);
	//²éÑ¯Èº×éÐÅÏ¢
	void getGroupInfo(int id);
	//¸ù¾Ýid²éÑ¯ÓÃ»§ÐÅÏ¢
	void getInfoById(int id, STRU_TCP_FRIEND_INFO* info);
	//¸ù¾Ýid²éÑ¯ÁÄÌì¼ÇÂ¼
	void getChatHistory(int id);
private:
	//Ð­ÒéÓ³Éä±í
	Fun m_netProtocaLMap[DEF_PROTOCAL_MAX];
	INetMediator* m_pMediator;
	INetMediator* m_pUdpMediator;
	CMySql m_Mysql;
	pthread_mutex_t lock;
public:
	//±£´æsocketºÍidµÄ¹ØÏµ
	std::map<int, long> m_mapSocketId;
};

