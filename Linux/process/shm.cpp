#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
/**
 * 共享内存
 * 共享内存不同于内存映射区，它不属于任何进程，并且不受进程生命周期的影响。通过调用Linux提供的系统函数就可得到这块共享内存
 * 在所有进程间通信的方式中共享内存的效率是最高的。
 * 1. 创建共享内存
 * int shmget(key_t key, size_t size, int shmflg);
 * 
*/
int main(){

    return 1;
}