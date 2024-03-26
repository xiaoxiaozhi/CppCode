#include "linux_head.h"
// io 需要引入的头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
/**
 * man 命令章节
 * 1 用户命令或可执行程序的文档
 * 2 系统调用（内核函数）的文档
 * 3 C库函数的文档
 * 4 特殊文件（通常是/dev下的文件）和设备驱动程序的文档
 * 5 文件格式和约定的文档
 * 6 游戏的文档
 * 7 杂项文档
 * 8 系统管理员命令的文档
 * 9 内核开发的文档
 * linux io
 * 1.open函数
 * 在线man手册 https://man.cx/open(2)
 * Open()打开由路径名指定的文件。如果指定的文件不存在，则可以通过 open()选择创建它(如果在标志中指定了 O_CREAT)
 * arg1:文件路径
 * arg2:必须包含这几个中的一个，O_RDONLY, O_WRONLY, or O_RDWR.其他flag可以通过 | 添加(标志很多，看手册)
 * arg3:创建文件时 赋予文件的权限
 * int open(const char *pathname, int flags, mode_t mode);
 *
 */
int main()
{
    int descriptor = open("./open_fun.txt", O_CREAT|O_WRONLY);
    if (descriptor != -1)
    {
        cout << "创建文件描述符---" << descriptor << endl;
    }
    else
    {
        perror("文件描述符失败---");
    }
    close(descriptor);//

    return 1;
}