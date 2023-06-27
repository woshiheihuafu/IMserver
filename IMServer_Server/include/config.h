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
//int epfd;//������������
//pthread_mutex_t accept_lock;
//����
typedef struct
{
    void* (*Business)(void*,void*);
    void* arg;
    void* arg1;
}task_t;
//�������
typedef struct
{
    int max;
    int cur;
    int rear;
    int front;
    task_t* list;

}container_t;
//�̳߳�
typedef struct
{
    int max;//�߳��������
    int min;//�߳���С����
    int que_max;//��������������
    int thread_alive;//����߳�����
    int thread_busy;//��߳�����
    container_t* ct;//�������
    pthread_t* customer_tid;//�������߳�����
    pthread_mutex_t lock;//
    pthread_cond_t NOT_FULL;//δ����������
    pthread_cond_t NOT_EMPTY;//δ����������
    pthread_t manager_tid;//�������߳�
    int shutdown;//�̳߳ؿ���
    int exitcode;//�˳�ma
}thread_pool_t;

//�����̳߳�
thread_pool_t* thread_pool_create(int,int,int);
//�������
int producer_job_add(thread_pool_t*,task_t);
//����������
void* customer_job(void*);
//����������
void* manager_job(void*);
//�̳߳�����
int thread_pool_destroy(thread_pool_t*);
//�ж��߳��Ƿ���æ
int thread_if_busy(pthread_t);
//��ʼ������
int net_init();
//����
void* Business(void*);
//����������
int epfd_create(int);
//��������
void* accept_business(void*,void*);
//��������
void* deal_business(void*,void*);
//����
void epoll_listen(int,int,thread_pool_t*,void*);



//�˿ں�
#pragma once
#include<stdio.h>
#define DEF_UDP_PORT (54325)
#define DEF_TCP_PORT (54321)
#define DEF_TCP_SERVER_IP ("10.56.53.247")
#define DEF_UDP_SERVER_IP ("10.56.53.247")
//���ջ�������С
#define DEF_BUFFER_SIZE (4096)

//���룬�绰����С
#define DEF_MAX_SIZE       (100)
#define DEF_PROTOCAL_COUNT (10)
#define Def_CONTENT_SIZE    (1024)
//����Ƶ����С
#define DEF_MAX_VIDEO_SIZE (20480)
//�ļ�����С
#define DEF_MAX_FILE_RQ_SIZE (1024 * 9)
#define DEF_MAX_FILE_SIZE  (1024 * 8)
//���ִ�С
#define DEF_NAME_SIZE (96)
//�������ݴ�С


//Э��ӳ����С
#define DEF_PROTOCAL_MAX  (100)
#define DEF_PROTOCAL_COUNT (10)
//����Э��ͷ
#define DEF_PROTOCAL_BASE (1000)
//����
#define DEF_UDP_ONLINE_RQ (DEF_PROTOCAL_BASE+1)
#define DEF_UDP_ONLINE_RS (DEF_PROTOCAL_BASE+2)
//����
#define DEF_UDP_OFFLINE_RQ (DEF_PROTOCAL_BASE+3)
//����
#define DEF_UDP_CHAT_RQ (DEF_PROTOCAL_BASE+4)


//���ļ�
// 
// 
// 
//ע��
#define DEF_TCP_REGISTER_RQ (DEF_PROTOCAL_BASE + 10)
#define DEF_TCP_REGISTER_RS (DEF_PROTOCAL_BASE + 11)
//��¼
#define DEF_TCP_LOGIN_RQ (DEF_PROTOCAL_BASE + 12)
#define DEF_TCP_LOGIN_RS (DEF_PROTOCAL_BASE + 13)
//������Ϣ
#define DEF_TCP_FRIEND_INFO (DEF_PROTOCAL_BASE + 14)
//��Ӻ���
#define DEF_TCP_ADD_FRIEND_RQ (DEF_PROTOCAL_BASE + 15)
#define DEF_TCP_ADD_FRIEND_RS (DEF_PROTOCAL_BASE + 16)
//����
#define DEF_TCP_CHAT_RQ (DEF_PROTOCAL_BASE + 17)
#define DEF_TCP_CHAT_RS (DEF_PROTOCAL_BASE + 18)
//����
#define DEF_TCP_OFFLINE_RQ (DEF_PROTOCAL_BASE + 19)
//����Ⱥ��
#define DEF_TCP_CREATEGROUP_RQ (DEF_PROTOCAL_BASE + 20)
#define DEF_TCP_CREATEGROUP_RS (DEF_PROTOCAL_BASE + 21)
#define DEF_TCP_GROUP_INFO    (DEF_PROTOCAL_BASE + 22)
#define DEF_TCP_GROUP_CHAT_RQ (DEF_PROTOCAL_BASE + 23)
//����Ⱥ��
#define DEF_TCP_JOIN_GROUP_RQ (DEF_PROTOCAL_BASE + 24)
#define DEF_TCP_JOIN_GROUP_RS (DEF_PROTOCAL_BASE + 25)
//ɾ������
#define DEF_TCP_DELETE_FRIEND_RQ (DEF_PROTOCAL_BASE + 29)
//�����¼
#define DEF_TCP_CHAT_HISTORY_RQ (DEF_PROTOCAL_BASE + 30)
//��Ƶ����
#define DEF_UDP_VIDEO_RQ (DEF_PROTOCAL_BASE + 31)
//��Ƶ����
#define DEF_TCP_VIDEO_INFO_RQ (DEF_PROTOCAL_BASE + 32)
#define DEF_TCP_VIDEO_INFO_RS (DEF_PROTOCAL_BASE + 33)

