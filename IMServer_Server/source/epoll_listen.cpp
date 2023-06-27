#include<config.h>




void epoll_listen(int sockfd,int epfd,thread_pool_t* ptr,void* lpvoid)
{
    struct epoll_event ready_arry[EPOLL_MAX];
    int err;
    int readycode;
    task_t ts;
    while(ptr->shutdown)
    {
        if((readycode = epoll_wait(epfd,ready_arry,EPOLL_MAX,-1)) == -1)
        {
            perror("epoll_wait error");
            exit(0);
        }
        int flag = 0;
        while(readycode)
        {
            if(ready_arry[flag].data.fd == sockfd)
            {
		printf("server sockfd ready\n");
                ts.Business = accept_business;
                ts.arg = (void*)&sockfd;
                ts.arg1 = lpvoid;
                producer_job_add(ptr,ts);

            }
            else
            {
		printf("client sockfd ready\n");
                ts.Business = deal_business;
                ts.arg = (void*)&ready_arry[flag].data.fd;
		ts.arg1 = lpvoid;
                producer_job_add(ptr,ts);
            }
            flag++;
            readycode--;
        }
    }
    return;
}
