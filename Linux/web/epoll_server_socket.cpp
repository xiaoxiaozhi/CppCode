#include <sys/socket.h>
#include <stdio.h>
#include <cstdlib>      // exit()
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>
#include <arpa/inet.h>  // htons
#include <string.h>
#include <iostream>
#include <sys/epoll.h>  // epoll 头文件
#include <errno.h>
using namespace std;

/**
 * epoll 工作流程：
 *
 * 1. epoll_create()  → 创建 epoll 实例（红黑树 + 就绪链表）
 * 2. epoll_ctl()     → 将需要监控的文件描述符添加到红黑树
 * 3. epoll_wait()    → 阻塞等待，返回就绪的文件描述符列表
 *
 * 三大核心函数：
 *
 * int epoll_create(int size);
 *   - 创建 epoll 实例
 *   - 返回 epoll 文件描述符 efd
 *
 * int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
 *   - op: EPOLL_CTL_ADD(添加) / EPOLL_CTL_MOD(修改) / EPOLL_CTL_DEL(删除)
 *   - event.events: EPOLLIN(读) / EPOLLOUT(写) / EPOLLERR(异常)
 *   - event.data.fd: 存储文件描述符
 *
 * int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
 *   - events: 传出参数，内核将就绪事件写入该数组
 *   - timeout: -1(阻塞) / 0(立即返回) / >0(毫秒)
 *   - 返回: >0(就绪数量) / 0(超时) / -1(出错)
 */

#define MAX_EVENTS 1024

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
    addr.sin_port = htons(10000);
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

    cout << "========== epoll 服务器启动 ==========" << endl;
    cout << "监听地址: 127.0.0.1:10000" << endl;
    cout << "工作模式: LT（水平触发，默认模式）" << endl;
    cout << "======================================" << endl;

    // 5. 创建 epoll 实例
    int efd = epoll_create(MAX_EVENTS);
    if (efd == -1)
    {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }

    // 6. 将监听套接字 lfd 添加到 epoll 监控
    struct epoll_event event;
    event.events = EPOLLIN;     // 监听读事件
    event.data.fd = lfd;        // 保存文件描述符

    ret = epoll_ctl(efd, EPOLL_CTL_ADD, lfd, &event);
    if (ret == -1)
    {
        perror("epoll_ctl (add lfd)");
        exit(EXIT_FAILURE);
    }

    // 7. 创建 epoll_event 数组，用于接收就绪事件
    struct epoll_event events[MAX_EVENTS];

    // 8. 事件循环（核心）
    while (1)
    {
        cout << "\n等待事件就绪..." << endl;

        // epoll_wait() 阻塞等待事件就绪
        // 参数：efd, 传出数组events, 数组大小, 超时时间-1(永远阻塞)
        int nready = epoll_wait(efd, events, MAX_EVENTS, -1);

        if (nready == -1)
        {
            perror("epoll_wait");
            break;
        }

        // 遍历就绪的事件数组
        for (int i = 0; i < nready; i++)
        {
            if (events[i].events & EPOLLIN)  // 读事件就绪
            {
                if (events[i].data.fd == lfd)
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
                    cout << "新客户端连接: " << client_ip << ":" << ntohs(cliaddr.sin_port)
                         << " (fd=" << cfd << ")" << endl;

                    // 将新客户端 cfd 加入 epoll 监控
                    struct epoll_event ev;
                    ev.events = EPOLLIN;     // 监听读事件（LT模式，默认）
                    ev.data.fd = cfd;

                    ret = epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &ev);
                    if (ret == -1)
                    {
                        perror("epoll_ctl (add cfd)");
                        close(cfd);
                    }
                }
                else
                {
                    // ===== 情况2：已有客户端发送数据 =====
                    char buf[1024];
                    int fd = events[i].data.fd;
                    int len = read(fd, buf, sizeof(buf) - 1);

                    if (len == -1)
                    {
                        perror("read");
                        // 从 epoll 中移除并关闭
                        epoll_ctl(efd, EPOLL_CTL_DEL, fd, NULL);
                        close(fd);
                    }
                    else if (len == 0)
                    {
                        // 客户端断开连接
                        cout << "客户端断开连接 (fd=" << fd << ")" << endl;
                        epoll_ctl(efd, EPOLL_CTL_DEL, fd, NULL);
                        close(fd);
                    }
                    else
                    {
                        buf[len] = '\0';
                        cout << "收到数据 (fd=" << fd << "): " << buf;

                        // 回显数据（echo server）
                        write(fd, buf, len);
                    }
                }
            }

            // 如果有异常事件（如对方异常断开）
            if (events[i].events & (EPOLLERR | EPOLLHUP))
            {
                int fd = events[i].data.fd;
                cout << "客户端异常断开 (fd=" << fd << ")" << endl;
                epoll_ctl(efd, EPOLL_CTL_DEL, fd, NULL);
                close(fd);
            }
        }
    }

    // 清理
    close(lfd);
    close(efd);
    return 0;
}