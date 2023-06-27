#include"UdpNet.h"
#include"packet.h"
#include"UdpNetMediator.h"
#include<process.h>
UdpNet::UdpNet(INetMediator* pMediator):m_sock(INVALID_SOCKET),m_hThreadHandle(NULL),m_isStop(false)
{
	m_pMediator = pMediator;
}
 UdpNet::~UdpNet()
{
	UninitNet();
}
//初始化网络,加载库，创捷套接字，绑定ip地址
bool UdpNet::InitNet()
{
	//1.选项目------加载库
	WORD wVersionRequester = {0};
	WSADATA wsaData = {0};
	int err = -1;

	wVersionRequester = MAKEWORD(2,2);
	err = WSAStartup(wVersionRequester,&wsaData);//WSA:windows socket api
	if(err != 0)
	{
		printf("WSAStartup failed with error : %d\n",err);
		return false;
	}
	if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("Could not find a usable version of Winsock.dll\n");
		WSACleanup();
		return false;
	}
	else
	{
		printf("The Winsock 2.2 dll was found okay\n");
	}

	//2.雇人--------创建套接字
	m_sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);//使用UDP协议
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
	//申请有限广播权限
	BOOL bval = TRUE;
	setsockopt(m_sock,SOL_SOCKET, SO_BROADCAST,(char *)&bval,sizeof(bval));
	//3.选地址-----绑定ip地址
	sockaddr_in server;
	server.sin_family = AF_INET;//         小端存储        打断存储
	server.sin_port = htons(_Def_UDP_PORT);//将整型变量从主机字节序转换成网络字节序
	server.sin_addr.S_un.S_addr = INADDR_ANY;   //绑定任意网卡 
	//server.sin_addr.S_un.S_addr = inet_addr("10.56.6.43");//字符串IP地址转换为ulong类型IP地址
	err = bind(m_sock,(sockaddr*)&server,sizeof(server));
	if(SOCKET_ERROR == err)
	{
		printf("socket error: %d\n",WSAGetLastError());
		closesocket(m_sock);
		WSACleanup();
		return false;
	}
	else
	{
		printf("bind succed\n");
	}
	
	//4.  TODO 接收数据————创建线程

	m_hThreadHandle = (HANDLE)_beginthreadex(NULL,0,&RecvThrad ,this ,0,NULL);
	return true;
}


unsigned int _stdcall UdpNet::RecvThrad(void* lpvoid)
{
	UdpNet *pThis = (UdpNet*)lpvoid;
	pThis->RecvData();
	return 0;
}
//关闭网络
void UdpNet::UninitNet()
{
	//退出线程

	//退出标志位设为true，尝试正常退出
	m_isStop = true;
	//等100ms，没有退出就强行杀死线程
	if(m_hThreadHandle)
	{
		if(WAIT_TIMEOUT == WaitForSingleObject(m_hThreadHandle,100))
		{
			TerminateThread(m_hThreadHandle,-1);
		}
		CloseHandle(m_hThreadHandle);
		m_hThreadHandle = NULL;
	}
	//关闭句柄
	if(INVALID_SOCKET != m_sock && m_sock)
	{
		closesocket(m_sock);
		WSACleanup();
	}
}
//发送数据
bool UdpNet::SendData(long ISendIp,char* buf,int nLen )
{
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(_Def_UDP_PORT);
	server.sin_addr.S_un.S_addr = ISendIp;
	if(sendto(m_sock,buf,nLen,0,(sockaddr*)&server,sizeof(server)) == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}
//接受数据
void UdpNet::RecvData()//只要外部不调用尽量设成 protected
{
	char recvBuf[_Def_BUFFER_SIZE] = "";
	sockaddr_in addrClient;
	int addrClientSize = sizeof(addrClient);
	int nRecvNum = 0;

	while(!m_isStop)
	{
		nRecvNum = recvfrom(m_sock,recvBuf,sizeof(recvBuf),0,(sockaddr*)&addrClient,&addrClientSize);
		if(nRecvNum > 0)
		{
			char *packBuf = new char[nRecvNum];
			memcpy(packBuf,recvBuf,nRecvNum);
			//通过中介者类发给kernel
			if(m_pMediator)
			{
				
				m_pMediator->DealData(addrClient.sin_addr.S_un.S_addr,packBuf,nRecvNum);
				//回收packBuf由使用数据的人决定
			}
		}
	}
}