#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define MEMORY_SIZE 100   //后面2个字符用于标志是否读写完成
#define READ  buffer[MEMORY_SIZE]  //读标志
#define WRITE buffer[MEMORY_SIZE+1] //写标志
#define FLAG  buffer[MEMORY_SIZE+1]  //读写完成标志

//由于sys/sem.h 与 linux/sem.h有冲突，所以单独定义
/* arg for semctl system calls. */
union semun {
	int val;			/* value for SETVAL */
	struct semid_ds *buf;	/* buffer for IPC_STAT & IPC_SET */
	unsigned short *array;	/* array for GETALL & SETALL */
	struct seminfo *__buf;	/* buffer for IPC_INFO */
	void *__pad;
};

int segment_id;
int full, empty, mutex;  //信号灯
pid_t p_read, p_write;

//函数审明
void P(int semid);
void V(int semid);
void sem_ass(int semid ,int val);
void ReadBuf(const char* fname);
void WriteBuf(const char* fname);

int main(int argc, char const *argv[])
{
    //判断参数是否完整
    if(argc != 3)
    {
        printf("输入参数不正确！\n");
        printf("请输入两个参数，前一个参数为被复制的文件名，后一个参数为目标文件名 ！\n");
        return 0;
    }

    //创建共享内存组
    segment_id = shmget(IPC_PRIVATE, MEMORY_SIZE+3, IPC_CREAT|0666);  //+2是为了用最后2个字符用于进程间通信
    //其中 [MEMORY_SIZE] 用于标志写位置， [MEMORY_SIZE+1] 用于标志读位置  [MEMORY_SIZE+2]标志读写完成

    //共享内存后面的标志量赋值
    char *buffer;
    buffer = (char*)shmat(segment_id, NULL, SHM_R|SHM_W);
    READ = 0;
    WRITE = 0;
    FLAG = 1;

    //创建信号灯及赋值
    full = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666); //将共享缓冲区问题转化为生产者消费者问题
    empty = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    mutex = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);

    sem_ass(full, 0);
    sem_ass(empty, MEMORY_SIZE);
    sem_ass(mutex, 1);

    //创建进程
    if( (p_read=fork() )== -1)
    {
        printf("Create Process failed! \n");
        return -1;
    }
    else if(p_read > -1)  //确保在父进程中
    {
        if( (p_write=fork()) == -1)
        {
            printf("Create Process failed! \n");
            return -1;
        }
    }

    if(p_read == 0)
    {
        ReadBuf(argv[2]);
    }
    else if(p_write == 0)
    {
        WriteBuf(argv[1]);
    }

    wait(NULL); //等待子进程结束
    printf("Process has finished! \n");

    //删除信号灯
    semctl(full, 0, IPC_RMID);
    semctl(empty, 0, IPC_RMID);
    semctl(mutex, 0, IPC_RMID);

    //解绑内存
    shmdt(buffer);
    //释放内存
    shmctl(segment_id, IPC_RMID, 0);

    return 0;
}

void ReadBuf(const char* fname)
{
    char *buffer;
    char p;
    FILE *fp;
    //绑定内存
    buffer = (char*)shmat(segment_id, NULL, SHM_R|SHM_W);
    //buffer[MEMORY_SIZE] 用于标志写位置， buffer[MEMORY_SIZE+1] 用于标志读位置
    //buffer[MEMORY_SIZE+2]标志读写完成 若为1则标志未完成，若为0则标志读写完成

    //打开需要复制的目标文件
    fp = fopen(fname, "wb");  //以二进制写入
    if(fp == NULL)
    {
        printf("文件创建失败！\n");
        return ;
    }

    while(READ != WRITE|| FLAG)  //FALG 为读写完成标记
    {
        P(full);
        P(mutex);  //互斥锁

        p = buffer[(int)READ]; //获取数据
        READ = (READ + 1)% MEMORY_SIZE; //+1
        //写入目标文件
        fwrite(&p, sizeof(char), 1, fp);

        V(mutex);
        V(empty);
    }
    fclose(fp);  //关闭文件

    //解绑内存
    shmdt(buffer);

    exit(0);
}

void WriteBuf(const char* fname)
{
    char *buffer;
    char c;
    FILE *fp;
    //绑定内存
    buffer = (char*)shmat(segment_id, NULL, SHM_R|SHM_W);

    //打开需要复制的原文件
    fp = fopen(fname, "rb+");
    if(fp == NULL)
    {
        printf("文件打开失败！\n");
        return ;
    }

    while(fread(&c, sizeof(char), 1, fp) != EOF)
    {
        P(empty);
        P(mutex);

        buffer[(int)WRITE] = c;
        WRITE = (WRITE+1)%MEMORY_SIZE;

        V(mutex);
        V(full);
    }
    FLAG = 0;  //读取文件结束，标志0
    fclose(fp); //关闭文件

    //解绑内存
    shmdt(buffer);

    exit(0);
}


void P(int semid)
{
    struct sembuf sem;
    sem.sem_num = 0;
    sem.sem_op = -1;   //P执行-1操作
    sem.sem_flg = 0;   //这里可以直接为0，没有关系

    semop(semid, &sem, 1);   //信号量操作

    return;
}

void V(int semid)
{
    struct sembuf sem;
    sem.sem_num = 0;
    sem.sem_op = +1;   //V执行+1操作
    sem.sem_flg = 0;   //这里可以直接为0，没有关系

    semop(semid, &sem, 1);   //信号量操作

    return;
}

//信号灯赋值
void sem_ass(int semid ,int val)
{
    union semun arg;
    arg.val = val;
    semctl(semid, 0, SETVAL, arg);
}
