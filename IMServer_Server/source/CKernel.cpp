#include <CKernel.h>
#include <TcpServerMediator.h>
#include <string>
using namespace std;
CKernel* CKernel::m_kernel = NULL;
#define ProtocalMap(a) m_netProtocaLMap[a - DEF_PROTOCAL_BASE - DEF_PROTOCAL_COUNT]
CKernel::CKernel()
{
	setProtocalMap();
	pthread_mutex_init(&lock,NULL);
	m_kernel = this;
}

CKernel::~CKernel()
{
}
//打开服务器
bool CKernel::startServer()
{
	//1.连接数据库
	char ip[] = "127.0.0.1";
	char user[] = "root";
	char password[] = "1276518478....z";
	char dbName[] = "qqproject";
	if (!m_Mysql.ConnectMySql(ip, user, password, dbName))
	{
		cout<<"1.open mysql error"<<endl;
		return false;
	}
	else
		cout<<"1.open mysql sucess"<<endl;
	//2.打开网络
	m_pMediator = new TcpServerMediator;
	if (!m_pMediator->OpenNet())
	{
		cout<<"2.open tcp net error"<<endl;
		//return false;
	}
	else
	{
		cout<<"2.open tcp net sucess"<<endl;
	}
	//3.打开udp网络
	/*m_pUdpMediator = new UdpNetMediator;
	if (!m_pUdpMediator->OpenNet())
	{
		printf("打开udp网络失败\n");
		return false;
	}*/
	return true;
}
//关闭服务器
void CKernel::closeServer()
{
	//1.回收资源
	
	//2.断开数据库连接
	m_Mysql.DisConnect();
	//3.关闭网络
		
	if (m_pMediator)
	{
		m_pMediator->CloseNet();
		delete m_pMediator;
		m_pMediator = NULL;
	}
	/*if (m_pUdpMediator)
	{
		m_pUdpMediator->CloseNet();
		delete m_pUdpMediator;
		m_pUdpMediator = NULL;
	}*/
	m_pUdpMediator->CloseNet();
}
//处理所有数据
void CKernel::dealData(long lSendIP, char* buf, int len)
{
	std::cout << "dealData : " << endl;
	//1.取协议头
	PackType type = *(PackType*)buf;
	printf("type = %d\n",type);
	if(type <= 1000 || type >= 1100)
	{
		delete[] buf;
		buf = nullptr;
		return;
	}
	//2.取函数
	Fun func = ProtocalMap(type);
	//3.调用函数
	//pthread_mutex_lock(&lock);
	if (func)
	{
		(this->*func)(lSendIP, buf, len);
	}
	//pthread_mutex_unlock(&lock);
	//4.删除buf
	delete[] buf;
	buf = nullptr;
	//测试
}
//初始化协议映射表
void CKernel::setProtocalMap()
{
	//注册请求
	ProtocalMap(DEF_TCP_REGISTER_RQ) = &CKernel::dealRegisterRq;
	//登录请求
	ProtocalMap(DEF_TCP_LOGIN_RQ) = &CKernel::dealLoginRq;

	//发送信息请求
	ProtocalMap(DEF_TCP_CHAT_RQ) = &CKernel::dealChatRq;
	//处理添加好友请求
	ProtocalMap(DEF_TCP_ADD_FRIEND_RQ) = &CKernel::dealAddFriendsRq;
	//处理添加好友回复
	ProtocalMap(DEF_TCP_ADD_FRIEND_RS) = &CKernel::dealAddFriendsRs;
	//处理下线请求
	ProtocalMap(DEF_TCP_OFFLINE_RQ) = &CKernel::dealOfflineRq;
	//处理创建群组请求
	ProtocalMap(DEF_TCP_CREATEGROUP_RQ) = &CKernel::dealCreateGroupRq;
	//处理群组聊天请求
	ProtocalMap(DEF_TCP_GROUP_CHAT_RQ) = &CKernel::dealGroupChatRq;
	//处理加入群组请求
	ProtocalMap(DEF_TCP_JOIN_GROUP_RQ) = &CKernel::dealJoinGroupRq;
	//处理加入群组回复
	ProtocalMap(DEF_TCP_JOIN_GROUP_RS) = &CKernel::dealJoinGroupRs;
	//处理发送文件信息请求
	ProtocalMap(DEF_TCP_FILE_INFO_RQ) = &CKernel::dealSendFileInfoRq;
	//处理接收文件请求
	ProtocalMap(DEF_TCP_FILE_BLOCK_RQ) = &CKernel::dealSendFileRq;
	//处理文件回复
	ProtocalMap(DEF_TCP_FILE_INFO_RS) = &CKernel::dealSendFileInfoRs;
	//处理删除好友请求
	ProtocalMap(DEF_TCP_DELETE_FRIEND_RQ) = &CKernel::dealDeleteFriendRq;
	//处理音视频通话请求
	ProtocalMap(DEF_UDP_VIDEO_RQ) = &CKernel::dealVideoRq;
	//处理视频通话信息请求
	ProtocalMap(DEF_TCP_VIDEO_INFO_RQ) = &CKernel::dealVideoInfoRq;
	//处理视频通话信息回复
	ProtocalMap(DEF_TCP_VIDEO_INFO_RS) = &CKernel::dealVideoInfoRs;
}
//处理注册请求
void CKernel::dealRegisterRq(long lSendIP, char* buf, int len)
{
	//注册回复包
	STRU_TCP_REGISTER_RS rs;
	//1.拆包
	STRU_TCP_REGISTER_RQ* rq = (STRU_TCP_REGISTER_RQ*)buf;
	//2.TODO:校验昵称，电话
	list<string> resultList;
        //3.查询是否已经有这个人
	char sqlBuf[1024] = "";
	sprintf(sqlBuf, "select userId from userInfo where userPhone = '%s';", rq->tel);
	if (!m_Mysql.SelectMySql(sqlBuf, 1, resultList))
	{
		cout << "select mysql error" << endl;
		rs.result = parameter_error;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	//4.如果有，直接发送失败回复
	if (1 == resultList.size())
	{
		rs.result = user_is_exit;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	//5.如果没有，创建用户
	else
	{
		sprintf(sqlBuf, "insert into userInfo (userPhone,userName,userPasw,userIcon,userFeel) values('%s','%s','%s',1,'this is '); ", rq->tel,rq->name,rq->password);
		if (!m_Mysql.UpdateMySql(sqlBuf))
		{
			cout << sqlBuf << endl;
			cout << "update mysql error" << endl;
			rs.result = parameter_error;
			m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
			return;
		}
		else
		{
			rs.result = register_success;
		}
		//6.发送成功回复
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
	}
	

}
//处理登录请求
void CKernel::dealLoginRq(long lSendIP, char* buf, int len)
{
	//TODO:
	//注册回复包；
	STRU_TCP_LOGIN_RS rs;
	//1.拆包
	STRU_TCP_LOGIN_RQ* rq = (STRU_TCP_LOGIN_RQ*)buf;
	//2.校验信息(查询是否有这个人)
	char sqlBuf[1024] = "";
	sprintf(sqlBuf, "select userPhone from userInfo where userPhone = '%s';", rq->tel);
	list<string> resultList;
	if (!m_Mysql.SelectMySql(sqlBuf, 1, resultList))
	{
		//数据库查询错误
		printf("数据库查询错误\n");
		rs.result = parameter_login_error;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	if (resultList.size() == 0)
	{ 
		//没有这个人
		rs.result = user_not_exist;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	//查询密码
	sprintf(sqlBuf, "select userPasw from userInfo where userPhone = '%s';", rq->tel);
	resultList.pop_front();
	if (!m_Mysql.SelectMySql(sqlBuf, 1, resultList))
	{
		//数据库查询错误
		printf("数据库查询错误\n");
		rs.result = parameter_login_error;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	if (strcmp(resultList.front().c_str(), rq->password) != 0)
	{
		//密码错误
		rs.result = password_error;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	//3.保存id和socket的关系
	//根据电话号码获取id
	resultList.pop_front();
	sprintf(sqlBuf, "select userId from userInfo where userPhone = '%s';", rq->tel);
	if (!m_Mysql.SelectMySql(sqlBuf, 1, resultList))
	{
		//数据库查询错误
		printf("数据库查询错误\n");
		rs.result = parameter_login_error;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	int id = stoi(resultList.front());
	m_mapSocketId[id] = lSendIP;




	//发送登录回复
	rs.userId = id;
	rs.result = login_success;
	m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
	//4.查询用户好友信息
	getFriends(id);
	//5.查询用户群组信息
	getGroupInfo(id);
	//6.查询聊天记录
	getChatHistory(id);
	return;
}
//处理聊天请求
void CKernel::dealChatRq(long lSendIP, char* buf, int len)
{
	//拆包
	STRU_TCP_CHAT_RQ* rq = (STRU_TCP_CHAT_RQ*)buf;
	STRU_TCP_CHAT_RS rs;
	char sqlBuf[1024] = "";
	//将聊天记录存储到mysql中
	sprintf(sqlBuf, "insert into userChatHistory (userId1,userId2,infotime,info) values(%d,%d,'%s','%s');", rq->userId, rq->friendId,rq->time, rq->content);
	if (!m_Mysql.UpdateMySql(sqlBuf))
	{
		printf("数据库更新失败\n");
		return;
	}
	//如果对方不在线，直接回复不在线聊天回复
	if (m_mapSocketId.count(rq->friendId) == 0)
	{
		rs.userId = rq->userId;
		rs.friendId = rq->friendId;
		rs.result = user_offline;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	//在线则将聊天请求转发给好友
	m_pMediator->SendData(m_mapSocketId[rq->friendId],buf,len);
}
//处理添加好友请求
void CKernel::dealAddFriendsRq(long lSendIP, char* buf, int len)
{
	//拆包
	STRU_TCP_ADD_FRIEND_RQ* rq = (STRU_TCP_ADD_FRIEND_RQ*)buf;
	STRU_TCP_ADD_FRIEND_RS rs;

	char sqlBuf[1024] = "";
	list<string> resultList;
	sprintf(sqlBuf, "select userId from userInfo where userPhone = '%s'", rq->friendPhone);
	if (!m_Mysql.SelectMySql(sqlBuf, 1, resultList))
	{
		//
		printf("查询数据库失败\n");
		rs.result = add_fail;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	//看有没有这个人
	if (resultList.size() == 0)
	{
		rs.result = no_this_user;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	//看看是否已经有这个好友
	resultList.clear();
	sprintf(sqlBuf, "select userId from userInfo where userPhone = '%s';", rq->friendPhone);
	if (!m_Mysql.SelectMySql(sqlBuf, 1, resultList))
	{
		rs.result = add_fail;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	int friendId = stoi(resultList.front());
	resultList.clear();
	sprintf(sqlBuf, "select userId1 from userRelation where userId2 = %d and userId1 = %d;", friendId,rq->userId);
	if (!m_Mysql.SelectMySql(sqlBuf, 1, resultList))
	{
		rs.result = add_fail;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	//已经有这个好友了
	if (resultList.size() > 0)
	{
		rs.result = already_add;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	//看这个人在不在线
	if (m_mapSocketId.count(friendId) == 0)
	{
		rs.result = user_offline;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	m_pMediator->SendData(m_mapSocketId[friendId], buf, len);

}
//添加好友回复
void CKernel::dealAddFriendsRs(long lSendIP, char* buf, int len)
{
	//拆包
	STRU_TCP_ADD_FRIEND_RS* rs = (STRU_TCP_ADD_FRIEND_RS*)buf;

	if (m_mapSocketId.count(rs->userId) > 0)
	{
		char sqlBuf[1024] = "";
		sprintf(sqlBuf, "insert into userRelation (userId1,userId2) values(%d,%d);", rs->userId, rs->friendId);
		if (!m_Mysql.UpdateMySql(sqlBuf))
		{
			printf("数据库更新失败\n");
			return;
		}
		sprintf(sqlBuf, "insert into userRelation (userId1,userId2) values(%d,%d);", rs->friendId, rs->userId);
		if (!m_Mysql.UpdateMySql(sqlBuf))
		{
			printf("数据库更新失败\n");
			return;
		}
		m_pMediator->SendData(m_mapSocketId[rs->userId], buf, len);
	}
}
//处理下线请求
void CKernel::dealOfflineRq(long lSendIP, char* buf, int len)
{
	//拆包
	STRU_TCP_OFFLINE_RQ* rq = (STRU_TCP_OFFLINE_RQ*)buf;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf, "select userId2 from userRelation where userId1 = %d;", rq->userId);
	list<string> resultList;
	if (!m_Mysql.SelectMySql(sqlBuf, 1, resultList))
	{
		printf("数据库查询错误\n");
		return;
	}
	if (resultList.size() > 0)
	{
		for (string n : resultList)
		{
			int id = stoi(n);
			if (m_mapSocketId.count(id) > 0)
			{
				m_pMediator->SendData(m_mapSocketId[id], buf, len);
			}
		}
	}
	auto it = m_mapSocketId.find(rq->userId);
	if (it != m_mapSocketId.end()) {
		m_mapSocketId.erase(it);
	}
}
//处理创建群组请求
void CKernel::dealCreateGroupRq(long lSendIP, char* buf, int len)
{
	STRU_TCP_CREATEGROUP_RQ* rq = (STRU_TCP_CREATEGROUP_RQ*)buf;
	STRU_TCP_CREATEGROUP_RS rs;
	char sqlBuf[1024] = "";
	list<string> resultList;
	sprintf(sqlBuf, "select groupName from userGroup where groupName = '%s';", rq->groupName);
	if (!m_Mysql.SelectMySql(sqlBuf, 1, resultList))
	{
		rs.result = create_error;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	//查询是否有群组叫此名
	if (resultList.size() > 0)
	{
		rs.result = group_is_exit;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	sprintf(sqlBuf, "insert into userGroup (groupName,creatorId,groupIcon) values('%s',%d,5);",rq->groupName,rq->userId);
	if (!m_Mysql.UpdateMySql(sqlBuf))
	{
		rs.result = create_error;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	resultList.clear();
	sprintf(sqlBuf, "select groupId from userGroup where groupName = '%s';", rq->groupName);
	if (!m_Mysql.SelectMySql(sqlBuf, 1, resultList))
	{
		rs.result = create_error;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	int groupId = 0;
	if (resultList.size() > 0)
	{
		groupId = stoi(resultList.front());
	}
	sprintf(sqlBuf, "insert into memberGroup (groupId,memberId) values(%d,%d);", groupId, rq->userId);
	if (!m_Mysql.UpdateMySql(sqlBuf))
	{
		rs.result = create_error;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	rs.creatorId = rq->userId;
	rs.groupIcon = 5;
	strcpy(rs.groupName, rq->groupName);
	rs.groupId = groupId;
	rs.result = create_success;
	m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
}
//处理群组聊天请求
void CKernel::dealGroupChatRq(long lSendIP, char* buf, int len)
{
	STRU_TCP_GROUP_CHAT_RQ* rq = (STRU_TCP_GROUP_CHAT_RQ*)buf;
	char sqlBuf[1024] = "";
	list<string> resultList;
	sprintf(sqlBuf, "select memberId from memberGroup where groupId = %d and memberId != %d;", rq->groupId,rq->userId);
	if (!m_Mysql.SelectMySql(sqlBuf, 1, resultList))
	{
		printf("数据库查询错误\n");
		return;
	}
	for (auto ite : resultList)
	{
		if (m_mapSocketId.count(stoi(ite)) > 0)
		{
			m_pMediator->SendData(m_mapSocketId[stoi(ite)],buf,len);
		}
	}
}
//处理加入群组请求
void CKernel::dealJoinGroupRq(long lSendIP, char* buf, int len)
{
	STRU_TCP_JOIN_GROUP_RQ* rq = (STRU_TCP_JOIN_GROUP_RQ*)buf;
	//用于向群主发送请求
	STRU_TCP_JOIN_GROUP_RQ rqs;
	STRU_TCP_JOIN_GROUP_RS rs;
	char sqlBuf[1024] = "";
	list<string> resultList;
	//获取群主id，群id
	sprintf(sqlBuf, "select creatorId,groupId from userGroup where groupName = '%s';", rq->groupName);
	//看看有没有这个群
	if (!m_Mysql.SelectMySql(sqlBuf, 2, resultList))
	{
		printf("数据库查询错误\n");
		rs.result = join_group_error;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	if (resultList.size() == 0)
	{
		rs.result = no_this_group;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	int creatorId = stoi(resultList.front());
	resultList.pop_front();
	int groupId = stoi(resultList.front());
	resultList.clear();
	//看看群主在不在线
	if (m_mapSocketId.count(creatorId) == 0)
	{
		rs.result = creator_offline;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	//看看是否已经加入
	sprintf(sqlBuf, "select groupId from memberGroup where groupId = %d and memberId = %d;", groupId,rq->userId);
	if (!m_Mysql.SelectMySql(sqlBuf, 1, resultList))
	{
		printf("数据库查询错误\n");
		rs.result = join_group_error;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	if (resultList.size() > 0)
	{
		rs.result = alreday_join;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	rqs.groupId = groupId;
	strcpy(rqs.groupName, rq->groupName);
	rqs.userId = rq->userId;
	strcpy(rqs.userName, rq->userName);
	m_pMediator->SendData(m_mapSocketId[creatorId], (char*)&rqs, sizeof(rqs));
}
//处理加入群组回复
void CKernel::dealJoinGroupRs(long lSendIP, char* buf, int len)
{
	STRU_TCP_JOIN_GROUP_RS* rs = (STRU_TCP_JOIN_GROUP_RS*)buf;
	char sqlBuf[1024] = "";
	list<string> resultList;
	sprintf(sqlBuf, "select creatorId from userGroup where groupName = '%s';", rs->groupName);
	if (!m_Mysql.SelectMySql(sqlBuf, 1, resultList))
	{
		printf("数据库查询错误\n");
		return;
	}
	int groupId = stoi(resultList.front());
	if (rs->result == join_group_sucess)
	{
		sprintf(sqlBuf, "insert into memberGroup (groupId,memberId) values(%d,%d);", groupId, rs->userId);
		if (!m_Mysql.UpdateMySql(sqlBuf))
		{
			printf("数据库查询错误\n");
			return;
		}
		m_pMediator->SendData(m_mapSocketId[rs->userId], buf, len);
	}
	else
	{
		rs->result = join_group_refuse;
		m_pMediator->SendData(m_mapSocketId[rs->userId], (char*)rs, sizeof(rs));
	}
	
}
//处理发送文件信息请求
void CKernel::dealSendFileInfoRq(long lSendIP, char* buf, int len)
{
	//拆包
	STRU_FILE_INFO_RQ* rq = (STRU_FILE_INFO_RQ*)buf;
	if (m_mapSocketId.count(rq->friendId) == 0)
	{
		STRU_FILE_INFO_RS rs;
		rs.result = not_login;
		m_pMediator->SendData(m_mapSocketId[rq->userId], (char*)&rs, sizeof(rs));
		return;
	}
	m_pMediator->SendData(m_mapSocketId[rq->friendId], buf, len);
}
//处理发送文件请求
void CKernel::dealSendFileRq(long lSendIP, char* buf, int len)
{
	STRU_FILE_BLOCK_RQ* rq = (STRU_FILE_BLOCK_RQ*)buf;
	m_pMediator->SendData(m_mapSocketId[rq->friendId], buf, len);
}
//处理接收文件回复
void CKernel::dealSendFileInfoRs(long lSendIP, char* buf, int len)
{
	STRU_FILE_INFO_RS* rs = (STRU_FILE_INFO_RS*)buf;
	m_pMediator->SendData(m_mapSocketId[rs->userId], buf, len);
}
//处理删除好友请求
void CKernel::dealDeleteFriendRq(long lSendIP, char* buf, int len)
{
	STRU_TCP_DELETE_FRIEND_RQ* rq = (STRU_TCP_DELETE_FRIEND_RQ*)buf;
	STRU_TCP_DELETE_FRIEND_RQ rs;
	int userId = rq->userId;
	int friendId = rq->friendId;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf, "delete from userRelation where (userId1 = %d and userId2 = %d) or (userId1 = %d and userId2 = %d);", userId, friendId, friendId, userId);
	if (!m_Mysql.UpdateMySql(sqlBuf))
	{
		printf("数据库更新失败\n");
		rs.result = delete_fail;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return;
	}
	rs.result = delete_success;
	m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
}
//处理音视频通话请求
void CKernel::dealVideoRq(long lSendIP, char* buf, int len)
{
	//printf("\n");
	STRU_UDP_VIDEO_RQ* rq = (STRU_UDP_VIDEO_RQ*)buf;
	cout << rq->userid1 << "send to --------------------------------" << rq->userid2 << endl;
	if(m_mapSocketId.count(rq->userid2) > 0)
		m_pMediator->SendData(m_mapSocketId[rq->userid2], buf, len);
}
//处理是否接受视频通话
void CKernel::dealVideoInfoRq(long lSendIP, char* buf, int len)
{
	printf("-------------------------RQ\n");
	STRU_TCP_VIDEO_INFO_RQ* rq = (STRU_TCP_VIDEO_INFO_RQ*)buf;
	if (m_mapSocketId.count(rq->userid2) == 0)
	{
		cout << "not in" << endl;
		STRU_TCP_VIDEO_INFO_RS rs;
		rs.userid1 = rq->userid1;
		rs.userid2 = rq->userid2;
		rs.result = 0;
		m_pMediator->SendData(lSendIP, (char*)&rs, sizeof(rs));
		return ;
	}
	m_pMediator->SendData(m_mapSocketId[rq->userid2], buf, len);
}
void CKernel::dealVideoInfoRs(long lSendIP, char* buf, int len)
{
	printf("-------------------------RS\n");
	STRU_TCP_VIDEO_INFO_RS *rs = (STRU_TCP_VIDEO_INFO_RS*)buf;
	m_pMediator->SendData(m_mapSocketId[rs->userid1], buf, len);
}
//获取好友信息
void CKernel::getFriends(int id)
{
	//查询自己信息
	STRU_TCP_FRIEND_INFO infoMe;
	getInfoById(id,&infoMe);
	//发送自己的信息给客户端
	m_pMediator->SendData(m_mapSocketId[id], (char*)&infoMe, sizeof(infoMe));
	//查询好友信息
	list<string> resultList;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf, "select userId2 from userRelation where userId1 = '%d';", id);
	if (!m_Mysql.SelectMySql(sqlBuf, 1, resultList))
	{
		//数据库查询错误
		printf("数据库查询错误:好友信息\n");
		return;
	}
	//循环发送好友信息给客户端
	while (resultList.size() > 0)
	{
		STRU_TCP_FRIEND_INFO infoFriend;
		int FriendId = stoi(resultList.front());
		resultList.pop_front();
		getInfoById(FriendId, &infoFriend);
		m_pMediator->SendData(m_mapSocketId[id], (char*)&infoFriend, sizeof(infoFriend));
		//如果好友在线，通知好友你上线了
		if (m_mapSocketId.count(FriendId) > 0)
		{
			m_pMediator->SendData(m_mapSocketId[FriendId], (char*)&infoMe, sizeof(infoMe));
		}
		//不在线继续
		else
		{
			continue;
		}
	}
	//发送好友信息给客户端
}

//获取群组信息
void CKernel::getGroupInfo(int id)
{
	list<string> resultList;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf, "select userGroup.groupid, userGroup.groupname, userGroup.creatorid, userGroup.groupicon\
		from userGroup\
		inner join memberGroup on userGroup.groupid = memberGroup.groupid\
	where memberGroup.memberid = %d; ", id);
	if (!m_Mysql.SelectMySql(sqlBuf, 4, resultList))
	{
		//数据库查询错误
		printf("数据库查询错误：群组信息\n");
		return;
	}
	while (resultList.size() != 0)
	{
		STRU_TCP_GROUP_INFO info;
		//获取头像，昵称，个签
		info.groupId = stoi(resultList.front());
		resultList.pop_front();
		strcpy(info.groupName, resultList.front().c_str());
		resultList.pop_front();
		info.creatorId = stoi(resultList.front());
		resultList.pop_front();
		info.groupIcon = stoi(resultList.front());
		resultList.pop_front();
		m_pMediator->SendData(m_mapSocketId[id], (char*)&info, sizeof(info));
	}
	
}
//根据id查询用户信息好友信息,协议头，昵称，用户状态，个性签名，头像，用户id
void CKernel::getInfoById(int id, STRU_TCP_FRIEND_INFO* info)
{
	info->userId = id;
	list<string> resultList;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf, "select userName,userFeel,userIcon from userInfo where userId = '%d';",id);
	if (!m_Mysql.SelectMySql(sqlBuf, 3, resultList))
	{
		//数据库查询错误
		printf("数据库查询错误\n");
		return;
	}
	if (resultList.size() == 3)
	{
		//获取头像，昵称，个签
		strcpy(info->name, resultList.front().c_str());
		resultList.pop_front();
		strcpy(info->feling, resultList.front().c_str());
		resultList.pop_front();
		info->iconId = stoi(resultList.front());
		resultList.pop_front();
	}
	//判断是否在线
	if (m_mapSocketId.count(id) > 0)
	{
		info->state = 1;
	}
	else
	{
		info->state = 0;
	}
	
}
//获取聊天记录
void CKernel::getChatHistory(int id)
{
	//TODO:
	STRU_TCP_CHAT_HISTORY_RQ rq;
	list<string> resultList;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf, "select* from userChatHistory where userId1 = %d or userId2 = %d order by infotime asc;", id,id);
	if (!m_Mysql.SelectMySql(sqlBuf, 4, resultList))
	{
		//数据库查询错误
		printf("数据库查询错误:聊天信息\n");
		return;
	}
	while (resultList.size() != 0)
	{
		rq.userid1 = stoi(resultList.front());
		resultList.pop_front();
		rq.userid2 = stoi(resultList.front());
		resultList.pop_front();
		char infotime[1024];
		strcpy(rq.infotime, resultList.front().c_str());
		resultList.pop_front();
		char info[1024];
		strcpy(rq.info, resultList.front().c_str());
		resultList.pop_front();
		//printf("userid1 = %d\nuserid2 = %d\ninfotime = %s\ninfo = %s\n", rq.userid1, rq.userid2, rq.infotime, rq.info);
		m_pMediator->SendData(m_mapSocketId[id], (char*)&rq, sizeof(rq));
	}

}
