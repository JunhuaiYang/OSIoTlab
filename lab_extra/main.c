#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    char dir_name[100];
    if (argc == 1)
    {
        //没有参数则作用在当前目录
        strcpy(dir_name, get_current_dir_name());
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

    return 0;
}
