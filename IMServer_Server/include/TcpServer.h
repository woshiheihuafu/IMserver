#pragma once
#include<INet.h>
#include<TcpServerMediator.h>
class TcpServer :public INet
{
public:
	TcpServer(INetMediator* Mediator);
	~TcpServer();
	//初始化网络
	bool InitNet();
	//关闭初始化网络
	void UnInitNet();
	//发送数据
	bool SendData(long lSendIP, char* buf, int len);
	//接收数据
	void RecvData();
public://仅仅在类内调用的尽量protected
	int sockListen;
	//监听树
	int m_epfd;
	//线程池
	thread_pool_t* m_ptr;
	pthread_mutex_t accept_lock;
	pthread_mutex_t deal_lock;
	bool m_isStop;
};
