#include<config.h>
#include<TcpServer.h>
void* accept_business(void* arg,void* arg1)
{
    TcpServer* pThis = (TcpServer*)arg1;
    int sockfd = *(int*)arg;
    int clientfd;
    struct sockaddr_in client_addr;
    socklen_t addrlen;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(PORT);
    pthread_mutex_lock(&pThis->accept_lock);
    if((clientfd = accept(sockfd,(struct sockaddr*)&client_addr,&addrlen)) == -1)
    {
        perror("accept error");
        return 0;
    }
    printf("ip = %s is connect\n",inet_ntoa(client_addr.sin_addr));
    pthread_mutex_unlock(&pThis->accept_lock);
    struct epoll_event Node;
    Node.data.fd = clientfd;
    Node.events = EPOLLIN|EPOLLONESHOT;
    if(epoll_ctl(pThis->m_epfd,EPOLL_CTL_ADD,clientfd,&Node) == -1)
    {
        perror("epoll_ctl add error");
    }
    return NULL;
}
