#pragma once
#include<INet.h>
#include<TcpServerMediator.h>
class TcpServer :public INet
{
public:
	TcpServer(INetMediator* Mediator);
	~TcpServer();
	//��ʼ������
	bool InitNet();
	//�رճ�ʼ������
	void UnInitNet();
	//��������
	bool SendData(long lSendIP, char* buf, int len);
	//��������
	void RecvData();
public://���������ڵ��õľ���protected
	int sockListen;
	//������
	int m_epfd;
	//�̳߳�
	thread_pool_t* m_ptr;
	pthread_mutex_t accept_lock;
	pthread_mutex_t deal_lock;
	bool m_isStop;
};
