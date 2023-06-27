#pragma once
#include"INet.h"
#include<list>
#include<map>
class TcpServer:public INet
{
public:
	TcpServer(INetMediator* pMediator);
	~TcpServer();	
	//初始化网络
	bool InitNet();
	//关闭网络
	void UninitNet();
	//发送数据
	bool SendData(long ISendIp,char* buf,int nLen );
protected:
	//接受数据
	void RecvData();//只要外部不调用尽量设成 protected
	  
	SOCKET m_sockListen;
	bool m_isStop;
	list<HANDLE> m_hThreadHandleList;//保存线程句柄
	map<unsigned int,SOCKET> m_mapThreadIdToSocket;//管理线程id 和对应的socket
	static unsigned int _stdcall AcceptThread(void* lpvoid);
	static unsigned int _stdcall RecvThread(void* lpvoid);
	
	
};