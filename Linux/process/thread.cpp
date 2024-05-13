#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
/**
 * 线程是轻量级的进程，本质还是进程(Linux)
 * 与进程相比线程没有独立的 存储空间，但是有独立的PCB
 * 线程是最小的执行单位，进程是分配资源的单位，进程可以看做是只有一个线程的进程。
 * 三个进程争夺cpu，如果进程A开启三个线程，得到执行机会将是其他两个进程的三倍。不断增加线程的数量会得到更多执行机会，直到一定值，再多反而会降低机会。这个说法对吗？？？
 * ps -Lf 进程号 可以查看创建的进程 LWP线程号  NLWP线程个数
 * 因为都有pcb，进程和线程对于Linux内核是一样的
 * 进程可以蜕变成线程，线程可以看做是寄存器和栈的集合
 * 线程共享：文件描述符、工作目录、用户ID和阻id内存地址空间
 * 线程非共享：线程id，栈指针、栈空间、errno变量、信号屏蔽字、调度优先级
 * 1.创建线程
 *   thread: 传出参数，是无符号长整形数，线程创建成功, 会将线程ID写入到这个指针指向的内存中
 *   attr: 线程的属性例如优先级等, 一般情况下使用默认属性即可, 写NULL
 *   start_routine: 函数指针，创建出的子线程的处理动作，也就是该函数在子线程中执行。
 *   arg: 作为实参传递到 start_routine 指针指向的函数内部
 *   线程创建成功返回0
 *   int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);
 *   pthread_t pthread_self(void); 获取线程号
 * 2.循环创建线程
 *   
 * 子线程被创建出来之后需要抢cpu时间片, 抢不到就不能运行，如果主线程退出了, 虚拟地址空间就被释放了, 子线程就一并被销毁了。但是如果某一个子线程退出了, 主线程仍在运行, 虚拟地址空间依旧存在。
 * 编译的时候要加上 -lpthread 参数
 */
void *tfun(void *arge)
{
    printf("线程创建成功 %ld\n", pthread_self());
    return NULL;
}
int main()
{

    printf("当前线程的id %ld ,当前进程的id %d\n", pthread_self(), getpid()); // 为什么还没创建线程就可以得到线程id，且进程id和线程id不一致
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, tfun, NULL);
    printf("ret %ld tid %ld\n", ret, tid);//不加 \n打印不出来线程 回调函数，神奇了。这是因为子线程被创建出来之后需要抢cpu时间片, 抢不到就不能运行，如果主线程退出了, 虚拟地址空间就被释放了, 子线程就一并被销毁了。但是如果某一个子线程退出了, 主线程仍在运行, 虚拟地址空间依旧存在。
    if (ret < 0)
    {
        perror("创建线程失败");
    }
    sleep(2);//进程执行完，释放虚拟空间，如果线程还来不及执行就会被销毁
    return 1;
}