#include"TcpClient.h"
#include"packet.h"
#include<process.h>
#include"TcpClientMediator.h"
TcpClient::TcpClient(INetMediator* pMediator):m_isStop(false),m_sock(INVALID_SOCKET),m_hThreadHandle(NULL)
{
	m_pMediator = pMediator;
}
 TcpClient::~TcpClient()
{
	UninitNet();
}
//初始化网络,加载库，创捷套接字,连接
bool TcpClient::InitNet()
{
	//加载库
	WORD wVersionRequester = {0};
	WSADATA wsaData = {0};
	int err = -1;
	wVersionRequester = MAKEWORD(2,2);
	err = WSAStartup(wVersionRequester,&wsaData);
	if(err != 0)
	{
		printf("WSAStartup failed with error : %d\n",err);
		return false;
	}
	if(LOBYTE(wVersionRequester) != 2  ||  HIBYTE(wVersionRequester) != 2)
	{
		printf("Could not find a usable version of Winsock.dll\n");
		WSACleanup();
		return false;
	}
	else
	{
		printf("The Winsock 2.2 dll was found okay\n");
	}
	//2.创建套接字
	
	m_sock = socket(AF_INET,SOCK_STREAM ,IPPROTO_TCP);
	if(INVALID_SOCKET == m_sock)
	{
		printf("socket error: %d\n",WSAGetLastError());;
		WSACleanup();
		return false;
	}
	else
	{
		printf("socket succeded\n");
	}
	//3.连接
	sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_port = htons(_Def_TCP_PORT);
    client.sin_addr.S_un.S_addr = inet_addr(_Def_TCP_SERVER_IP);
	err = connect(m_sock,(sockaddr *)&client,sizeof(client));
	if(err == SOCKET_ERROR)
	{
		printf("connection fuction failed with %d\n",WSAGetLastError());
		closesocket(m_sock);
		WSACleanup();
	}
	else
	{
		printf("connect succed\n");
	}

	//4.创建线程--接受数据
	m_hThreadHandle = (HANDLE)_beginthreadex(NULL,0,&RecvThrad,this,0,NULL);
	
	return true;
}


unsigned int _stdcall TcpClient::RecvThrad(void* lpvoid)
{
	TcpClient *pThis = (TcpClient*)lpvoid;
	pThis->RecvData();
	return 0;
}
//关闭网络
void TcpClient::UninitNet()
{
	//1.退出线程
	m_isStop = true;
	if(WAIT_TIMEOUT == WaitForSingleObject(m_hThreadHandle,100))
	{
		TerminateProcess(m_hThreadHandle,-1);
	}
	CloseHandle(m_hThreadHandle);
	m_hThreadHandle = NULL;
	//2.关闭socket
	if(!m_sock && m_sock != INVALID_SOCKET)
	{
		closesocket(m_sock);
	}
	//3.卸载库
	WSACleanup();
}
//发送数据
bool TcpClient::SendData(long ISendIp,char* buf,int nLen )
{
	
	//1.参数校验
	if(!buf || nLen <= 0)
	{
		return false;
	}
	//2.先发包大小
	if(send(m_sock,(char *)&nLen,sizeof(int),0) <= 0)
	{
		return false;
	}
	//3.再发包内容
	if(send(m_sock,buf,nLen,0) <= 0)
	{
		return false;
	}
	return true;
}
//接受数据
void TcpClient::RecvData()//只要外部不调用尽量设成 protected
{
	int nRecvNum = 0;//粘包 字节流没有边界  先发包大小再发
	int nPckSize = 0;
	int offset = 0;
	while(!m_isStop)
	{
		//接收包大小
		nRecvNum = recv(m_sock,(char*)&nPckSize,sizeof(int),0);//返回接收到的字节数
		if(nRecvNum > 0)
		{
			//接收包内容
			char* buf = new char[nPckSize];//新new一个buf 防止一个buf时上一个buf未处理完就向buf里导入新内容
			while(nPckSize)//再设置一个循环 防止长度过长一次没有接受全
			{
				nRecvNum = recv(m_sock,buf+offset,nPckSize,0);
				if(nRecvNum > 0)
				{
					offset += nRecvNum;
					nPckSize -= nRecvNum;
				}
			}
			//接收完一个完整的包，offset清零，数据发送给中介者类
            int a = *(int*)buf;
			this->m_pMediator->DealData(m_sock,buf,offset);
			offset = 0;

		}
	}
}
