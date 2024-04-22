#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
/**
 * 共享内存
 * 共享内存不同于内存映射区，它不属于任何进程，并且不受进程生命周期的影响。通过调用Linux提供的系统函数就可得到这块共享内存.
 * 在所有进程间通信的方式中共享内存的效率是最高的。
 * 1. 创建共享内存
 * key: 类型 key_t 是个整形数, 通过这个key可以创建或者打开一块共享内存，该参数的值一定要大于0，key_t key = ftok("/home/robin", 'a'); 路径和第二个参数是随便写的吗
 * size: 创建共享内存的时候, 指定共享内存的大小，如果是打开一块存在的共享内存, size是没有意义的
 * shmflg：IPC_CREAT|0664|IPC_EXCL
 * 返回值：共享内存创建或者打开成功返回标识共享内存的唯一的ID，失败返回-1
 * int shmget(key_t key, size_t size, int shmflg);
 * 2. 进程与共享内存关联
 * shmid: 要操作的共享内存的ID, 是 shmget() 函数的返回值
 * shmaddr: 共享内存的起始地址, 用户不知道, 需要让内核指定, 写NULL
 * shmflg: SHM_RDONLY: 读权限, 只能读共享内存中的数据  0: 读写权限，可以读写共享内存数据
 * void *shmat(int shmid, const void *shmaddr, int shmflg);
 * 3. 解除关联
 * 参数：shmat() 函数的返回值, 共享内存的起始地址
 * 返回值：关联解除成功返回0，失败返回-1
 * int shmdt(const void *shmaddr);
 * 4. 删除共享内存
 * int shmctl(int shmid, int cmd, struct shmid_ds *buf);
 * shmctl() 函数是一个多功能函数，可以设置、获取共享内存的状态也可以将共享内存标记为删除状态
 * 当共享内存被标记为删除状态之后，并不会马上被删除，直到所有的进程全部和共享内存解除关联，共享内存才会被删除。
 * 因为通过shmctl()函数只是能够标记删除共享内存，所以在程序中多次调用该操作是没有关系的。
 * 5.相关命令
 * 使用ipcs 添加参数-m可以查看系统中共享内存的详细信息
 * 
*/
int main(){

    return 1;
}