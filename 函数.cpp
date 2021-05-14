#include <iostream>
using namespace std;
//改变 传递值的 两种参数
void reset(int *p); // 指针形参
void reset(int &p); //传引用参数
int main()
{
    int a = 6;
    reset(&a); //指针形参
    cout << "a = " << a << endl;
    reset(a); //引用参数
    cout << "a = " << a << endl;
    return 0;
}
void reset(int *p) // 指针形参
{
    *p = 0;
    cout << "指针参数 p = " << *p << endl;
}
void reset(int &p)
{
    p = 50;
    cout << "引用参数 p = " << p << endl;
}