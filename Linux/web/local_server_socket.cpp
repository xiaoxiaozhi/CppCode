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
#include <sys/un.h>
#include <stddef.h>

/**
 * 本地套接字大丙老师没有，以黑马视频为主
 *
 *
 * socket通信
 * domain:协议族 AF_UNIX或者AF_LOCAL进程间通信、 AF_INET ip4 、AF_INET6 ip6    https://man.cx/address_families(7)
 * type:数据传输类型 ，除了以下两个其他基本不用
 *  SOCK_STREAM 面向连接的socket数据不会丢失、顺序不会乱、双向通信
 *  SOCK_DGRAM 无连接的socket 数据可能会丢失、顺序可能会乱、传输效率更高 (就是基于UDP的socket)
 *  对于本地套接字 两者都可以
 * protocol：最终使用的协议SOCK_STREAM 对应IPPROTO_TCP  TCP协议 、SOCK_DGRAM 对应使用IPPROTO_UDP UDP协议. 也可以填0
 *
 * 1. 创建socket
 * int socket(int domain, int type, int protocol);成功: 可用于套接字通信的文件描述符 失败: -1
 * 2. 绑定
 *    与网络套接字不同，本地套接字的地址结构有变，sockaddr_in ---> sockaddr_un
 * 将得到的监听的文件描述符和本地的IP 端口进行绑定
 * addrlen 计算第二个参数的长度 sizeof(addr)即可
 * int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen); 成功返回0，失败返回-1
 * 与网络端不一样低地方在于，structure addrsocket_un 因为文件名的关系实际内容是可变长的 这时候就不能用sizeof(addr)，
 * 用宏函数 offsetof()计算协议长度 ，实际就是2 固定的，再套个函数有什么必要呢？
 * bind函数调用成功会创建一个socke文件
 * 再bind前调用unlink(函数只会删除文件的链接,而不会删除文件内容，相当与删除文件索引吗？) 成功返回0
 * 3.监听
 * int listen(int sockfd, int backlog);函数调用成功返回0，调用失败返回 -1
 * backlog 每轮最多监听多少个客户端，内核中写死最多128个，监听结束还有下一轮
 *
 * struct sockaddr_un {
 *  sa_family_t sun_family;     // AF_UNIX
 *  char        sun_path[108];  // socket 文件名， bind后会自动生成，(如果 是已经存在的文件会怎么样呢？
 * }
 *
 */

int main()
{
    // 1. 创建监听的套接字
    // int lfd = socket(AF_INET, SOCK_STREAM, 0);
    int lfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket");
        exit(0);
    }

    // 2. 将socket()返回值和本地的IP端口绑定到一起
    char socket_name[] = "local_socket";
    struct sockaddr_un addr, cliaddr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, socket_name);
    int len = offsetof(struct sockaddr_un, sun_path) + strlen(socket_name);
    int ret = unlink(socket_name); // bind会产生一个socket文件，再bind之前确保文件不存在，调用unlink删除文件。如果不删除，bind就会失败
    if (ret != 0)
        printf("unlink 调用失败");
    ret = bind(lfd, (struct sockaddr *)&addr, len);
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
    while (1)
    {
        int clilen = sizeof(cliaddr);
        int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &clilen);
        clilen -= offsetof(struct sockaddr_un,sun_path);
        if (cfd == -1)
        {
            perror("accept");
            exit(0);
        }
        cliaddr.sun_path[clilen]='\0';
        printf("客户端套接字名称 %s\n",cliaddr.sun_path)
    }
    

    // // 4. 阻塞等待并接受客户端连接
    // struct sockaddr_in cliaddr;
    // int clilen = sizeof(cliaddr);
    // int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &clilen);
    // if (cfd == -1)
    // {
    //     perror("accept");
    //     exit(0);
    // }
    // // 打印客户端的地址信息
    // char ip[24] = {0};
    // printf("客户端的IP地址: %s, 端口: %d\n",
    //        inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)),
    //        ntohs(cliaddr.sin_port));

    // close(cfd);
    // close(lfd);

    return 0;
}