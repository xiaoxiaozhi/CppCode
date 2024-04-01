#include "linux_head.h"
// io 需要引入的头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <fcntl-linux.h>
#include <errno.h>  //errno
#include <string.h> //strerror
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
 * linux 文件操作
 * 1.open 打开文件
 * 在线man手册 https://man.cx/open(2)
 * Open()打开由路径名指定的文件。如果指定的文件不存在，则可以通过 open()选择创建它(如果在标志中指定了 O_CREAT)
 * arg1:文件路径
 * arg2:必须包含这几个中的一个，O_RDONLY, O_WRONLY, or O_RDWR.其他flag可以通过 | 添加(标志很多，看手册)
 * arg3:创建文件时 赋予文件的权限
 * int open(const char *pathname, int flags, mode_t mode);//创建文件时必须制定mode_t，如果不指定，会有什么后果呢？mode_t种类繁多具体看manpages
 * 2. 读写
 * arg1:文件描述符
 * arg2：缓冲区
 * arg2：读写的字节
 * ssize_t read(int fd, void *buf, size_t count);正常状态读取几个字节返回字节数，读到结尾返回0 ，读错返回-1 
 * ssize_t write(int fd, const void *buf, size_t count);
 * 读写系统调用速度比不上c库函数读写文件，因为系统调用没有用缓冲区，是一个字节一个字节读写。每次读写都要从用户空间到内核空间切换，状态切换是个耗时操作
 * 系统调用就是帮助用户调用内核空间才有的功能
 * 这俩函数差不多，返回一个有符号整形，失败返回-1 并且设置errno
 * 写完要close关闭文件，再次open文件才能读文件
 *
 *
 * 3. 关闭文件
 *
 * open("", O_CREAT | O_WRONLY , S_IRWXU) 存在就覆盖文件 不存在就创建
 * open("", O_CREAT | O_WRONLY | O_EXCL, S_IRWXU);  存在报错-1，不存在创建
 * open("", O_WRONLY | O_CREAT | O_APPEND, S_IRWXU); 存在就追加，不存在就创建
 * O_EXCL:用于在打开文件时指定文件存在时的处理方式。如果指定的文件已经存在，则open函数会失败，并返回错误-1。文件存在
 * O_TRUNC：把文件清零  open("", O_CREAT | O_WRONLY | O_APPEND, S_IRWXU); 如果文件存在就清零，不存在就创建
 *
 * 文件权限受umask影响，(mode & ~umask).  umask 是一条命令输入后显示一个8进制的值，这条看不明白
 * open函数 失败会自动设置erron操作系统的全局变量<erron.h> 想知道erron具体什么意思用strerror(errno) 头文件string.h
 * trerror(errno)得到的错误与perror一致
 * open函数还能打开目录
 * printf("read了%d个字符--%.*s", n, n, buf_read); 按长度打印数组长度
 *
 * strace ./可执行文件，可以打印程序执行过程中的系统调用
 * PCB进程控制块 中保存着文件描述符表，表中的每个文件描述符是一个指针，open函数返回的fd就是表的下标。一个进程最多打开1024个文件
 * 老师说前三个文件描述符是 STDIN_FILENO STDOUT_FILENO STDERR_FILENO  这几个东西是什么呢？？？确实是这样，我自己打开open后得到值是3，验证了这个说法
 * 4. 阻塞与非阻塞
 *    读设备或者读网络会产生阻塞，读常规文件不会产生阻塞。(常规文件时存储卡里的文件吗？)设备文件 dev/tty  键盘 外设之类的
 *    read(STDIN_FILENO, keyRead, sizeof(keyRead)); 会一直等待键盘输入 产生阻塞
 *    阻塞是设备文件和网络问价的属性，以非阻塞方式打开文件 open("dev/tty",O_WRONLY|O_NONBLOCK)，如果没有数据返回-1并且erron赋值 EWOULDBLOCK
 *    TODO O_NONBLOCK 04000 在minggw64下面没有，在ubuntu里面有。 
 *    
 * 
 * 在C++中，如果使用printf函数输出内容时不加\n，输出的内容可能不会立即打印出来，而是会被缓冲起来，直到缓冲区满了或者遇到换行符才会刷新输出。
 *   printf("Hello, ");
 *   fflush(stdout); // 强制刷新，输出缓冲
 *   printf("world!");
 *5. fcntl()改变文件阻塞属性
 *   不用重新打开文件就可以改变
 */
int main()
{
    // 1. 打开文件
    int descriptor = open("./open_fun.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    if (descriptor < 0)
    {
        // perror("创建文件描失败---%d");
        printf("打开或创建open_fun.txt失败 erron---%d strerror---%s", errno, strerror(errno));
        exit(1);
    }

    cout << "创建文件成功---" << descriptor << endl;
    // 2. 读写
    const char *buf = "hello word";
    printf("写buf长度%d\n", strlen(buf));
    write(descriptor, buf, strlen(buf));
    close(descriptor);

    int df = open("./open_fun.txt", O_RDONLY);
    if (df < 0)
    {
        perror("打开文件open_fun.txt失败---");
        exit(1);
    }
    char buf_read[1024];
    int n = 0;
    while ((n = read(df, buf_read, sizeof(buf_read))) > 0)
    {
        printf("read了%d个字符--%.*s\n", n, n, buf_read);
    }

    printf("读取结束 erron---%d strerror---%s\n", errno, strerror(errno));
    close(df);
   

    // 4. 读外设，阻塞
    char keyRead[1024];
    int keybord = read(STDIN_FILENO, keyRead, sizeof(keyRead));
    if (keybord > 0)
    {
        printf("键盘输入了%d字符---%.*s\n", keybord, keybord, keyRead);
    }
    else
    {
        perror("读键盘err---");
    }
    //5.改变文件阻塞属性
    

    return 1;
}