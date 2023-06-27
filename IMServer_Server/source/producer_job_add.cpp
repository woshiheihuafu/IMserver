#include<config.h>




int producer_job_add(thread_pool_t* pool,task_t task)
{
    if(pool->shutdown)//添加一次任务
    {
        //上锁
        pthread_mutex_lock(&pool->lock);
        while(pool->ct->cur == pool->ct->max)
        {
            pthread_cond_wait(&pool->NOT_FULL,&pool->lock);
        }
        pool->ct->list[pool->ct->front].Business = task.Business;
        pool->ct->list[pool->ct->front].arg = task.arg;
	pool->ct->list[pool->ct->front].arg1 = task.arg1;
        pool->ct->front = (pool->ct->front + 1) % pool->ct->max;
        ++(pool->ct->cur);
        //解锁
        pthread_mutex_unlock(&pool->lock);
        //唤醒消费ze
        pthread_cond_signal(&pool->NOT_EMPTY);
        //printf("thread 0x%x add job sucess\n",(unsigned int)pthread_self());
        
    }
    return 0;
}



