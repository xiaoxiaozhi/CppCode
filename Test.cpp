#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t t = fork();
    cout<<"进程号---"<<t<<endl;
    return 1;
}