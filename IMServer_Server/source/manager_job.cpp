#include<config.h>
//扩容
//ALIVE-BUSY <  CUR
//ALIVE + add < MAX


//缩减
// ALIVE = 2 * BUSY,  缩减数量不能小于最小阈值
//管理者给出缩减数量，去唤醒NOT_EMPTY上的线程，线程自杀



void* manager_job(void* arg)
{
    pthread_detach(pthread_self());

    thread_pool_t* ptr = (thread_pool_t*)arg;
    int BUSY;
    int ALIVE;
    int CUR;
    int err;
    int add;
    while(ptr->shutdown)
    {
        //获取数值
        pthread_mutex_lock(&ptr->lock);
        BUSY = ptr->thread_busy;
        ALIVE = ptr->thread_alive;
        CUR = ptr->ct->cur;
        pthread_mutex_unlock(&ptr->lock);
        //printf("\n---->Manager info:\nALIVE:%d\nBUSY:%d\nCUR:%d\nrelax:%d\nBUSY/ALIVE:%.2f%%\nALIVE/ALL:%.2f%%\n\n",ALIVE,BUSY,CUR,ALIVE-BUSY,(double)BUSY/ALIVE*100,(double)ALIVE/ptr->max*100);
        //扩容
        if(ALIVE-BUSY <= CUR || (double)BUSY/ALIVE*100 >= (double)70 && ALIVE + ptr->min <= ptr->max)
        {   
            //printf("1\n");
            for(int i=0,add=ptr->min;i<ptr->max,add>0;i++)
            {
                if(ptr->customer_tid[i] == 0 || thread_if_busy(ptr->customer_tid[i]))
                {
                    if((err = pthread_create(&ptr->customer_tid[i],NULL,customer_job,(void*)ptr)) > 0)
                    {
                        perror("customer pthread_create error");
                        exit(0);
                    }
                    add--;
                    pthread_mutex_lock(&ptr->lock);
                    ++(ptr->thread_alive);
                    pthread_mutex_unlock(&ptr->lock);
                }
            }
        }
        //缩减
        if(BUSY*2 <= ALIVE - BUSY && ALIVE - ptr->min >= ptr->min)
        {
            //printf("2\n");
            pthread_mutex_lock(&ptr->lock);
            ptr->exitcode = ptr->min;
            pthread_mutex_unlock(&ptr->lock);
            //唤醒闲线程
            for(int i=0;i<ptr->min;i++)
            {
                pthread_cond_signal(&ptr->NOT_EMPTY);
            }
        }
        sleep(TIME_OUT);
    }
}
