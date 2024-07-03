#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <iostream>
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
 *   线程函数传值，注意值传递和地址传递的区别
 * 3.线程间共享变量
 *   位于同一虚拟地址空间中的线程，虽然不能共享栈区数据，但是可以共享全局数据区和堆区数据
 * 4.线程退出
 *   exit(0);//退出进程
 *   pthread_exit(void* retval)表示退出线程，可以和 return NULL相互替代， 其中retval 代表线程返回值，我从哪接收这个值呢？在这里吗pthread_create
 *   感觉语法上不同，但是编译器认为是一回事
 *   在线程函数中return NULL 退出线程，可以将NULL或nullptr等效void*类型的空指针
 * 5.等待线程终止
 *   pthread_join()
 * 
 *
 *
 *暂时不看了，学习socket
 *
 * 子线程被创建出来之后需要抢cpu时间片, 抢不到就不能运行，如果主线程退出了, 虚拟地址空间就被释放了, 子线程就一并被销毁了。但是如果某一个子线程退出了, 主线程仍在运行, 虚拟地址空间依旧存在。
 * 编译的时候要加上 -lpthread 参数
 * reinterpret_cast 类型转换.例如 int 转 void* reinterpret_cast<void *>(传入一个int变量)
 */
void *tfun(void *arge)
{
    printf("线程创建成功 %ld ，当前进程id %d\n", pthread_self(), getpid());
    return NULL;
}
void *tfun1(void *arge)
{
    sleep(1);
    printf("线程创建成功 %ld ，当前进程id %d 接收到的值%d\n", pthread_self(), getpid(), *((int *)arge));
    return NULL;
}
int var = 100; // 全局变量
void *tfun2(void *arge)
{
    var = 200;
    printf("子线程修改var值%d\n", var);
    return NULL;
}
void *tfun3(void *arge)
{
    long i = reinterpret_cast<long>(arge); // arge 是指针类型在64为平台上有8位，转成int会报错以及丢失精度，应该转成long
    // long i = (long)(arge);//这样也可以
    // int i = (int)arge;黑马老师是这样转的，我编译不过，怀疑他用的是32位操作系统
    if (i == 3)
    {
        printf("如果i=3退出线程---\n");
        // exit(0);//退出进程
        pthread_exit(NULL);
        // return NULL;//退出线程
        
    }
    printf("线程创建成功 %ld ，当前进程id %d 接收到的值%ld\n", pthread_self(), getpid(), i);
    // return NULL;// return NULL: 返回函数调用者不是退出线程。注释掉这句报错，pthread_exit(NULL);可以代替 return NULL
    pthread_exit(NULL);//可以代替 return NULL
}
void* tfun4(void *arge)
{
   pthread_exit((void*)30);
}
int main()
{

    // 没有创建线程也能得到线程id，pthread_self()认为进程是一个独享独立空间的线程
    printf("当前线程的id %ld ,当前进程的id %d\n", pthread_self(), getpid());
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, tfun, NULL);
    printf("ret %ld tid %ld\n", ret, tid); // 不加 \n打印不出来线程 回调函数，神奇了。这是因为子线程被创建出来之后需要抢cpu时间片, 抢不到就不能运行，如果主线程退出了, 虚拟地址空间就被释放了, 子线程就一并被销毁了。但是如果某一个子线程退出了, 主线程仍在运行, 虚拟地址空间依旧存在。
    if (ret < 0)
    {
        perror("创建线程失败");
    }
    sleep(1); // 进程执行完，释放虚拟空间，如果线程还来不及执行就会被销毁
    // 2. 循环创建线程
    printf("2.循环创建线程-----------------------------\n");
    for (int i = 5; i > 0; i--)
    {
        pthread_create(&tid, NULL, tfun1, &i); // 传递的是地址，当线程隔一秒执行的时候，i地址的值成了0，这里采用值传递就不会出现这种现象
    }
    sleep(3);
    // 3. 线程间共享变量
    printf("3. 线程间共享变量-----------------------------\n");
    printf("主线程var=%d\n", var);
    pthread_create(&tid, NULL, tfun2, NULL);
    sleep(1);
    printf("主线程var=%d\n", var);
    sleep(1);
    // 4. 线程退出
    printf("4. 线程退出-----------------------------\n");
    for (int i = 5; i > 0; i--)
    {
        pthread_create(&tid, NULL, tfun3, reinterpret_cast<void *>(i)); 
        //  pthread_create(&tid, NULL, tfun3, (void *)i); 黑马老师的方法，我编译不过
    }
    sleep(1);
    //5. 等待线程终止
    printf("5. 等待线程终止-----------------------------\n");
    pthread_create(&tid, NULL, tfun4, NULL);
    int **joinValue = NULL;
    pthread_join(tid,joinValue);
    printf("等待线程终止var=%d\n", **joinValue);
    pthread_exit((void *)0); // 退出主线程，不影响子线程，这样就不必sleep等待子线程执行完再结束进程
    return 1;
}