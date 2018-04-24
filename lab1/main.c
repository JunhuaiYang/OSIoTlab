#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>

//函数原型
void Child_1(void);
void Child_2(void);
void My_SIGUSR1(int sig);
void My_SIGUSR2(int sig);
void My_SIGINT(int sig);

pid_t p1, p2;

int pipefd[2]; //管道


int main(int argc, char const *argv[])
{

    if(pipe(pipefd) < 0)  //创建无名管道
    {
        printf("Create Pipe failed! \n");
        return -1;
    }

    if((p1=fork()) == -1)  
    {
        printf("Create Child Process 1 failed! \n");
        return -1;
    }
    else if(p1 > 0) //此时要判断是否在父进程中，否则会多产生一个子进程
    {
        if((p2=fork()) == -1)  //创建子进程2
        {
            printf("Create Child Process 2 failed! \n");
            return -1;
        }
    }

    signal(SIGINT, My_SIGINT); //设置信号SIGINT

    if(p1 == 0)  //子进程1 中
    {
        Child_1();
    }
    else if(p2 == 0)  // 子进程2中
    {
        Child_2();
    }

    wait(NULL);
    printf("   Parent Process is Killed! \n");
    exit(0);

    return 0;
}

void Child_1(void)
{
    int count = 1;
    char temp[50];

    signal(SIGINT, SIG_IGN);  //子进程忽略SIGINT
    signal(SIGUSR1, My_SIGUSR1);

    while(1)
    {
        printf("Here is Process 1\n");
        //字符串操作
        sprintf(temp, "I send you %d times !", count++); //格式化写入字符串
        write(pipefd[1], temp, strlen(temp)+1);

        sleep(1);  //睡眠一秒
    }

}

void Child_2(void)
{
    char cache[50]; //接受管道通讯的数据
    memset(cache,0, sizeof(cache));     //在一段内存块中填充某个特定的值，此为，对cache数组以0进行填充

    signal(SIGINT, SIG_IGN);  //子进程忽略SIGINT
    signal(SIGUSR2, My_SIGUSR2);

    while(1)
    {
        printf("Here is Process 2\n");
        read(pipefd[0],cache, 50);  //读

        printf("%s\n", cache);
    }
}


void My_SIGUSR1(int sig)
{
    if(sig == SIGUSR1)  //收到来自SIGUSR1的信号
    {
        close(pipefd[1]);                          //     关闭写管道
        close(pipefd[0]);                          //     关闭读管道

        printf("Now Child Process 1 has stoped! \n");

        exit(0);   //退出当前进程
    }
}

void My_SIGUSR2(int sig)
{
    if(sig == SIGUSR2)  //收到来自SIGUSR1的信号
    {
        close(pipefd[1]);                          //     关闭写管道
        close(pipefd[0]);                          //     关闭读管道

        printf("Now Child Process 2 has stoped! \n");

        exit(0);   //退出当前进程

    }
}

void My_SIGINT(int sig)
{
    if(sig == SIGINT)  //收到SIGINT
    {
        //给两个子进程发送结束的命令
        kill(p1, SIGUSR1);
        kill(p2, SIGUSR2);
    }
}

