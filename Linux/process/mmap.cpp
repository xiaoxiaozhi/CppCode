#include <unistd.h> // pipe()
#include <fcntl.h>
#include <string.h>
#include <stdio.h> //perror()
#include <sys/types.h>
#include <stdlib.h>   //exit()
#include <sys/wait.h> //wait()
#include <sys/stat.h> //mkfifo()
#include <sys/mman.h> //mmap()
/**
 * 内存映射实现进程间通信
 * 内存映射区和同一个磁盘文件进行映射，这样进程之间就可以通过磁盘文件这个唯一的桥梁完成数据的交互了
 * 这样做还有一个好处，操作内存就相当于操作文件，除了read write 还可以用指针、各种函数来操作文件
 *
 * ddr: 一般指定为NULL, 委托内核分配
 * length: 创建的内存映射区的大小（单位：字节），实际上这个大小是按照4k的整数倍去分配的
 * prot: 对内存映射区的操作权限;PROT_READ: 读内存映射区 ;PROT_WRITE: 写内存映射区 ;如果要对映射区有读写权限: PROT_READ | PROT_WRITE
 * flags:MAP_SHARED: 多个进程可以共享数据，进行映射区数据同步;MAP_PRIVATE: 映射区数据是私有的，不能同步给其他进程
 * fd: 文件描述符, 对应一个打开的磁盘文件，内存映射区通过这个文件描述符和磁盘文件建立关联
 * offset: 磁盘文件的偏移量，文件从偏移位置开始进行数据映射，使用这个参数需要注意两个问题：偏移量必须是4k的整数倍, 写0代表不偏移，从头开始映射
 * 成功: 返回一个内存映射区的起始地址 失败: MAP_FAILED (that is, (void *) -1)
 * void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
 * 1. 创建映射文件
 *    open(O_CREAT) 再调用 lseek() write()这是因为大小为0的文件无法映射，所以要给文件设置长度(黑马老师这样说的但是大丙老师用例子证明可以)
 * 2. 创建映射区
 *    void *p = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
 * 3. 向映射区写
 *     strcpy(p,"向内存映射写字符"); 然后查看文件是否写入
 * 4. 关闭文件、映射区
 *    len:映射区大小，我如果填的值比真实值大或者小会怎么样呢？？？
 *    munmap(p,len);
 * 5. 进程间通过映射文件
 * 
 *
 *
 * */

int main()
{
    // 1. 创建映射文件
    int fd = open("mmap.txt", O_CREAT | O_RDWR, 0777);
    // off_t len = lseek(fd, 1023, SEEK_END);
    // write(fd,"\0",SEEK_END);
    // ftruncate(fd, 1024); // 给文件设置大小的原因是因为，系统资源紧张防止存储空间不够。我觉得这才是真正原因
    // int len = lseek(fd, 0, SEEK_END);//文件指针在末尾，在映射区写东西，结果出现在文件开头
    printf("偏移%d\n", 1024);
    // 2.创建映射区
    char *p = (char *)mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    // int pid = fork();
    strcpy(p, "vjb");// 相当与写文件
    printf("读映射区%s|\n",p);//相当与读文件
    close(fd);
    munmap(p, 1024);
    // if (pid > 0)
    // {

    // }
    // else if (pid == 0)
    // {
    // }
    perror("");

    return 1;
}