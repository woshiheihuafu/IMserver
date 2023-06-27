#include<config.h>




int epfd_create(int sockfd)
{
    int epfd;
    int err;
    if((epfd = epoll_create(EPOLL_MAX)) == -1)
    {
        perror("epoll_create error");
    }
    struct epoll_event Node;
    Node.data.fd = sockfd;
    Node.events = EPOLLIN | EPOLLET;
    if((err = epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&Node)) == -1)
    {
        perror("epoll_ctl add sockfd error");
        exit(0);
    }
    return epfd;
}
