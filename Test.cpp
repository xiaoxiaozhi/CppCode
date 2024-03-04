#include <iostream>
#include <string.h>
class TT
{
public:
    int *t1;
};
int main()
{
    TT tt;
    *tt.t1 =  20;//为什么无法赋值？？？
    return 1;
}