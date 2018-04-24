#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define MEMORY_SIZE 1024   //后面+1最后一个字符用于标志是否读写完成

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

//函数审明
void P(int semid);
void V(int semid);
void sem_ass(int semid ,int val);

int main(int argc, char const *argv[])
{

    //判断参数是否完整
    if(argc != 2)
    {
        printf("输入参数不正确！\n);
        printf("请输入两个参数，前一个参数为被复制的文件名，后一个参数为目标文件名 ！\n");
        return 0;
    }

    //创建共享内存组
    segment_id = shmget(IPC_PRIVATE, MEMORY_SIZE+1, IPC_CREAT|0666);  //+1是为了用最后一个字符标志读写是否完成

    //创建信号灯及赋值
    full = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666); //将共享缓冲区问题转化为生产者消费者问题
    empty = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    mutex = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);

    sem_ass(full, 0);
    sem_ass(empty, MEMORY_SIZE);
    sem_ass(mutex, 1);



    return 0;
}

void ReadBuf()
{

}

void WriteBuf()
{

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
