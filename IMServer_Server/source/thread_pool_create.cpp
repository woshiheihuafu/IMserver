#include<config.h>

thread_pool_t* thread_pool_create(int min,int max,int que_max)
{
    int err;
    //1.创建线程池,开辟空间
    thread_pool_t* pool = (thread_pool_t*)malloc(sizeof(thread_pool_t));
    //2.任务队列,开辟空间
    pool->max = max;
    pool->min = min;
    pool->que_max = que_max;
    pool->thread_alive = 0;
    pool->thread_busy = 0;
    pool->shutdown = 1;
    pool->exitcode = 0;


    pool->ct = (container_t*)malloc(sizeof(container_t));
    pool->ct->list = (task_t*)malloc(sizeof(task_t) * que_max);
    pool->ct->max = que_max;
    pool->ct->rear = 0;
    pool->ct->front = 0;
    pool->ct->cur = 0;
    //3.初始化线程锁,条件变量
    if(pthread_mutex_init(&pool->lock,NULL) != 0 || pthread_cond_init(&pool->NOT_FULL,NULL) != 0 || pthread_cond_init(&pool->NOT_EMPTY,NULL) != 0)
    {
        perror("mutex_init or cond_init error");
        exit(0);
    }
    //4.创建消费者线程
    pool->customer_tid = (pthread_t*)malloc(sizeof(pthread_t) * max);
    for(int i=0;i<min;i++)
    {
        if((err = pthread_create(&pool->customer_tid[i],NULL,customer_job,(void*)pool)) > 0)
        {
            perror("customer pthread_create error");
            exit(0);
        }
        printf("create custormer sucess\n");
        ++(pool->thread_alive);

    }
    bzero(pool->customer_tid,sizeof(pthread_t) * min);
    //5.创建管理者线程
    if((err = pthread_create(&pool->manager_tid,NULL,manager_job,(void*)pool)) > 0)
    {
        perror("manager pthread_create error");
        exit(0);
    }
    printf("create manager sucess\n");
    return pool;

}