//���ؽ��
//ע�����󷵻ؽ��
#define register_success (0)//ע��ɹ�
#define user_is_exit    (1)//�û��Ѵ���
#define parameter_error (2)//��������
#define register_fail  (3)
//��¼���󷵻ؽ��
#define login_success    (0)//��¼�ɹ�
#define password_error  (1)//�������
#define user_not_exist  (2)//�û�������
#define parameter_login_error       (3) 
//��Ӻ���������
#define add_success      (0)//��ӳɹ�
#define no_this_user    (1)//û�д��û�
#define user_refuse     (2)//�û��ܾ�
#define user_offline    (3)//�û�������
#define add_fail         (4)
#define already_add    (5)//�Ѿ��д˺���
//������Ϣ���ؽ��
#define send_success     (0)//���ͳɹ�
#define send_error      (1)//����ʧ��
//����״̬
#define status_online   (1)//�û�����
#define status_offline  (0)//�û�������
//����Ⱥ������
#define group_is_exit (0)
#define create_success (1)
#define create_error (2)
//Ⱥ�����췵�ؽ��
#define group_send_error (0)
//����Ⱥ�鷵�ؽ��
#define join_group_error (0)
#define join_group_refuse (1)
#define join_group_sucess (2)
#define no_this_group (3)
#define alreday_join (4)
#define creator_offline (5)

//����ṹ��
//��������Э��ͷ��������ͷ�񣬸���ǩ����
struct STRU_UDP_ONLINE 
{
	STRU_UDP_ONLINE():nType(DEF_UDP_ONLINE_RQ)
	{
		memset(nNmae, 0, DEF_NAME_SIZE);
	}
	int nType;
	char nNmae[DEF_NAME_SIZE];
};
//��������Э��ͷ��
struct STRU_UDP_OFFLINE_RQ
{
	STRU_UDP_OFFLINE_RQ():nType(DEF_UDP_OFFLINE_RQ)
	{
	}
	int nType;
};
//��������Э��ͷ�����ݣ�
struct STRU_UDP_CHAT_RQ
{
	STRU_UDP_CHAT_RQ() :nType(DEF_UDP_CHAT_RQ)
	{
		memset(nContent, 0, DEF_NAME_SIZE);
	}
	int nType;
	char nContent[DEF_NAME_SIZE];
};


