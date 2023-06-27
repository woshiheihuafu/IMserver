#include"UdpNetMediator.h"
#include"UdpNet.h"
UdpNetMediator::UdpNetMediator()
{
	m_pNet = new UdpNet(this);
}
UdpNetMediator::~UdpNetMediator()
{
	if(m_pNet)
	{
		delete m_pNet;
		m_pNet = NULL;
	}
}
//打开网络
bool UdpNetMediator::OpenNet()
{
	if(!m_pNet->InitNet())
	{
		return false;
	}
	return true;
}
//发送数据
bool UdpNetMediator::SendData(long lSendlp,char *buf,int nLen)
{
	if(!m_pNet->SendData(lSendlp,buf,nLen))
	{
		return false;
	}
	return true;
}
//关闭网络
void UdpNetMediator::CloseNet()
{
	m_pNet->UninitNet();  
}
//处理数据
#include<iostream>
void UdpNetMediator::DealData(long ISendIp,char* buf,int nLen)
{
	//TODO:把数据传给kernel类
	std::cout<<buf<<std::endl;
}