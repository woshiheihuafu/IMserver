#include<TcpServer.h>

TcpServer::TcpServer(INetMediator* pMediator):sockListen(0),m_isStop(false)
{
	m_pMediator = pMediator;
}
TcpServer::~TcpServer()
{
	UnInitNet();
}
//��ʼ������ ���ؿ⣬����socket����ip��ַ���������������������̣߳��������������߳�
bool TcpServer::InitNet()
{
	pthread_mutex_init(&accept_lock,NULL);
	pthread_mutex_init(&deal_lock,NULL);
	sockListen = net_init();
	m_epfd = epfd_create(sockListen);
	m_ptr = thread_pool_create(10,100,500);
	cout<<"2.open tcp net sucess"<<endl;
	epoll_listen(sockListen,m_epfd,m_ptr,(void*)this);
}

//�رճ�ʼ������
void TcpServer::UnInitNet()
{
	//�ر��߳�
	//for (auto ite = m_listHandle.begin(); ite != m_listHandle.end();)
	//{
		//if (*ite)
		//{
			//HANDLE handle = *ite;
			//if (WaitForSingleObject(handle, 100) == WAIT_TIMEOUT)
			//{
			//	TerminateThread(handle, -1);
			//}
			//CloseHandle(handle);
			//handle = NULL;
			//ite = m_listHandle.erase(ite);
		//}
	//}
	//�ر�socket
	//if (sockListen && sockListen != INVALID_SOCKET)
	//{
		//closesocket(sockListen);
	//}
    close(sockListen);
	////�ر�map�е�socket
	//for (auto ite = m_map.begin(); ite != m_map.end();)
	//{
		//if (ite->second && ite->second != INVALID_SOCKET)
		//{
			//closesocket(ite->second);
		//}
		//ite = m_map.erase(ite);
	//}
	//ж�ؿ�
	//WSACleanup();
}
//��������
bool TcpServer::SendData(long lSendIP, char* buf, int len)
{
	if (!buf && len <= 0)
	{
		return false;
	}

	//�ȷ�����С
	if (send(lSendIP, (char*)&len, sizeof(len), 0) < 0)
	{
		return false;
	}
	//�ٷ�������
	if (send(lSendIP, buf, len, 0) < 0)
	{
		return false;
	}
	return true;
}
//��������
void TcpServer::RecvData()
{
	//����100ms ���������У����ܻ�û�н�socket�Ž�map���Ϳ�ʼȡ�ˣ��ȴ���socket�Ž�map��
	//Sleep(100);
	//SOCKET sockClient = m_map[GetCurrentThreadId()];
	//if (!sockClient || sockClient == INVALID_SOCKET)
	//{
		//return;
	//}
	//while (!m_isStop)
	//{
		//���հ���С
		//int recvNum = 0;
		//int nPackSize = 0;
		//recvNum = recv(sockClient, (char*)&nPackSize, sizeof(nPackSize), 0);
		//if (recvNum > 0)
		//{
			//���հ�����
			//char* buf = new char[nPackSize];
			//int offset = 0;
			//while (nPackSize)
			//{
				//recvNum = recv(sockClient, buf + offset, nPackSize, 0);
				//if (recvNum > 0)
				//{
					//offset += recvNum;
					//nPackSize -= recvNum;
				//}
				//else
				//{
					//return;
				//}
			//}
			//m_pMediator->DealData(sockClient, buf, offset);
			//offset = 0;
		//}
	//}
}
