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
//��ʼ������,���ؿ⣬�����׽��֣���ip��ַ
bool UdpNet::InitNet()
{
	//1.ѡ��Ŀ------���ؿ�
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

	//2.����--------�����׽���
	m_sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);//ʹ��UDPЭ��
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
	//�������޹㲥Ȩ��
	BOOL bval = TRUE;
	setsockopt(m_sock,SOL_SOCKET, SO_BROADCAST,(char *)&bval,sizeof(bval));
	//3.ѡ��ַ-----��ip��ַ
	sockaddr_in server;
	server.sin_family = AF_INET;//         С�˴洢        ��ϴ洢
	server.sin_port = htons(_Def_UDP_PORT);//�����ͱ����������ֽ���ת���������ֽ���
	server.sin_addr.S_un.S_addr = INADDR_ANY;   //���������� 
	//server.sin_addr.S_un.S_addr = inet_addr("10.56.6.43");//�ַ���IP��ַת��Ϊulong����IP��ַ
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
	
	//4.  TODO �������ݡ������������߳�

	m_hThreadHandle = (HANDLE)_beginthreadex(NULL,0,&RecvThrad ,this ,0,NULL);
	return true;
}


unsigned int _stdcall UdpNet::RecvThrad(void* lpvoid)
{
	UdpNet *pThis = (UdpNet*)lpvoid;
	pThis->RecvData();
	return 0;
}
//�ر�����
void UdpNet::UninitNet()
{
	//�˳��߳�

	//�˳���־λ��Ϊtrue�����������˳�
	m_isStop = true;
	//��100ms��û���˳���ǿ��ɱ���߳�
	if(m_hThreadHandle)
	{
		if(WAIT_TIMEOUT == WaitForSingleObject(m_hThreadHandle,100))
		{
			TerminateThread(m_hThreadHandle,-1);
		}
		CloseHandle(m_hThreadHandle);
		m_hThreadHandle = NULL;
	}
	//�رվ��
	if(INVALID_SOCKET != m_sock && m_sock)
	{
		closesocket(m_sock);
		WSACleanup();
	}
}
//��������
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
//��������
void UdpNet::RecvData()//ֻҪ�ⲿ�����þ������ protected
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
			//ͨ���н����෢��kernel
			if(m_pMediator)
			{
				
				m_pMediator->DealData(addrClient.sin_addr.S_un.S_addr,packBuf,nRecvNum);
				//����packBuf��ʹ�����ݵ��˾���
			}
		}
	}
}