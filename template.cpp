#include <iostream>
using namespace std;
/**
 * 1.
*/
//1. 函数模板
//函数模板-------------------------------------------------------------------------------------------------------------
template <typename T>   //template模板声明关键字，声明使用模板,并定义T是一个模板类型 多参数用，号隔开<typename T,typenameE >
void mySwap(T &a, T &b) //函数模板
{
    T c = a;
    a = b;
    b = c;
}
template <unsigned M, unsigned N> //非类型模板参数 非类型参数表示一个值而不是一个类型,值在编译的时候就能确定
int compare(const char (&p)[N], const char (&q)[M])//
{
    if (N > M)
        return 1;
    if (N < M)
        return -1;
    return 0;
}
//类模板------------------------------------------------------------------------------------------------------------------

int main()
{
    int a = 4, b = 5;
    mySwap(a, b);
    cout << "a = " << a << "\tb = " << b << endl;
    cout << compare("13", "helo") << endl;
    return 0;
}