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
#include <sys/epoll.h>
#include <sys/epoll.h>
using namespace std;

/**
 *
 * // 创建epoll实例，通过一棵红黑树管理待检测集合
 * int epoll_create(int size);
 * size：在Linux内核2.6.8版本以后，这个参数是被忽略的，只需要指定一个大于0的数值就可以了。
 * 返回：失败返回-1 成功返回一个有效的文件描述符，通过这个文件描述符就可以访问创建的epoll实例了
 * // 管理红黑树上的文件描述符(添加、修改、删除)
 * int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
 * epfd：红黑树的根节点
 * op：这是一个枚举值，控制通过该函数执行什么操作
 *     EPOLL_CTL_ADD：往epoll模型中添加新的节点
 *     EPOLL_CTL_MOD：修改epoll模型中已经存在的节点
 *     EPOLL_CTL_DEL：删除epoll模型中的指定的节点
 *
 * events：委托epoll检测的事件
 *         EPOLLIN：读事件, 接收数据, 检测读缓冲区，如果有数据该文件描述符就绪
 *         EPOLLOUT：写事件, 发送数据, 检测写缓冲区，如果可写该文件描述符就绪
 *         EPOLLERR：异常事件
 *         data：用户数据变量，这是一个联合体类型，通常情况下使用里边的fd成员，用于存储待检测的文件描述符的值，与第三个参数相同
 * 失败返回-1 成功返回0
 *
 * // 检测epoll树中是否有就绪的文件描述符
 * int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
 * events：传出参数, 这是一个结构体数组的地址, 里边存储了已就绪的文件描述符的信息
 * timeout：如果检测的epoll实例中没有已就绪的文件描述符，该函数阻塞的时长, 单位ms 毫秒
 *          0：函数不阻塞，不管epoll实例中有没有就绪的文件描述符，函数被调用后都直接返回
 *          大于0：如果epoll实例中没有已就绪的文件描述符，函数阻塞对应的毫秒数再返回
 *          -1：函数一直阻塞，直到epoll实例中有已就绪的文件描述符之后才解除阻塞
 * 失败返回-1 成功 返回0代表函数是阻塞被强制解除了, 没有检测到满足条件的文件描述符 返回>0 检测到的已就绪的文件描述符的总个数
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
    //4. 创建对象epoll根节点
    int efd = epoll_create(1024);
    struct epoll_event event;
    event.evens = EPOLLIN;
    event.data = lfd;
    epoll_ctl(efd,EPOLL_CTL_ADD,lfd,&event);

    close(lfd);
    return 0;
}