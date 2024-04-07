#include <sys/socket.h>
#include <stdio.h>
#include <cstdlib>// exit（）头文件
#include <netinet/in.h>//IPPROTO_TCP 头文件是这个吗？
#include <netdb.h>//IPPROTO_TCP 是这个吗？
#include <unistd.h>
#include <arpa/inet.h>//htons 头文件
#include <sys/types.h>
#include <sys/socket.h>

/**
 * socket通信
 * domain:协议族 AF_UNIX或者AF_LOCAL进程间通信、 AF_INET ip4 、AF_INET6 ip6    https://man.cx/address_families(7)
 * type:数据传输类型 ，除了以下两个其他基本不用
 *  SOCK_STREAM 面向连接的socket数据不会丢失、顺序不会乱、双向通信
 *  SOCK_DGRAM 无连接的socket 数据可能会丢失、顺序可能会乱、传输效率更高 (就是基于UDP的socket)
 * protocol：最终使用的协议SOCK_STREAM 对应IPPROTO_TCP  TCP协议 、SOCK_DGRAM 对应使用IPPROTO_UDP UDP协议. 也可以填0
 * int socket(int domain, int type, int protocol);
 * 1. 创建socket
 * 2. 绑定
 * 
 *
 */
/**
 * 存放协议族、端口和地址信息，客户端和connect()函数和服务端的bind()函数需要这个结构体
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
*/

int main()
{
    printf("AF_UNIX---%d\n", AF_UNIX);
    printf("AF_INET---%d\n", AF_INET);
    //1. 创建socket
    int socket_fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(socket_fd<0){
        perror("创建socket---");
        exit(1);
    }
    printf("创建socket成功---%d",socket_fd);
    //2. 向服务器发起请求
    struct sockaddr_in servaddr;               // 用于存放协议、端口和IP地址的结构体。
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;             // ①协议族，固定填AF_INET。
    servaddr.sin_port = htons(8080);  // ②指定服务端的通信端口。
    close(socket_fd);
    return 1;
}