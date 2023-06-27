#include<config.h>



int net_init()
{
    //1.获取socket
    int sockfd;
    int err;
    if((sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1)
    {
        perror("socket error");
        exit(0);
    }
    //2.bind
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if((err = bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))) == -1)
    {
        perror("bind error");
        exit(0);
    }
    //3.listen
    if((err = listen(sockfd,10)) == -1)
    {
        perror("listen error");
        exit(0);
    }
    return sockfd;
}
