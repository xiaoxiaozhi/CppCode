#include <iostream>
using namespace std;
template <typename T>       //声明使用模板,并定义T是一个模板类型
//vector // 类型
void Swap(T& a, T& b)           //紧接着使用T
{
    T c = a;
    a = b;
    b = c;
} 
int main()
{
    return 0;
}