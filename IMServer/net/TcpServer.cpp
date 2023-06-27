#include"TcpServer.h"
#include"packet.h"
#include<process.h>
#include"TcpServerMediator.h"
TcpServer::TcpServer(INetMediator* pMediator):m_sockListen(INVALID_SOCKET),m_isStop(false)
{
	m_pMediator = pMediator;
}
 TcpServer::~TcpServer()
{
	UninitNet();
}
//初始化网络,加载库，创捷套接字，绑定ip地址,监听，创建连接线程，接受连接线程
bool TcpServer::InitNet()
{
	//1.加载库
	WORD wVersionRequester =  {0};
	WSADATA wsaData = {0};
	int err = -1;
	wVersionRequester = MAKEWORD(2,2);
	err = WSAStartup(wVersionRequester,&wsaData);
	if(err != 0)
	{
		printf("WSAStartup failed with error : %d\n",err);
		return false;
	}
	if(LOBYTE(wVersionRequester) != 2 || HIBYTE(wVersionRequester) != 2)
	{
		printf("Could not find a usable version of Winsock.dll\n");
		WSACleanup();
		return false;
	}
	else
	{
		printf("The Winsock 2.2 dll was found okay\n");
	}



	//2.创建socket
	m_sockListen = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//服务器需要n+1个socket  socklisten用来监听n个客户端
	if(INVALID_SOCKET == m_sockListen)
	{
		printf("socket error: %d\n",WSAGetLastError());;
		WSACleanup();
		return false;
	}
	else
	{
		printf("socket succeded\n");
	}



	//3.绑定ip地址
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(54321);
	server.sin_addr.S_un.S_addr = INADDR_ANY;
	err = bind(m_sockListen,(sockaddr *)&server,sizeof(server));
	if(SOCKET_ERROR == err)
	{
		printf("socket error: %d\n",WSAGetLastError());
		closesocket(m_sockListen);
		WSACleanup();
		return false;
	}
	else
	{
		printf("bind succed\n");
	}  



	//4.监听  ——   listen
	err = listen(m_sockListen,10);
	if(err == SOCKET_ERROR)
	{
		printf("listen errpr : %d\n",WSAGetLastError());
		closesocket(m_sockListen);
		WSACleanup();
		return false;
	}
	else
	{
		printf("listen succeded\n");
	}
	
	//5.接受连接--创建线程
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,&AcceptThread,this,0,0);
	if(handle)
	{
		m_hThreadHandleList.push_back(handle);
	}
	return true;
}
//接受连接线程
unsigned int _stdcall TcpServer::AcceptThread(void* lpvoid)
{
	TcpServer *pThis = (TcpServer*)lpvoid;
	sockaddr_in clientAddr;
	int clientAddrsize = sizeof(clientAddr);
	SOCKET sockClient = INVALID_SOCKET;
	while(!pThis->m_isStop)
	{
		//1.接受连接
		sockClient =  accept(pThis->m_sockListen,(sockaddr*)&clientAddr,&clientAddrsize);
		cout<<"ip:"<< inet_ntoa(clientAddr.sin_addr) <<"connect"<<endl;
		//2.给每个客户创造一个接受线程
		unsigned int nThreadId = 0;
		HANDLE handle = (HANDLE)_beginthreadex(NULL,0,&RecvThread,pThis,0,&nThreadId);
		if(handle)
		{
			//保存线程句柄
			pThis->m_hThreadHandleList.push_back(handle);
			//保存每个客户端的socket
			pThis->m_mapThreadIdToSocket[nThreadId] = sockClient;

		}
	}
	
	
	return 0;
}

unsigned int _stdcall TcpServer::RecvThread(void* lpvoid)
{
	TcpServer *pThis = (TcpServer*)lpvoid;
	pThis->RecvData();
	return 0;
}
//关闭网络
void TcpServer::UninitNet()
{
	//1.退出线程
	m_isStop = true;
	for(auto ite = m_hThreadHandleList.begin();ite != m_hThreadHandleList.end();)
	{
		if(*ite)
		{
			if(WAIT_TIMEOUT == WaitForSingleObject(*ite,100))
			{
				TerminateThread(*ite,-1);
			}
			CloseHandle(*ite);
			*ite = NULL;
		}
		ite = m_hThreadHandleList.erase(ite);
	}
	//2.关闭socket 
	if(m_sockListen && m_sockListen != INVALID_SOCKET)
	{
		closesocket(m_sockListen);
	}
	for(auto ite = m_mapThreadIdToSocket.begin();ite != m_mapThreadIdToSocket.end();)
	{
		if(ite->second && ite->second != INVALID_SOCKET)
		{
			closesocket(ite->second);
		}
		ite = m_mapThreadIdToSocket.erase(ite);
	}

}
//发送数据
bool TcpServer::SendData(long ISendIp,char* buf,int nLen )
{
	
	//1.参数校验
	if(!buf || nLen <= 0)
	{
		return false;
	}
	//2.先发包大小
	if(send(ISendIp,(char *)&nLen,sizeof(int),0) <= 0)
	{
		return false;
	}
	//3.再发包内容
	if(send(ISendIp,buf,nLen,0) <= 0)
	{
		return false;
	}
	return true;
}
//接受数据
void TcpServer::RecvData()//只要外部不调用尽量设成 protected
{
	//1.休眠100ms  因为可能会出错，创建线程以后立刻就执行线程，但是把线程id和套接字socket存入map需要一点时间
	Sleep(100); 
	//2.从map中取出当前线程对应的socket
	SOCKET sock = m_mapThreadIdToSocket[GetCurrentThreadId()];	
	if(!sock || sock == INVALID_SOCKET)
	{
		return;
	}
	int nRecvNum = 0;//粘包 字节流没有边界  先发包大小再发
	int nPckSize = 0;
	int offset = 0;
	while(!m_isStop)
	{
		//接收包大小
		nRecvNum = recv(sock,(char*)&nPckSize,sizeof(int),0);//返回接收到的字节数
		if(nRecvNum > 0)
		{
			//接收包内容
			char* buf = new char[nPckSize];//新new一个buf 防止一个buf时上一个buf未处理完就向buf里导入新内容
			while(nPckSize)//再设置一个循环 防止长度过长一次没有接受全
			{
				nRecvNum = recv(sock,buf+offset,nPckSize,0);
				if(nRecvNum > 0)
				{
					offset += nRecvNum;
					nPckSize -= nRecvNum;
				}
			}
			//接收完一个完整的包，offset清零，数据发送给中介者类
			
			this->m_pMediator->DealData(sock,buf,offset);
			offset = 0;
		
		
		}
	}
}