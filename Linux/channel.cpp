#include <unistd.h>
#include <fcntl.h>
#include <string.h>
using namespace std;
/**
 * 进程间通信方式
 * 管道 使用最简单
 * 信号 开销最小
 * 共享映射区 无血缘关系
 * 本地套接字 最稳定
 * 管道---进程间的通信（有血缘关系才能使用）
 * 内核中的一块4k内存，有两个文件描述符读和写
*/
int mian(){


    return 1;
}