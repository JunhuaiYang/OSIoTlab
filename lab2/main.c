#include <stdio.h>
#include <stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<linux/sem.h>

int semid;
pthread_t p1,p2;

void *subp1();
void *subp2();

int main()
{
    printf("Hello world!\n");
    return 0;
}


void P(int semid,int index)
{

}

void V(int semid,int index)
{

}

void *subp1()
{

}

void *subp2()
{

}
