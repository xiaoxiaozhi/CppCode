#include <unistd.h> // pipe()
#include <fcntl.h>
#include <string.h>
#include <stdio.h> //perror()
#include <sys/types.h>
#include <stdlib.h>   //exit()
#include <sys/wait.h> //wait()
#include <sys/stat.h> //mkfifo()
#include <sys/mman.h>//mmap()
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
 * offset: 磁盘文件的偏移量，文件从偏移到的位置开始进行数据映射，使用这个参数需要注意两个问题：偏移量必须是4k的整数倍, 写0代表不偏移这个参数必须是大于 0 的
 * 成功: 返回一个内存映射区的起始地址 失败: MAP_FAILED (that is, (void *) -1)
 * void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
 * */ 

int main(){

    return 1;
}