//TCP�ṹ��
typedef int PackType;
//ע��,Э��ͷ���ǳƣ����룬�ֻ���
struct STRU_TCP_REGISTER_RQ
{
	STRU_TCP_REGISTER_RQ() :type(DEF_TCP_REGISTER_RQ)
	{
		memset(name, 0, DEF_MAX_SIZE);
		memset(tel, 0, DEF_MAX_SIZE);
		memset(password, 0, DEF_MAX_SIZE);
	}
	PackType type;
	char name[DEF_MAX_SIZE];
	char tel[DEF_MAX_SIZE];
	char password[DEF_MAX_SIZE];
};
//ע��ظ���Э��ͷ,ע����
struct STRU_TCP_REGISTER_RS
{
	STRU_TCP_REGISTER_RS() :type(DEF_TCP_REGISTER_RS), result(register_success)
	{
	}
	PackType type;
	int result;
};
//��¼��Э��ͷ���绰���룬����
struct STRU_TCP_LOGIN_RQ
{
	STRU_TCP_LOGIN_RQ() :type(DEF_TCP_LOGIN_RQ)
	{
		memset(tel, 0, DEF_MAX_SIZE);
		memset(password, 0, DEF_MAX_SIZE);
	}
	PackType type;
	char tel[DEF_MAX_SIZE];
	char password[DEF_MAX_SIZE];
};
//��¼�ظ���Э��ͷ����¼���,�û�id
struct STRU_TCP_LOGIN_RS
{
	STRU_TCP_LOGIN_RS() :type(DEF_TCP_LOGIN_RS), result(login_success), userId(0)
	{
	}
	PackType type;
	int result;
	int userId;
};
//������Ϣ,Э��ͷ���ǳƣ��û�״̬������ǩ����ͷ���û�id
struct STRU_TCP_FRIEND_INFO
{
	STRU_TCP_FRIEND_INFO() :type(DEF_TCP_FRIEND_INFO), state(status_online), iconId(0), userId(0)
	{
		memset(name, 0, DEF_MAX_SIZE);
		memset(feling, 0, DEF_MAX_SIZE);
	}
	PackType type;
	char name[DEF_MAX_SIZE];
	char feling[DEF_MAX_SIZE];
	int state;
	int iconId;
	int userId;

};
//��Ӻ�������Э��ͷ���û��ǳƣ��û�id������ӵ��û��ǳ�
struct STRU_TCP_ADD_FRIEND_RQ
{
	STRU_TCP_ADD_FRIEND_RQ() :type(DEF_TCP_ADD_FRIEND_RQ), userId(0), userIcon(0)
	{
		memset(userPhone, 0, DEF_MAX_SIZE);
		memset(friendPhone, 0, DEF_MAX_SIZE);
		memset(userName, 0, DEF_MAX_SIZE);
		memset(userFeel, 0, DEF_MAX_SIZE);
	}
	PackType type;
	char userPhone[DEF_MAX_SIZE];
	char friendPhone[DEF_MAX_SIZE];
	char userName[DEF_MAX_SIZE];
	char userFeel[DEF_MAX_SIZE];
	int userIcon;
	int userId;
};
//��Ӻ��ѻظ���Э��ͷ��������û����ǳƺ�id,��ӽ��,������������û���id
struct STRU_TCP_ADD_FRIEND_RS
{
	STRU_TCP_ADD_FRIEND_RS() :type(DEF_TCP_ADD_FRIEND_RS), friendId(0), userId(0), result(add_success), friendIcon(0)
	{
		memset(friendName, 0, DEF_MAX_SIZE);
		memset(friendFeel, 0, DEF_MAX_SIZE);
	}
	PackType type;
	int result;
	char friendName[DEF_MAX_SIZE];
	char friendFeel[DEF_MAX_SIZE];
	int friendId;
	int friendIcon;
	int userId;
};
//��������Э��ͷ���������ݣ������ߵ�id�������ߵ�id
struct STRU_TCP_CHAT_RQ
{
	STRU_TCP_CHAT_RQ() :type(DEF_TCP_CHAT_RQ), friendId(0), result(0), userId(0)
	{
		memset(content, 0, DEF_MAX_SIZE);
	}
	PackType type;
	int result;
	char content[Def_CONTENT_SIZE];
	int friendId;
	int userId;
	char time[Def_CONTENT_SIZE];
};
//����ظ������ͽ���������ߺͽ����ߵ�id
struct STRU_TCP_CHAT_RS
{
	STRU_TCP_CHAT_RS() :type(DEF_TCP_CHAT_RS), friendId(0), result(send_success), userId(0)
	{
	}
	PackType type;
	int result;
	int friendId;
	int userId;
};
//��������Э��ͷ���û�id
struct STRU_TCP_OFFLINE_RQ
{
	STRU_TCP_OFFLINE_RQ() :type(DEF_TCP_OFFLINE_RQ), userId(0),result()
	{
	}
	PackType type;
	int result;
	int userId;
};
//����Ⱥ������
struct STRU_TCP_CREATEGROUP_RQ
{
	STRU_TCP_CREATEGROUP_RQ() :type(DEF_TCP_CREATEGROUP_RQ), userId(0)
	{
		memset(groupName, 0, DEF_MAX_SIZE);
	}
	PackType type;
	char groupName[DEF_MAX_SIZE];
	int userId;
};
///����Ⱥ��ظ�
struct STRU_TCP_CREATEGROUP_RS
{
	STRU_TCP_CREATEGROUP_RS() :type(DEF_TCP_CREATEGROUP_RS), result(2), groupId(-1), creatorId(0), groupIcon(0)
	{
		memset(groupName, 0, DEF_MAX_SIZE);
	}
	PackType type;
	int groupId;
	int result;
	char groupName[DEF_MAX_SIZE];
	int creatorId;
	int groupIcon;
};
//Ⱥ����Ϣ
struct STRU_TCP_GROUP_INFO
{
	STRU_TCP_GROUP_INFO() :type(DEF_TCP_GROUP_INFO), groupId(0),creatorId(0), groupIcon(0)
	{
		memset(groupName, 0, DEF_MAX_SIZE);
	}
	PackType type;
	int groupId;
	char groupName[DEF_MAX_SIZE];
	int creatorId;
	int groupIcon;
};
//Ⱥ������
struct STRU_TCP_GROUP_CHAT_RQ
{
	STRU_TCP_GROUP_CHAT_RQ() :type(DEF_TCP_GROUP_CHAT_RQ), groupId(0), userId(0)
	{
		memset(content, 0, DEF_MAX_SIZE);
		memset(userName, 0, DEF_MAX_SIZE);
	}
	PackType type;
	int groupId;
	int userId;
	char userName[DEF_MAX_SIZE];
	char content[DEF_MAX_SIZE];
};
//����Ⱥ������
struct STRU_TCP_JOIN_GROUP_RQ
{
	STRU_TCP_JOIN_GROUP_RQ() :type(DEF_TCP_JOIN_GROUP_RQ), userId(0), groupId(0)
	{
		memset(userName, 0, DEF_MAX_SIZE);
		memset(groupName, 0, DEF_MAX_SIZE);
	}
	PackType type;
	int userId;
	int groupId;
	char userName[DEF_MAX_SIZE];
	char groupName[DEF_MAX_SIZE];

};
//����Ⱥ��ظ�
struct STRU_TCP_JOIN_GROUP_RS
{
	STRU_TCP_JOIN_GROUP_RS() :type(DEF_TCP_JOIN_GROUP_RS), result(0), userId(0), groupId(0),groupIcon(0), creatorId(0)
	{
		memset(groupName, 0, DEF_MAX_SIZE);
	}
	PackType type;
	int result;
	int userId;
	int groupId;
	int groupIcon;
	char groupName[DEF_MAX_SIZE];
	int creatorId;
};


