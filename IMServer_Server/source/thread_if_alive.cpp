#include<config.h>

int thread_if_busy(pthread_t tid)
{
    pthread_kill(tid,0);
    if(errno = ESRCH)
    {
        return 1;
    }
    return 0;
}

