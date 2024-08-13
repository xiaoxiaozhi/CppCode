#include <sys/socket.h>
#include <stdio.h>
#include <cstdlib>      // exit（）头文件
#include <netinet/in.h> //IPPROTO_TCP 头文件是这个吗？sockaddr_in 头文件，windows下看不到该结构体，Linux下可以
#include <netdb.h>      //IPPROTO_TCP 是这个吗？
#include <unistd.h>
#include <arpa/inet.h> //htons 头文件
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/select.h>
#include <string.h>
#include <iostream>
using namespace std;

/**

 * 4.
 * #include <sys/select.h>
 * int select(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, struct timeval * timeout);
 *
 * timeout：超时时长，用来强制解除select()函数的阻塞的 NULL：函数检测不到就绪的文件描述符会一直阻塞。
 * 等待固定时长（秒）：函数检测不到就绪的文件描述符，在指定时长之后强制解除阻塞，函数返回0
 * 不等待：函数不会阻塞，直接将该参数对应的结构体初始化为0即可。
 *
 * 函数返回值： 大于0：成功，返回集合中已就绪的文件描述符的总个数 ；等于-1：函数调用失败 ；等于0：超时，没有检测到就绪的文件描述符
 *
 * fd_set 类型相关操作函数
 * // 将文件描述符fd从set集合中删除 == 将fd对应的标志位设置为0
 * void FD_CLR(int fd, fd_set *set);
 * // 判断文件描述符fd是否在set集合中 == 读一下fd对应的标志位到底是0还是1
 * int  FD_ISSET(int fd, fd_set *set); 在里面返回非0，不在返回0
 * // 将文件描述符fd添加到set集合中 == 将fd对应的标志位设置为1
 * void FD_SET(int fd, fd_set *set);
 * // 将set集合中, 所有文件文件描述符对应的标志位设置为0, 集合中没有添加任何文件描述符
 * void FD_ZERO(fd_set *set);
 *
 */

int main()
{
    // 1. 创建监听的套接字
    // int lfd = socket(AF_INET, SOCK_STREAM, 0);
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket");
        exit(0);
    }

    // 2. 将socket()返回值和本地的IP端口绑定到一起
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000); // 大端端口
                                  // INADDR_ANY代表本机的所有IP, 假设有三个网卡就有三个IP地址
                                  // 这个宏可以代表任意一个IP地址
                                  // 这个宏一般用于本地的绑定操作
    // addr.sin_addr.s_addr = INADDR_ANY;  // 这个宏的值为0 == 0.0.0.0
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
    int ret = bind(lfd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("bind");
        exit(0);
    }

    // 3. 设置监听
    ret = listen(lfd, 128);
    if (ret == -1)
    {
        perror("listen");
        exit(0);
    }
    // 4.
    fd_set result;
    FD_ZERO(&result);
    FD_SET(lfd, &result);
    int maxFd = lfd;
    while (1)
    {
        cout << "select前" << endl;
        ret = select(lfd + 1, &result, NULL, NULL, NULL);
        cout << "ret = " << ret << " lfd = " << lfd << endl;
        if (ret > 0)
        {

            for (int i = lfd; i <= maxFd; i++)
            {

                if (FD_ISSET(i, &result) != 0)
                {
                    if (i == lfd)
                    {
                        struct sockaddr_in client_addr;
                        int client = sizeof(client_addr);
                        int client_fd = accept(lfd, (struct sockaddr *)&client_addr, &client);
                        FD_SET(client_fd, &result);
                        maxFd = (maxFd, client_fd);
                    }
                    else
                    {
                        char buf[1024];
                        memset(buf, 0, sizeof(buf));
                        int len = 0;
                        while ((len = read(i, buf, sizeof(buf))) > 0)
                        {
                            printf("客户端say: %s\n", buf);
                            sprintf(buf, "你好，客户端");
                            write(i, buf, strlen(buf) + 1);
                        }
                    }
                }
            }
        }
        else if (ret == -1)
        {
            perror("select");
            exit(0);
        }
        else
        {
            cout << "超时没有监听到变化" << endl;
        }
    }
    close(lfd);
    return 0;
}