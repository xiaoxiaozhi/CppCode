#include "linux_head.h"
// io 需要引入的头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <errno.h>  //errno
#include <string.h> //strerror
#include <stdio.h>  //fputc c语言标准函数库
/**
 * man 命令章节
 * 1 用户命令或可执行程序的文档
 * 2 系统调用（内核函数）的文档
 * 3 C库函数的文档
 * 4 特殊文件（通常是/dev下的文件）和设备驱动程序的文档
 * 5 文件格式和约定的文档
 * 6 游戏的文档
 * 7 杂项文档
 * 8 系统管理员命令的文档
 * 9 内核开发的文档
 * linux 文件操作
 * 1.open 打开文件
 * 在线man手册 https://man.cx/open(2)
 * Open()打开由路径名指定的文件。如果指定的文件不存在，则可以通过 open()选择创建它(如果在标志中指定了 O_CREAT)
 * arg1:文件路径
 * arg2:必须包含这几个中的一个，O_RDONLY, O_WRONLY, or O_RDWR.其他flag可以通过 | 添加(标志很多，看手册)
 * arg3:如果创建文件， 赋予文件的权限  (mode & ~umask) Ubuntu umask值=0002   传入 0777  (0777 & ~umask)=0775 。生成的文件权限确实是0775
 * 终端输入umask可以得到当前系统的umask值当前是0002
 * int open(const char *pathname, int flags, mode_t mode);//创建文件时必须制定mode_t，如果不指定，会有什么后果呢？mode_t种类繁多具体看manpages
 * 2. 读写
 * arg1:文件描述符
 * arg2：缓冲区
 * arg2：读写的字节
 * ssize_t read(int fd, void *buf, size_t count);正常状态读取几个字节返回字节数，读到结尾返回0 ，读错返回-1
 * ssize_t write(int fd, const void *buf, size_t count);
 * 读写系统调用速度比不上c库函数读写文件，因为系统调用没有用缓冲区，是一个字节一个字节读写。每次读写都要从用户空间到内核空间切换，状态切换是个耗时操作
 * 系统调用就是帮助用户调用内核空间才有的功能
 * 这俩函数差不多，返回一个有符号整形，失败返回-1 并且设置errno
 * 写完要close关闭文件，再次open文件才能读文件
 * c语言标准函数库 fopen，fgetc读，fputc写 ，fclose关闭 头文件<stdio.h>
 * "r"：只读模式，文件必须存在。文件的指针将被放在文件的开头。
 * "w"：只写模式，如果文件存在则清空文件，如果文件不存在则创建文件。
 * "a"：追加模式，如果文件存在则将数据追加到文件末尾，如果文件不存在则创建文件。文件的指针将被放在文件的末尾。
 * "r+"：读写模式，文件必须存在。文件的指针将被放在文件的开头。
 * "w+"：读写模式，如果文件存在则清空文件，如果文件不存在则创建文件。如果文件不存在，尝试创建一个新文件。文件的指针将被放在文件的开头。
 * "a+"：读写模式，如果文件存在则将数据追加到文件末尾，如果文件不存在则创建文件。文件的指针将被放在文件的末尾。
 *
 *
 * 3. 关闭文件
 *
 * open("", O_CREAT | O_WRONLY , S_IRWXU) 存在就覆盖文件 不存在就创建
 * open("", O_CREAT | O_WRONLY | O_EXCL, S_IRWXU);  存在报错-1，不存在创建
 * open("", O_WRONLY | O_CREAT | O_APPEND, S_IRWXU); 存在就追加，不存在就创建
 * O_EXCL:用于在打开文件时指定文件存在时的处理方式。如果指定的文件已经存在，则open函数会失败，并返回错误-1。文件存在
 * O_TRUNC：把文件清零  open("", O_CREAT | O_WRONLY | O_APPEND, S_IRWXU); 如果文件存在就清零，不存在就创建
 *
 * 文件权限受umask影响，(mode & ~umask).  umask 是一条命令输入后显示一个8进制的值，这条看不明白
 * open函数 失败会自动设置erron操作系统的全局变量<erron.h> 想知道erron具体什么意思用strerror(errno) 头文件string.h
 * trerror(errno)得到的错误与perror一致
 * open函数还能打开目录
 * printf("read了%d个字符--%.*s", n, n, buf_read); 按长度打印数组长度
 *
 * strace ./可执行文件，可以打印程序执行过程中的系统调用
 * PCB进程控制块 中保存着文件描述符表，表中的每个文件描述符是一个指针，open函数返回的fd就是表的下标。一个进程最多打开1024个文件
 * 老师说前三个文件描述符是 STDIN_FILENO STDOUT_FILENO STDERR_FILENO  这几个东西是什么呢？？？确实是这样，我自己打开open后得到值是3，验证了这个说法
 * 4. 阻塞与非阻塞
 *    读设备或者读网络会产生阻塞，读常规文件不会产生阻塞。(常规文件是存储卡里的文件吗？)设备文件 dev/tty  键盘 外设之类的
 *    read(STDIN_FILENO, keyRead, sizeof(keyRead)); 会一直等待键盘输入 产生阻塞
 *    阻塞是设备文件和网络文件的属性，以非阻塞方式打开文件 open("dev/tty",O_WRONLY|O_NONBLOCK)，如果没有数据返回-1并且erron赋值 EWOULDBLOCK
 *    TODO O_NONBLOCK 04000 在minggw64下面没有，在ubuntu里面有。
 * 在C++中，如果使用printf函数输出内容时不加\n，输出的内容可能不会立即打印出来，而是会被缓冲起来，直到缓冲区满了或者遇到换行符才会刷新输出。
 *   printf("Hello, ");
 *   fflush(stdout); // 强制刷新，输出缓冲
 *   printf("world!");
 *5. fcntl()改变文属性
 *   上文中的例子，如果键盘一直不输入就会一直阻塞，那有什么办法改变文件的属性呢？
 *   本例使用fcntl 不重新打开文件描述符就修改为非阻塞属性 设置O_NONBLOCK或O_NDELAY 改变阻塞属性
 *   /dev/tty 查了下这个是标准输入和标准输出文件，老师重新打开这个文件。
 *   read(STDIN_FILENO, keyRead, sizeof(keyRead)); 读外设文件，读错返回-1 ，另一种情况，读设备文件 以非阻塞方式，如果没有读到数据，此时系统会设置
 *   read返回-1 并且 erron=EAGAIN or EWOULDBLOCK
 *   fcntl不用重新打开文件就可以改变文件属性
 *6. 移动文件指针、扩展文件
 *   off_t 返回偏移结果
 *   off_t lseek(int fd, off_t offset, int whence);
 *   6.1 移动文件指针
 *        lseek(fd,0,SEEK_END);移动到最后  lseek(fd,0,SEEK_CUR);得到当前文件指针位置 lseek(fd,0,SEEK_SET)文件指针移动到文件头部
 *   6.2 扩展文件
 *       lseek(fd,10,SEEK_END)   然后必须再写一个字符 write(fd," ",1)否则扩展无效
 * 7.文件扩展或者截断
 *   int truncate(const char *path, off_t length);
 *	 int ftruncate(int fd, off_t length);  
 *   文件原来size > length，文件被截断, 尾部多余的部分被删除, 文件最终长度为length
 *   文件原来size < length，文件被拓展, 文件最终长度为length
 *   两者区别在于 一个传入路径一个传入 文件描述符。 上文提到扩展文件要再写一个字符否则无效，用这个函数可以直接扩展无需再写
 *   ftruncate(fd,1024);
 *   
 * 注意：读或写函数执行完必须 close 否则无法 继续读或写。正确  read close write 或者 write close read；错误 read write 或者 write read 。
 *      读或者写之后可以移动文件指针吗？
 * 
 * 
 */
