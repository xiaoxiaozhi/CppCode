#include <iostream>
using namespace std;
template <typename T> //template模板声明关键字，声明使用模板,并定义T是一个模板类型 多参数用，号隔开<typename T,E >
//vector // 类型
void mySwap( T &a,  T &b) //函数模板 
{
    T c = a;
    a = b;
    b = c;
}
int main()
{
    int a = 4, b = 5;
    mySwap(a, b);
    cout << a << b << endl;
    return 0;
}