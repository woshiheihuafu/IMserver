#pragma once
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<pthread.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/fcntl.h>
#include<signal.h>
#include<arpa/inet.h>
#include<time.h>
#include<sys/epoll.h>
#define TIME_OUT 2
#define EPOLL_MAX 1024
#define PORT 12345
//int epfd;//监听树描述符
inline pthread_mutex_t accept_lock;
//任务
typedef struct
{
    void* (*Business)(void*,void*);
    void* arg;
    void* arg1;
}task_t;
//任务队列
typedef struct
{
    int max;
    int cur;
    int rear;
    int front;
    task_t* list;

}container_t;
//线程池
typedef struct
{
    int max;//线程最大数量
    int min;//线程最小数量
    int que_max;//任务队列最大数量
    int thread_alive;//存活线程数量
    int thread_busy;//活动线程数量
    container_t* ct;//任务队列
    pthread_t* customer_tid;//消费者线程数组
    pthread_mutex_t lock;//
    pthread_cond_t NOT_FULL;//未满条件变量
    pthread_cond_t NOT_EMPTY;//未空条件变量
    pthread_t manager_tid;//管理者线程
    int shutdown;//线程池开关
    int exitcode;//退出ma
}thread_pool_t;

//创建线程池
thread_pool_t* thread_pool_create(int,int,int);
//添加任务
int producer_job_add(thread_pool_t*,task_t);
//消费者任务
void* customer_job(void*);
//管理者任务
void* manager_job(void*);
//线程池销毁
int thread_pool_destroy(thread_pool_t*);
//判断线程是否在忙
int thread_if_busy(pthread_t);
//初始化网络
int net_init();
//任务
void* Business(void*);
//创建监听树
int epfd_create(int);
//接受链接
void* accept_business(void*,void*);
//处理请求
void* deal_business(void*,void*);
//监听
void epoll_listen(int,int,thread_pool_t*,void*);

