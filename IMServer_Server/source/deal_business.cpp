#include<config.h>
#include<TcpServer.h>
void* deal_business(void* arg,void* arg1)
{
	//接收包大小
	int clientfd = *(int*)arg;
	TcpServer* pThis = (TcpServer*)arg1;
	socklen_t recvNum = 0;
	int nPackSize = 0;
	if(((recvNum = recv(clientfd, (char*)&nPackSize, sizeof(nPackSize), 0)) > 0) && nPackSize > 0 && nPackSize < 30000)
	{
	    printf("pthreadid = 0x%x npacksize = %d,clientfd = %d,recvNum = %d\n",(unsigned int)pthread_self(),nPackSize,clientfd,recvNum);
	    //接收包内容
		char* buf = new char[nPackSize];
		int offset = 0;
		int recvSize = 0;
		int type = 0;
		while (nPackSize)
		{
			recvNum = recv(clientfd, buf + offset, nPackSize, 0);
			if (recvNum > 0)
			{
				recvSize += recvNum;
				offset += recvNum;
				nPackSize -= recvNum;
			}
			else
			{
				return NULL;
			}
		}
		type = *(int*)buf;
		pThis->m_pMediator->DealData(clientfd, buf, offset);
		struct epoll_event Node;
		Node.data.fd = clientfd;
		Node.events = EPOLLIN|EPOLLONESHOT;
		if(epoll_ctl(pThis->m_epfd,EPOLL_CTL_MOD,clientfd,&Node) == -1)
		{
		    perror("epoll_ctl mod error");
		}
		if(type == DEF_TCP_OFFLINE_RQ)
		{
			printf("clientfd = %d disconnect-------------------------------------\n",clientfd);
			//从监听树yichu
			epoll_ctl(pThis->m_epfd,EPOLL_CTL_DEL,clientfd,NULL);
			close (clientfd);
		}
		return NULL;
	}
    return NULL;
}
