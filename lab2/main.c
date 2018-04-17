#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/sem.h>
//#include<linux/sem.h>

#define COUNT 100

//由于sys/sem.h 与 sys/sem.h有冲突，所以单独定义
/* arg for semctl system calls. */
union semun {
	int val;			/* value for SETVAL */
	struct semid_ds *buf;	/* buffer for IPC_STAT & IPC_SET */
	unsigned short *array;	/* array for GETALL & SETALL */
	struct seminfo *__buf;	/* buffer for IPC_INFO */
	void *__pad;
};

//公共变量
int a;
//全局变量
int semid_a, semid_b;  //需要申请两个信号灯

//函数审明
void *subp1();
void *subp2();
void P(int semid);
void V(int semid);
void sem_ass(int semid ,int val);

int main()
{
    pthread_t p1,p2;  //两个线程

    semid_a = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);   //信号灯a初值为0，表示公共变量中无信息
    semid_b = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);   //信号灯b初值为1，表示公共变量是可以更新

    //a信号量赋值
    sem_ass(semid_a, 0);
    //b信号量赋值
    sem_ass(semid_b, 1);

    //创建线程1
    if(pthread_create(&p1, NULL, subp1, NULL) == -1)
    {
        printf("Create Pthread 1 Error! \n");
        return -1;
    }
    //创建线程2
    if(pthread_create(&p2, NULL, subp2, NULL) == -1)
    {
        printf("Create Pthread 2 Error! \n");
        return -1;
    }

    //等待
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("pthread has all finished! \n");

    //删除信号灯
    semctl(semid_a, 0, IPC_RMID);
    semctl(semid_b, 0, IPC_RMID);

    return 0;
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

//线程1 函数，线程1相当于写者
void *subp1()
{
    int i;
    for(i=1; i<COUNT+1; i++)
    {
        printf("Now in pthread 1 ! \n");
        P(semid_b);  //P(b)
        a += i;
        //printf("i %d a %d \n", i,a);
        V(semid_a);  //V(a)
    }

    pthread_exit(NULL);
}
 //线程2 相当于读着
void *subp2()
{
    int i;
    for(i=0; i<COUNT; i++)
    {
        printf("Now in pthread 2 ! \n");
        P(semid_a);  //P(a)
        printf("count = %d  |   a = %d\n", i+1, a);
        V(semid_b);  //V(b)
    }

    pthread_exit(NULL);
}

//信号灯赋值
void sem_ass(int semid ,int val)
{
    union semun arg;
    arg.val = val;
    semctl(semid, 0, SETVAL, arg);
}
