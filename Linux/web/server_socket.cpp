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

/**
 * 网络字节序，主机字节序
 * 
 * 
 * socket通信
 * domain:协议族 AF_UNIX或者AF_LOCAL进程间通信、 AF_INET ip4 、AF_INET6 ip6    https://man.cx/address_families(7)
 * type:数据传输类型 ，除了以下两个其他基本不用
 *  SOCK_STREAM 面向连接的socket数据不会丢失、顺序不会乱、双向通信
 *  SOCK_DGRAM 无连接的socket 数据可能会丢失、顺序可能会乱、传输效率更高 (就是基于UDP的socket)
 * protocol：最终使用的协议SOCK_STREAM 对应IPPROTO_TCP  TCP协议 、SOCK_DGRAM 对应使用IPPROTO_UDP UDP协议. 也可以填0
 * 
 * 1. 创建socket
 * int socket(int domain, int type, int protocol);成功: 可用于套接字通信的文件描述符 失败: -1
 * 2. 绑定
 * 将得到的监听的文件描述符和本地的IP 端口进行绑定
 * addrlen 计算第二个参数的长度 sizeof(addr)即可
 * int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen); 成功返回0，失败返回-1
 * 3.监听
 * int listen(int sockfd, int backlog);函数调用成功返回0，调用失败返回 -1
 * backlog 每轮最多监听多少个客户端，内核中写死最多128个，监听结束还有下一轮
 * 4.获取监听到的客户端信息
 * int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);*addr传一个空的结构体指针，
 * 存放协议族、端口和地址信息，客户端和connect()函数和服务端的bind()函数需要这个结构体
 * 函数调用成功，得到一个文件描述符, 用于和建立连接的这个客户端通信，调用失败返回 -1
 * 
 * struct sockaddr {
 * unsigned short sa_family;	// 协议族，与socket()函数的第一个参数相同，填AF_INET。
 * unsigned char sa_data[14];	// 14字节的端口和地址。
 * };
 * sockaddr结构体是为了统一地址结构的表示方法，统一接口函数，但是，操作不方便，所以定义了等价的sockaddr_in结构体，它的大小与sockaddr相同，可以强制转换成sockaddr
 * struct sockaddr_in {
 * unsigned short sin_family;	// 协议族，与socket()函数的第一个参数相同，填AF_INET。
 * unsigned short sin_port;		// 16位端口号，大端序。用htons(整数的端口)转换。
 * struct in_addr sin_addr;		// IP地址的结构体。192.168.101.138
 * unsigned char sin_zero[8];	// 未使用，为了保持与struct sockaddr一样的长度而添加。
 * };
 * struct in_addr {				// IP地址的结构体。
 * unsigned int s_addr;		// 32位的IP地址，大端序。
 * };
 * gethostbyname函数
 * 根据域名/主机名/字符串IP获取大端序IP，用于网络通讯的客户端程序中。
 * struct hostent *gethostbyname(const char *name);
 * struct hostent {
 * char *h_name;     	// 主机名。
 * char **h_aliases;    	// 主机所有别名构成的字符串数组，同一IP可绑定多个域名。
 * short h_addrtype; 	// 主机IP地址的类型，例如IPV4（AF_INET）还是IPV6。
 * short h_length;     	// 主机IP地址长度，IPV4地址为4，IPV6地址则为16。
 * char **h_addr_list; 	// 主机的ip地址，以网络字节序存储。
 * };
 *#define h_addr h_addr_list[0] 	// for backward compatibility.
 *转换后，用以下代码把大端序的地址复制到sockaddr_in结构体的sin_addr成员中。
 *memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);
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

    // 4. 阻塞等待并接受客户端连接
    struct sockaddr_in cliaddr;
    int clilen = sizeof(cliaddr);
    int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &clilen);
    if (cfd == -1)
    {
        perror("accept");
        exit(0);
    }
    // 打印客户端的地址信息
    char ip[24] = {0};
    printf("客户端的IP地址: %s, 端口: %d\n",
           inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, sizeof(ip)),
           ntohs(cliaddr.sin_port));

    // 5. 和客户端通信
    while (1)
    {
        // 接收数据
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        int len = read(cfd, buf, sizeof(buf));
        if (len > 0)
        {
            printf("客户端say: %s\n", buf);
            write(cfd, buf, len);
        }
        else if (len == 0)
        {
            printf("客户端断开了连接...\n");
            break;
        }
        else
        {
            perror("read");
            break;
        }
    }

    close(cfd);
    close(lfd);

    return 0;
}