void unixIo();  // 没有缓冲机制，读写要慢，好处是能立即的写入文件。库函数要等缓冲区满了才能写入文件各有优缺点。
void cio();     // 有缓冲机制读写快。这个例子读写字符，怎么读写二进制呢？总结库函数要比系统函数好用
void block();   // 阻塞
void unblock(); // 不重新打开改变文件属性
int main()
{
    // 5.改变文件阻塞属性
    unixIo();
    cio();
    block();
    printf("改变标准输入文件属性--->非阻塞，再次执行block()看是否阻塞\n");
    unblock();
    int fd = open("lseek.txt", O_CREAT | O_RDWR, 0777);
    char buf[1024];
    memset(buf, '\0', 1024);
    int n = 0;
    if (fd < 0)
    {
        perror("创建lseek.txt失败---");
    }
    // 6.1 移动文件指针
    off_t ft = lseek(fd, 0, SEEK_END); // 虽然没有指定文件续写属性，但是指针移动到最后，再写字符就实现了续写功能
    if (ft < 0)
    {
        perror("lseek移动指针失败---");
        exit(1);
    }
    printf("偏移---%d\n", ft); // 该例文件指针偏移到尾部并返回 指针位置。
    int resultR = write(fd, "a", 1);
    if (resultR < 0)
    {
        perror("写lseek.txt失败---");
        exit(1);
    }
    close(fd);
    fd = open("lseek.txt", O_CREAT | O_RDWR, 0777);
    while ((n = read(fd, buf, sizeof(buf))) > 0)
    {
        printf("读lseek %s\n", buf);
    }
    if (n < 0)
    {
        perror("读lseek.txt失败---");
        exit(1);
    }
    close(fd);
    fd = open("lseek.txt", O_CREAT | O_RDWR, 0777);
    // 6.2 扩展文件
    off_t ft1 = lseek(fd, 9, SEEK_END);
    write(fd, "b", 1);
    printf("扩展后文件指针位置---%d\n", ft1);
    perror("");
    return 1;
}
void unixIo()
{
    // 1. 打开文件
    int descriptor = open("./open_fun.txt", O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
    if (descriptor < 0)
    {
        // perror("创建文件描述符失败---%d");
        printf("打开或创建open_fun.txt失败 erron---%d strerror---%s", errno, strerror(errno));
        exit(1);
    }
    cout << "创建文件成功---" << descriptor << endl;
    // 2. 读写
    const char *buf = "hello word\n";
    printf("写buf长度%zu\n", strlen(buf));
    write(descriptor, buf, strlen(buf));
    close(descriptor);

    int df = open("./open_fun.txt", O_RDONLY);
    if (df < 0)
    {
        perror("打开文件open_fun.txt失败---");
        exit(1);
    }
    char buf_read[1024];
    int n = 0;
    while ((n = read(df, buf_read, sizeof(buf_read))) > 0)
    {
        printf("read了%d个字符--%.*s\n", n, n, buf_read);
    }

    printf("读取结束 erron---%d strerror---%s\n", errno, strerror(errno));
    // 3.关闭文件
    close(df);
}
void cio()
{
    FILE *fp, *fp_out;
    int n;
    char buffer[100];
    fp = fopen("open_fun.txt", "r"); // r r+ w w+ a(续写) a+(续写不存在就创建)
    if (fp == NULL)
    {
        perror("fopen打开失败");
        exit(1);
    }
    fp_out = fopen("dict.cp", "w"); // w文件存在就清空，文件不存在就创建一个
    if (fp_out == NULL)
    {
        perror("fopen打开失败");
        exit(1);
    }
    // 一次读一个
    //  while ((n = fgetc(fp)) != EOF)
    //  {
    //      fputc(n, fp_out);
    //  }
    // 一次读一行。 不用关心buffer实际大小吗？？？fgets 好像读取的时候已经给数组加了结束符，这样调用 fputs 会将字符串buffer写入到指定的文件中，直到遇到null终止符。
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        fputs(buffer, fp_out);
    }
    fclose(fp);
    fclose(fp_out);
}
void block()
{
    // 4. 读外设，阻塞
    char keyRead[5];
    int keybord = read(STDIN_FILENO, keyRead, sizeof(keyRead));
    if (keybord > 0)
    {
        printf("键盘输入了%d字符---%.*s\n", keybord, keybord, keyRead);
        // keyRead[2]='\0';
        //  printf("键盘输入了%d字符---%s\n", keybord, keyRead);
    }
    else
    {
        perror("读键盘err---");
    }
}
void unblock()
{
    int flag = fcntl(STDIN_FILENO, F_GETFL);
    // flag |= O_APPEND;
    flag |= O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, flag);
    block();
}