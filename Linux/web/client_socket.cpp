#include <sys/socket.h>
#include <stdio.h>
#include <cstdlib>      // exit（）头文件
#include <netinet/in.h> //IPPROTO_TCP 头文件是这个吗？sockaddr_in 头文件，windows下看不到该结构体，Linux下可以
#include <netdb.h>      //IPPROTO_TCP 是这个吗？
#include <unistd.h>
#include <arpa/inet.h> //htons()、inet_pton()头文件 
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

/**
 * socket通信
 * domain:协议族 AF_UNIX或者AF_LOCAL进程间通信、 AF_INET ip4 、AF_INET6 ip6    https://man.cx/address_families(7)
 * type:数据传输类型 ，除了以下两个其他基本不用
 *  SOCK_STREAM 面向连接的socket数据不会丢失、顺序不会乱、双向通信
 *  SOCK_DGRAM 无连接的socket 数据可能会丢失、顺序可能会乱、传输效率更高 (就是基于UDP的socket)
 * protocol：最终使用的协议SOCK_STREAM 对应IPPROTO_TCP  TCP协议 、SOCK_DGRAM 对应使用IPPROTO_UDP UDP协议. 也可以填0
 * int socket(int domain, int type, int protocol);
 * 1. 创建socket
 * 2. 链接 connect
 *    成功连接服务器之后, 客户端会自动随机绑定一个端口 第二个参数存储的就是客户端的IP和端口信息
 *    int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
 */

int main()
{
      // 1. 创建通信的套接字
    // int fd = socket(AF_INET, SOCK_STREAM, 0);
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(0);
    }

    // 2. 连接服务器
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);   // 大端端口
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);// 127开头好像是本地地址

    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1)
    {
        perror("connect");
        exit(0);
    }

    // 3. 和服务器端通信
    int number = 0;
    while(1)
    {
        // 发送数据
        char buf[1024];
        sprintf(buf, "你好, 服务器...%d\n", number++);
        write(fd, buf, strlen(buf)+1);
        
        // 接收数据
        memset(buf, 0, sizeof(buf));
        int len = read(fd, buf, sizeof(buf));
        if(len > 0)
        {
            printf("服务器say: %s\n", buf);
        }
        else if(len  == 0)
        {
            printf("服务器断开了连接...\n");
            break;
        }
        else
        {
            perror("read");
            break;
        }
        sleep(1);   // 每隔1s发送一条数据
    }

    close(fd);

    return 0;
}