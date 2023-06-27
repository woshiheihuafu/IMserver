#include"TcpClientMediator.h"
#include"TcpClient.h"
TcpClientMediator::TcpClientMediator()
{
	m_pNet = new TcpClient(this);
}
TcpClientMediator::~TcpClientMediator()
{
	if(m_pNet)
	{
		delete m_pNet;
		m_pNet = NULL;
	}
}
//打开网络
bool TcpClientMediator::OpenNet()
{
	if(!m_pNet->InitNet())
	{
		return false;
	}
	return true;
}
//发送数据
bool TcpClientMediator::SendData(long lSendlp,char *buf,int nLen)
{
	if(!m_pNet->SendData(lSendlp,buf,nLen))
	{
		return false;
	}
	return true;
}
//关闭网络
void TcpClientMediator::CloseNet()
{
	m_pNet->UninitNet();  
}
//处理数据
#include<iostream>
void TcpClientMediator::DealData(long lSendIp,char* buf,int nLen)
{
    //把数据传给kernel类
	std::cout<<buf<<std::endl;
    Q_EMIT SIG_ReadyData(lSendIp,buf,nLen);
}