//�ļ���Ϣ
#define DEF_TCP_FILE_INFO_RQ (DEF_PROTOCAL_BASE + 26)
//�ļ���
#define DEF_TCP_FILE_BLOCK_RQ (DEF_PROTOCAL_BASE + 27)
#define DEF_TCP_FILE_INFO_RS (DEF_PROTOCAL_BASE + 28)
#define MAX_FILE_PATH (512)
#define MAX_FILE_CONTENT (8*1024)



#define wait_file (0)//�ȴ���Ӧ
#define aggre_file (1)//ͬ�����
#define refuse_file (2)//�ܾ�����
#define not_login (3)//������
//�ļ���Ϣ����
struct STRU_FILE_INFO_RQ
{
	STRU_FILE_INFO_RQ():type(DEF_TCP_FILE_INFO_RQ), fileSize(0),friendId(0),userId(0)
	{
		memset(fileId, 0, MAX_FILE_PATH);
		memset(fileName, 0, MAX_FILE_PATH);
	}
	PackType type;//Э��ͷ
	char fileId[MAX_FILE_PATH];//�ļ���ʶ��
	char fileName[MAX_FILE_PATH];//�ļ���
	long long fileSize;//�ļ���С
	int friendId;
	int userId;
};
//�ļ���
struct STRU_FILE_BLOCK_RQ
{
	STRU_FILE_BLOCK_RQ() :type(DEF_TCP_FILE_BLOCK_RQ), fileBlockSize(0),friendId(0),userId(0),result(0)
	{
		memset(fileId, 0, MAX_FILE_PATH);
		memset(fileContent, 0, MAX_FILE_CONTENT);
		memset(SHA_256, 0, 32);
	}
	PackType type;//Э��ͷ
	char fileId[MAX_FILE_PATH];//�ļ���ʶ��
	char fileContent[MAX_FILE_CONTENT];
	int fileBlockSize;
	int friendId;
	int userId;
	char SHA_256[1024];
	int result;
};
//�����ļ��ظ�
struct STRU_FILE_INFO_RS
{
	STRU_FILE_INFO_RS() :type(DEF_TCP_FILE_INFO_RS), result(0), userId(0), friendId(0)
	{
	}
	PackType type;
	int result;
	int userId;
	int friendId;
};
//�����ļ���Ϣ:�ļ���־���ļ������ļ�·������ǰλ�ã��ܴ�С���ļ�ָ��
struct FileInfo
{
	FileInfo() :pos(0), nFileSize(0),pFile(NULL)
	{
		memset(fileId, 0, MAX_FILE_PATH);
		memset(fileName, 0, MAX_FILE_PATH);
		memset(filePath, 0, MAX_FILE_PATH);
	}
	char fileId[MAX_FILE_PATH];//�ļ���ʶ��
	char fileName[MAX_FILE_PATH];//�ļ���
	char filePath[MAX_FILE_PATH];
	long long pos;
	long long nFileSize;
	FILE *pFile;
};









