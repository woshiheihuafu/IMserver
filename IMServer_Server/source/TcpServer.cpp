#include<TcpServer.h>

TcpServer::TcpServer(INetMediator* pMediator):sockListen(0),m_isStop(false)
{
	m_pMediator = pMediator;
}
TcpServer::~TcpServer()
{
	UnInitNet();
}
//初始化网络 加载库，创建socket，绑定ip地址，监听，创建接受连接线程，创建接收数据线程
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

//关闭初始化网络
void TcpServer::UnInitNet()
{
	//关闭线程
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
	//关闭socket
	//if (sockListen && sockListen != INVALID_SOCKET)
	//{
		//closesocket(sockListen);
	//}
    close(sockListen);
	////关闭map中的socket
	//for (auto ite = m_map.begin(); ite != m_map.end();)
	//{
		//if (ite->second && ite->second != INVALID_SOCKET)
		//{
			//closesocket(ite->second);
		//}
		//ite = m_map.erase(ite);
	//}
	//卸载库
	//WSACleanup();
}
//发送数据
bool TcpServer::SendData(long lSendIP, char* buf, int len)
{
	if (!buf && len <= 0)
	{
		return false;
	}

	//先发包大小
	if (send(lSendIP, (char*)&len, sizeof(len), 0) < 0)
	{
		return false;
	}
	//再发包内容
	if (send(lSendIP, buf, len, 0) < 0)
	{
		return false;
	}
	return true;
}
//接收数据
void TcpServer::RecvData()
{
	//休眠100ms 创建即运行（可能还没有将socket放进map，就开始取了，等待将socket放进map）
	//Sleep(100);
	//SOCKET sockClient = m_map[GetCurrentThreadId()];
	//if (!sockClient || sockClient == INVALID_SOCKET)
	//{
		//return;
	//}
	//while (!m_isStop)
	//{
		//接收包大小
		//int recvNum = 0;
		//int nPackSize = 0;
		//recvNum = recv(sockClient, (char*)&nPackSize, sizeof(nPackSize), 0);
		//if (recvNum > 0)
		//{
			//接收包内容
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
