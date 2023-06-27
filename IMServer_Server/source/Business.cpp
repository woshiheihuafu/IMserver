#include<config.h>




void* Business(void* arg)
{
    //printf("thread 0x%x finish\n",(unsigned int)pthread_self());
    //while(1);
    printf("thread 0x%x finish\n",(unsigned int)pthread_self());
    sleep(10);
    return NULL;
}