////�����ļ�����
//struct STRU_TCP_SEND_FILE_RQ
//{
//	STRU_TCP_SEND_FILE_RQ() :type(DEF_TCP_SEND_FILE_RQ), friendId(0), userId(0), ifend(false), ifbegin(false), fileSize(0), allSize(0)
//	{
//		memset(fileBuffer, 0, DEF_MAX_FILE_RQ_SIZE);
//		memset(fileName, 0, DEF_MAX_SIZE);
//	}
//	PackType type;
//	char fileBuffer[DEF_MAX_FILE_RQ_SIZE];
//	int friendId;
//	int userId;
//	bool ifend;
//	bool ifbegin;
//	char fileName[DEF_MAX_SIZE];
//	long long fileSize;
//	long long allSize;
//};

////�ļ���Ϣ����
//struct STRU_TCP_SEND_FILE_INFO
//{
//	STRU_TCP_SEND_FILE_INFO() :type(DEF_TCP_SEND_FILE_INFO), fileSize(0),userId(0),friendId(0)
//	{
//		memset(fileName, 0, DEF_MAX_SIZE);
//	}
//	PackType type;
//	char fileName[DEF_MAX_SIZE];
//	long long fileSize;
//	int userId;
//	int friendId;
//};
//struct STRU_TCP_SEND_FILE_RQ
//{
//	STRU_TCP_SEND_FILE_RQ() :type(DEF_TCP_SEND_FILE_RQ), nBlockSize(0)
//	{
//		memset(fileBuffer, 0, DEF_MAX_FILE_SIZE);
//	}
//	PackType type;
//	char fileBuffer[DEF_MAX_FILE_SIZE];
//	int nBlockSize;
//};



#define delete_fail (0)
#define delete_success (1)
//ɾ����������
struct STRU_TCP_DELETE_FRIEND_RQ
{
	STRU_TCP_DELETE_FRIEND_RQ() :type(DEF_TCP_DELETE_FRIEND_RQ), userId(-1), friendId(-1),result(0)
	{

	}
	PackType type;
	int userId;
	int friendId;
	int result;
};
//�����¼�ṹ��
struct STRU_TCP_CHAT_HISTORY_RQ
{
	STRU_TCP_CHAT_HISTORY_RQ() :type(DEF_TCP_CHAT_HISTORY_RQ), userid1(0), userid2(0)
	{
		memset(infotime, 0, Def_CONTENT_SIZE);
		memset(info, 0, Def_CONTENT_SIZE);
	}
	PackType type;
	int userid1;
	int userid2;
	char infotime[Def_CONTENT_SIZE];
	char info[Def_CONTENT_SIZE];
};
//��Ƶ�ṹ��
struct STRU_UDP_VIDEO_RQ
{
	STRU_UDP_VIDEO_RQ() :type(DEF_UDP_VIDEO_RQ), userid1(0), userid2(0)
	{
		memset(videoinfo, 0, DEF_MAX_VIDEO_SIZE);
	}
	PackType type;
	int userid1;
	int userid2;
	char videoinfo[DEF_MAX_VIDEO_SIZE];
};
struct STRU_TCP_VIDEO_INFO_RQ
{
	STRU_TCP_VIDEO_INFO_RQ() :type(DEF_TCP_VIDEO_INFO_RQ), userid1(0), userid2(0)
	{

	}
	PackType type;
	int userid1;
	int userid2;
};
struct STRU_TCP_VIDEO_INFO_RS
{
	STRU_TCP_VIDEO_INFO_RS() :type(DEF_TCP_VIDEO_INFO_RS), userid1(0), userid2(0), result(0)
	{

	}
	PackType type;
	int userid1;
	int userid2;
	int result;
};
