#include <unistd.h> // pipe()
#include <fcntl.h>
#include <string.h>
#include <stdio.h> //perror()
#include <sys/types.h>
#include <stdlib.h>//exit()
using namespace std;
/**
 * 道的本质其实就是内核中的一块内存(或者叫内核缓冲区)，这块缓冲区中的数据存储在一个环形队列中
 * 进程间通信方式：
 *   管道 使用最简单，必须有血缘关系才能使用  大丙老师说只要获取管道读写描述符任何进程都能操作，真的吗？
 *   信号 开销最小
 *   共享映射区 无血缘关系
 *   本地套接字 最稳定
 * 管道特点：
 *    管道对应的内核缓冲区大小是固定的，默认为4k（也就是队列最大能存储4k数据）
 *    管道分为两部分：读端和写端（队列的两端），数据从写端进入管道，从读端流出管道。
 *    管道中的数据只能读一次，做一次读操作之后数据也就没有了（读数据相当于出队列）。
 *    管道是单工的：数据只能单向流动, 数据从写端流向读端。
 *    对管道的操作（读、写）默认是阻塞的
 *    读管道：管道中没有数据，读操作被阻塞，当管道中有数据之后阻塞才能解除
 *    写管道：管道被写满了，写数据的操作被阻塞，当管道变为不满的状态，写阻塞解除
 * 1. 创建匿名管道
 *    pipe() 创建并打开管道成功返回0 失败返回-1
 *    pipefd[0]: 对应管道读端的文件描述符，通过它可以将数据从管道中读出
 *    pipefd[1]: 对应管道写端的文件描述符，通过它可以将数据写入到管道中
 */
void readPip(int fd);
void writePip(int fd);
int main()
{

    int pipfd[2];
    int n = pipe(pipfd);
    if (n < 0)
    {
        perror("管道创建失败");
        exit(1);
    }
    pid_t child = fork();
    if (child == 0)
    {
        //父进程

    }
    else if (child > 0)
    {
        //子进程
    }else{
        perror("创建进程失败");
        exit(1);
    }

    return 1;
}
void readPip(int fd){

}
void writePip(int fd){

}