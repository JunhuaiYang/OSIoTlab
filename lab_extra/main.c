#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

void PrintDir(char *dir, int depth);
void printf_o(int a);

int main(int argc, char const *argv[])
{
    char dir_name[256];  //linux 为长目录结构 有256目录大小
    if (argc == 1)
    {
        getcwd(dir_name, 256);
        //没有参数则作用在当前目录
        //strcpy(dir_name, name);
    }
    else if (argc == 2)
    {
        //有参数则到参数目录下
        strcpy(dir_name, argv[1]);
    }
    else
    {
        printf("参数输入错误！请输入一个参数或不输入参数！\n");
        printf("程序功能： 若程序无参数则显示当前目录下文件，若输入第二个参数则显示第二个参数目录\n");
        exit(0);
    }

    printf("当前工作目录：%s\n\n", dir_name);
    PrintDir(dir_name, 0);
    printf("Program Over!\n");

    return 0;
}

void PrintDir(char *dir, int depth)
{
    DIR *dp;
    struct dirent *entry;   //目录信息
    struct stat statbuf;    //文件信息
    struct tm mytime;

    //printf("\n当前 %s\n", dir);
    if((dp = opendir(dir)) == NULL)  //打开目录失败
    {
        printf("打开目录失败，请确认输入目录是否正确！\n");
        return;
    }

    chdir(dir);   //切换到当前目下操作 如cd

    while((entry = readdir(dp))!= NULL)  //获取下一级目录
    {
        //忽略最初的 . 和 ..
        if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
            continue;

        lstat(entry->d_name, &statbuf);   //读取文件信息
        //判断是否是目录
        if(S_ISDIR(statbuf.st_mode))
        {
            printf("\n");
            printf_o(depth);
            printf("目录  当前深度 %d   %s\n", depth, entry->d_name);
            PrintDir(entry->d_name, depth+1);  //递归
        }
        else  //文件
        {
            localtime_r(&statbuf.st_mtime, &mytime);  //时间转化，把time_t转化为tm可用于输出
            printf_o(depth);
            printf("文件  当前深度 %d %30s %10.2lf(kb)",depth, entry->d_name, statbuf.st_size/1024.0);
            printf("   %d年%2d月%2d日  %d:%d:%d\n", mytime.tm_year + 1900, mytime.tm_mon+1, mytime.tm_mday,mytime.tm_hour, mytime.tm_min, mytime.tm_sec);
        }
    }
    printf("\n");  //格式化
    chdir("..");  //回到上一级目录
    closedir(dp);   //关闭当前目录

}

void printf_o(int a)
{
    int i;
    for(i=0; i<a; i++)
        printf("  ");
}