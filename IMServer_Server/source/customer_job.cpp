#include<config.h>

void* customer_job(void* arg)
{
    
    pthread_detach(pthread_self());
    thread_pool_t* ptr = (thread_pool_t*)arg;
    task_t Node;
    while(ptr->shutdown)
    {
        pthread_mutex_lock(&ptr->lock);
        while(ptr->ct->cur == 0)
        {
            pthread_cond_wait(&ptr->NOT_EMPTY,&ptr->lock);
            if(ptr->shutdown == 0)
            {
                pthread_mutex_unlock(&ptr->lock);
                pthread_exit(NULL);
            }
            if(ptr->exitcode != 0)
            {
                --(ptr->exitcode);
                --(ptr->thread_alive);
                pthread_mutex_unlock(&ptr->lock);
                pthread_exit(NULL);
            }
        }
        Node.Business = ptr->ct->list[ptr->ct->rear].Business;
        Node.arg = ptr->ct->list[ptr->ct->rear].arg;
	Node.arg1 = ptr->ct->list[ptr->ct->rear].arg1;
        ptr->ct->rear = (ptr->ct->rear + 1) % ptr->ct->max;
        --(ptr->ct->cur);
        ++(ptr->thread_busy);
        pthread_mutex_unlock(&ptr->lock);

        pthread_cond_signal(&ptr->NOT_FULL);
        (Node.Business)(Node.arg,Node.arg1);//执行任务

        pthread_mutex_lock(&ptr->lock);
        --(ptr->thread_busy);
        pthread_mutex_unlock(&ptr->lock);

    }
    pthread_exit(NULL);
}

