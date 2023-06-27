#include<config.h>






int thread_pool_destroy(thread_pool_t* ptr)
{
    pthread_mutex_lock(&ptr->lock);
    ptr->shutdown = 0;
    pthread_mutex_unlock(&ptr->lock);
}
