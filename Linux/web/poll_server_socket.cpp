#include <sys/socket.h>
#include <stdio.h>
#include <cstdlib>      // exit()
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>
#include <arpa/inet.h>  // htons
#include <string.h>
#include <iostream>
#include <poll.h>       // poll 头文件
#include <errno.h>
using namespace std;

/**
 * poll() 函数原型：
 * 
 * int poll(struct pollfd *fds, nfds_t nfds, int timeout);
 * 
 * struct pollfd {
 *     int   fd;         // 要监控的文件描述符
 *     short events;     // 要监控的事件（输入参数）
 *     short revents;    // 实际发生的事件（输出参数，由内核填充）
 * };
 *
 * 常用事件：
 *   POLLIN    — 有数据可读
 *   POLLOUT   — 可以写入数据
 *   POLLERR   — 发生错误
 *   POLLHUP   — 对方关闭连接
 *
 * timeout：
 *   -1  — 阻塞等待
 *   0   — 立即返回（非阻塞）
 *   >0  — 等待的毫秒数
 *
 * 返回：
 *   >0  — 就绪的文件描述符数量
 *   0   — 超时
 *   -1  — 出错
 */

#define MAX_EVENTS 1024  // 最大监控数量

int main()
{
    // 1. 创建监听套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2. 设置端口复用（方便调试，避免 "Address already in use"）
    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 3. 绑定 IP 和端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10001);      // 用10001端口，避免和epoll冲突
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    int ret = bind(lfd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // 4. 设置监听
    ret = listen(lfd, 128);
    if (ret == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    cout << "========== poll 服务器启动 ==========" << endl;
    cout << "监听地址: 127.0.0.1:10001" << endl;
    cout << "======================================" << endl;

    // 5. 创建 pollfd 数组（相当于 select 的 fd_set）
    struct pollfd fds[MAX_EVENTS];

    // 初始化所有 fd 为 -1（表示未使用）
    for (int i = 0; i < MAX_EVENTS; i++)
    {
        fds[i].fd = -1;
    }

    // 将监听套接字加入 poll 监控
    fds[0].fd = lfd;
    fds[0].events = POLLIN;   // 监控读事件
    fds[0].revents = 0;

    int max_index = 0;  // 当前已使用的最大索引

    // 6. 事件循环（核心）
    while (1)
    {
        cout << "\n等待事件就绪..." << endl;

        // poll() 阻塞等待事件就绪
        // 参数：fds 数组, 数组大小（从0到max_index之间，fd!=-1的数量）, 超时时间-1（永远阻塞）
        int nready = poll(fds, max_index + 1, -1);

        if (nready == -1)
        {
            perror("poll");
            break;
        }
        else if (nready == 0)
        {
            cout << "poll 超时" << endl;
            continue;
        }

        // 7. 遍历 pollfd 数组，检查哪些描述符就绪
        for (int i = 0; i <= max_index && nready > 0; i++)
        {
            if (fds[i].fd == -1)
                continue;

            if (fds[i].revents & POLLIN)  // 有读事件就绪
            {
                if (fds[i].fd == lfd)
                {
                    // ===== 情况1：有新客户端连接 =====
                    struct sockaddr_in cliaddr;
                    socklen_t clilen = sizeof(cliaddr);
                    int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &clilen);

                    if (cfd == -1)
                    {
                        perror("accept");
                        continue;
                    }

                    char client_ip[16];
                    inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, client_ip, sizeof(client_ip));
                    cout << "新客户端连接: " << client_ip << ":" << ntohs(cliaddr.sin_port) << endl;

                    // 将新客户端 fd 加入 poll 数组
                    int j;
                    for (j = 0; j < MAX_EVENTS; j++)
                    {
                        if (fds[j].fd == -1)
                        {
                            fds[j].fd = cfd;
                            fds[j].events = POLLIN;
                            fds[j].revents = 0;
                            break;
                        }
                    }

                    // 更新 max_index
                    if (j > max_index)
                        max_index = j;

                    // 如果数组满了，打印警告
                    if (j == MAX_EVENTS)
                    {
                        cout << "警告：客户端数量已达上限！" << endl;
                        close(cfd);
                    }
                }
                else
                {
                    // ===== 情况2：已有客户端发送数据 =====
                    char buf[1024];
                    int len = read(fds[i].fd, buf, sizeof(buf) - 1);

                    if (len == -1)
                    {
                        perror("read");
                        close(fds[i].fd);
                        fds[i].fd = -1;
                    }
                    else if (len == 0)
                    {
                        // 客户端断开连接
                        cout << "客户端断开连接 (fd=" << fds[i].fd << ")" << endl;
                        close(fds[i].fd);
                        fds[i].fd = -1;
                    }
                    else
                    {
                        buf[len] = '\0';
                        cout << "收到数据 (fd=" << fds[i].fd << "): " << buf;

                        // 回显数据（echo server）
                        write(fds[i].fd, buf, len);
                    }
                }

                nready--;  // 处理完一个就绪事件
            }
        }
    }

    // 关闭所有连接
    for (int i = 0; i <= max_index; i++)
    {
        if (fds[i].fd != -1)
        {
            close(fds[i].fd);
        }
    }

    return 0;
}