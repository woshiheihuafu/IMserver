#include"TcpServerMediator.h"
#include"TcpServer.h"
TcpServerMediator::TcpServerMediator()
{
	m_pNet = new TcpServer(this);
}
TcpServerMediator::~TcpServerMediator()
{
	if(m_pNet)
	{
		delete m_pNet;
		m_pNet = NULL;
	}
}
//打开网络
bool TcpServerMediator::OpenNet()
{
	if(!m_pNet->InitNet())
	{
		return false;
	}
	return true;
}
//发送数据
bool TcpServerMediator::SendData(long lSendlp,char *buf,int nLen)
{
	if(!m_pNet->SendData(lSendlp,buf,nLen))
	{
		return false;
	}
	return true;
}
//关闭网络
void TcpServerMediator::CloseNet()
{
	m_pNet->UninitNet();  
}
//处理数据
#include<iostream>
void TcpServerMediator::DealData(long ISendIp,char* buf,int nLen)
{
	//TODO:把数据传给kernel类
	std::cout << buf << std::endl;
	this->SendData(ISendIp,buf,nLen);
}