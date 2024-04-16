#include <unistd.h> // pipe()
#include <fcntl.h>
#include <string.h>
#include <stdio.h> //perror()
#include <sys/types.h>
#include <stdlib.h>   //exit()
#include <sys/wait.h> //wait()
#include <sys/stat.h> //mkfifo()
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
 * 2. 创建有名管道
 *    不管有没有血缘关系，都可以使用有名管道实现进程间通信 与匿名管道通信方式是一样的
 *    有名管道文件大小永远为0，因为有名管道也是将数据存储到内存的缓冲区中，打开这个磁盘上的管道文件就可以得到操作有名管道的文件描述符，通过文件描述符读写管道存储在内核中的数据。
 *    int ret = mkfifo("./testfifo", 0777); 创建有名管道, 如果已经存在会报错 mkfifo: File exists，但不妨碍读写
 *    读写两个进程，如果只是读端打开了或者只是写端打开了，进程会阻塞在这里不会向下执行，直到在另一个进程中将管道的对端打开，
 *    当前进程的阻塞也就解除了。读端打开文件之后开始 read 如果管道没有数据就会阻塞， 写端写满了才会阻塞。
 *    有名管道大小 受文件系统的最大文件大小限制以及文件系统的可用空间限制
 *
 *
 * 下面例子 子进程写管道 父进程堵管道并显示,还真的注意的读进程要把写端关闭，写进程要把读端关闭，否则会出问题
 */
void readPip(int &fdr);
void writePip(int &fdw);
int main()
{
    // 1. 匿名管道
    //  int pipfd[2];
    //  int n = pipe(pipfd);
    //  if (n < 0)
    //  {
    //      perror("管道创建失败");
    //      exit(1);
    //  }
    //  pid_t child = fork();
    //  if (child == 0)
    //  {
    //      // 父进程
    //      close(pipfd[1]);
    //      readPip(pipfd[0]);
    //  }
    //  else if (child > 0)
    //  {
    //      // 子进程
    //      close(pipfd[0]);
    //      writePip(pipfd[1]);
    //  }
    //  else
    //  {
    //      perror("创建进程失败");
    //      exit(1);
    //  }
    // 2.有名管道------------------------------------------------------------------
    // execlp("rm","rm","-f","testfifo",NULL);
    // perror("execlp---");
    int ret = mkfifo("./testfifo", 0777);
    if (ret == -1)
    {
        perror("mkfifo");
    }
    printf("管道文件创建成功...\n");
    int forkId = fork();
    if (forkId == 0)
    {
        // 父进程
        int rfd = open("./testfifo", O_RDONLY);
        if (rfd == -1)
        {
            perror("open");
            exit(0);
        }
        printf("以只读的方式打开文件成功...\n");

        while (1)
        {
            char buf[1024];
            memset(buf, 0, sizeof(buf));
            // 读是阻塞的, 如果管道中没有数据, read自动阻塞，有数据解除阻塞, 继续读数据
            int len = read(rfd, buf, sizeof(buf));
            printf("读出的数据: %s\n", buf);
            if (len == 0)
            {
                // 写端关闭了, read解除阻塞返回0
                printf("管道的写端已经关闭, 拜拜...\n");
                break;
            }
        }
        close(rfd);
    }
    else if (forkId > 0)
    {
        // 子进程
        // 因为要写管道, 所有打开方式, 应该指定为 O_WRONLY
        // 如果先打开写端, 读端还没有打开, open函数会阻塞, 当读端也打开之后, open解除阻塞
        int wfd = open("./testfifo", O_WRONLY);
        if (wfd == -1)
        {
            perror("open");
            exit(0);
        }
        printf("以只写的方式打开文件成功...\n");
        int i = 0;
        while (i < 100)
        {
            char buf[1024];
            sprintf(buf, "hello, fifo, 我在写管道...%d\n", i);
            write(wfd, buf, strlen(buf));
            i++;
            sleep(1);
        }
        close(wfd);
    }
    else
    {
        perror("创建进程失败");
        exit(1);
    }
    return 1;
}
void readPip(int &fdr)
{

    char buf[4096];
    int n = 0;
    // 读管道
    // 如果管道中没有数据, read会阻塞 有数据之后, read解除阻塞, 直接读数据 需要循环读数据, 管道是有容量的, 写满之后就不写了  数据被读走之后, 继续写管道, 那么就需要再继续读数据
    while ((n = read(fdr, buf, sizeof(buf))) > 0)
    {
        printf("%s, len = %d\n", buf, n);
    }
    if (n < 0)
    {
        perror("读管道---");
    }
    close(fdr);
    wait(NULL);
    exit(1);
}
void writePip(int &fdw)
{
    dup2(fdw, STDOUT_FILENO);
    execlp("ps", "ps", "aux", NULL);
    perror("execlp");
}