#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;
int main()
{
    
    // pid_t t = fork();
    // cout<<"进程号---"<<t<<endl;
    printf("第一行");
    printf("第一点五行");
    sleep(5);
    printf("第二行");
    return 1;
}