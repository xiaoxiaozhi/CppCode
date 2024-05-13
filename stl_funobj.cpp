#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
/**
 * 函数对象(仿函数)是一个类，不是一个函数
 * 重载函数调用操作符的类，其对象常称为函数对象，函数对象使用重载的()时，行为类似函数调用，也叫仿函数
 * 1.函数对象在使用时，可以像普通函数那样调用, 可以有参数，可以有返回值
 * 2.函数对象可以有自己的状态， 比如统计调用次数
 * 3.函数对象可以作为参数传递
 * 4.谓词
 *   返回bool类型的仿函数称为谓词
 *   如果operator()接受一个参数，那么叫做一元谓词
 *   如果operator()接受两个参数，那么叫做二元谓词
 */
// 1.
class MyAdd
{
public:
    int operator()(int v1, int v2)
    {
        return v1 + v2;
    }
};
// 2.
class MyPrint
{
    int count{};//c++11 引入的初始化语法

public:
    void operator()(string test)
    {
        cout << test << count++ << endl;
    }
};
// 3.
void doPrint(MyPrint &mp, string msg)
{
    mp(msg);
}
int main()
{
    // 1.
    MyAdd ma;
    int i = ma(1, 2);
    cout << "调用函数对象---" << i << endl;
    // 2.
    MyPrint mp;
    mp("函数对象有自己的状态---");
    mp("函数对象有自己的状态---");
    mp("函数对象有自己的状态---");
    mp("函数对象有自己的状态---");
    //3.
    doPrint(mp,"函数对象可以作为参数传递");
    int ii{};
    cout << ii << endl;
    return 1;
}