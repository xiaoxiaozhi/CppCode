#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
/**
 * 守护进程（Daemon Process），也就是通常说的 Daemon 进程（精灵进程），是 Linux 中的后台服务进程。它是一个生存期较长的进程，通常独立于控制终端并且周期性地执行某种任务或等待处理某些发生的事件。一般采用以d结尾的名字。
 * 进程组：多个进程的集合就是进程组, 这个组中必须有一个组长, 组长就是进程组中的第一个进程，组长以外的都是普通的成员，
 *        每个进程组都有一个唯一的组ID，进程组的ID和组长的PID是一样的。进程组中的成员是可以转移的，
 *        如果当前进程组中的成员被转移到了其他的组，或者进制中的所有进程都退出了，那么这个进程组也就不存在了。
 *        如果进程组中组长死了, 但是当前进程组中有其他进程，这个进程组还是继续存在的。
 *        pid_t getpgrp(void); 得到当前进程所在的进程组的组ID
 *        pid_t getpgid(pid_t pid); 获取指定的进程所在的进程组的组ID，参数 pid 就是指定的进程
 *        int setpgid(pid_t pid, pid_t pgid); 将某个进程移动到其他进程组中或者创建新的进程组
 * 会话：会话(session)是由一个或多个进程组组成的，一个会话可以对应一个控制终端, 也可以没有。一个普通的进程可以调用 setsid() 函数使自己成为新 session 的领头进程（会长），并且这个 session 领头进程还会被放入到一个新的进程组中。
 * 1.fork子进程，然后退出父进程（目的是创建会话，黑马老师说回话是脱离终端控制的，退出父进程后就可以脱离终端是这样吗？？？）
 *   大丙老师说的有道理，调用这个函数的进程不能是组长进程，先fork()创建子进程, 终止父进程, 让子进程调用这个函数，这个进程会变成当前会话中的第一个进程，同时也会变成新的进程组的组长
 *   该函数调用成功之后, 当前进程就脱离了控制终端，因此不会阻塞终端
 * 2.创建会话
 *   pid_t getsid(pid_t pid); 获取某个进程所属的会话ID
 *   pid_t setsid(void); 使用哪个进程调用这个函数, 这个进程就会变成一个会话  将某个进程变成会话 =>> 得到一个守护进程，
 * 3.根据需要，改变工作目录位置。不需要就不用设置
 * 4.根据需要，重新设置umask掩码
 * 5.根据需要，关闭文件描述符或者重定向
 * 6.守护进程业务逻辑
 * 
 * 感觉创建会话就是创建一个守护进程
 *
 */
int main()
{

    pid_t pid = fork();
    // 1.
    if (pid > 0)
    {
        printf("关闭父进程\n");
        exit(0);
    }
    sleep(1);
    pid_t child = getpid();
    printf("子进程创建会话 %d\n",child);
    // 2.创建会话
    pid = setsid(); //
    if (pid < 0){
        perror("创建会话失败\n");
    }
    // 3.改变工作目录
    const char *home = getenv("HOME");
    printf("home目录%s",home);
    int ret = chdir(home);
    if (ret < 0)
    {
        perror("改变工作目录失败");
    }
    execlp("pwd", "pwd", NULL);
    // 4.设置umarsk掩码
    umask(0022); // 改变文件访问权限
    // 5.关闭或者重定向文件描述符
    close(STDIN_FILENO); // 关闭文件，是告诉操作系统不再需要使用，该文件，文件具体什么时候关闭要等操作系统巨鼎，如果只是重定向文件描述符，并不等于关闭文件，文件还会一直打开。这里大丙老师的说法有问题，黑马老师正确
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    int fd = open("/dev/null", O_RDWR);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    while (1)
    {
        /* code */
    }
    
    return 1;
}