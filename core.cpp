#include <stdio.h>
#include <iostream>
using namespace std;
/**
 * 本章节来自黑马视频
 * 1. 内存分区模型
 *    1.1代码区：存放函数体的二进制代码，由操作系统进行管理的
 *           存放 CPU 执行的机器指令代码区是共享的，共享的目的是对于频繁被执行的程序，只需要在内存中有一份代码即可,代码区只读
 *    1.2全局区：存放全局变量、全局常量、静态变量以及字符串；该区域的数据在程序结束后由操作系统释放.
 *    1.3栈区：由编译器自动分配释放, 存放函数的参数值,局部变量、常量等
 *            不要返回局部变量地址，函数执行完后局部变量会回收，如果非要返回，系统会保留一次
 *    1.4堆区：由程序员分配和释放,若程序员不释放,程序结束时由操作系统回收
 *             用new 在堆区开辟内存,还是堆区的例子，用 int* a = new(10) return a 可以返回地址a,因为a在堆区不会回收
 *    堆内存是分配给用户的，不手动释放系统是不会帮你释放，栈区对象离开作用域，系统就会释放，全局进程解释释放
 * 2. new操作符
 *    new 类型(参数) 返回地址，用指针接受  new int(10)构造函数吗？  new int[10] 返回10个元素空间的整形数组
 *    用delete关键字释放堆区的数据     delete 指针 释放单个变量     delete[] 指针  释放数组
 * 3. 引用
 *    引用的作用就是给变量起个别名 数据类型 &别名 = 原名  给变量起个别名，通过别名也可以操作这块内存
 *    引用必须要声明处初始化，一旦初始化就不能改变，引用的本质在c++内部实现是一个指针常量. int* const ref = &a;
 *    引用不可以直接赋值，这样写是错误的  int& ref = 10; 换种写法就可以 cont int& ref = 10；实际编译器优化过 int temp = 10;int& ref = temp;
 *    常量引用，主要用来修饰形参，防止误操作
 *
 */
// 全局变量
int g_a = 10;
int g_b = 10;
// 全局常量
const int c_g_a = 10;
const int c_g_b = 10;
int *func();
int *func1();
// 3. 常量引用
void showValue(const int &v)
{
    // v += 10;
    cout << v << endl;
}
int main()
{

    // 1.2 全局区 ：全局变量常量、静态变量、字符串，通过观察可以发现全局变量地址开头与栈区不一样
    cout << "全局变量g_a地址为： " << (long long)&g_a << endl;
    cout << "全局变量g_b地址为： " << (long long)&g_b << endl;
    cout << "全局常量c_g_a地址为： " << (long long)&c_g_a << endl;
    cout << "全局常量c_g_b地址为： " << (long long)&c_g_b << endl;
    // 静态变量
    static int s_a = 10;
    static int s_b = 10;
    cout << "静态变量s_a地址为： " << (long long)&s_a << endl;
    cout << "静态变量s_b地址为： " << (long long)&s_b << endl;
    cout << "字符串常量地址为： " << (long long)&"hello world" << endl;
    cout << "字符串常量地址为： " << (long long)&"hello world1" << endl;
    // 局部变量、常量 在一个栈区 ，栈区和全局区开头明显不一样
    int a = 10;
    int b = 10;
    const int c_l_a = 10;
    const int c_l_b = 10;
    cout << "局部常量c_l_a地址为： " << (long long)&c_l_a << endl;
    cout << "局部常量c_l_b地址为： " << (long long)&c_l_b << endl;
    cout << "局部变量a地址为： " << (long long)&a << endl;
    cout << "局部变量b地址为： " << (long long)&b << endl;

    // 1.3 栈区： 不要返回局部变量地址，因为函数执行后就会自动回收局部变量
    //  int *p = func();
    //  cout << *p << endl;//系统会帮你保存一次
    //  cout << *p << endl;//第二次执行就打印不出来
    //  老师用的vs 就能执行，vscode 执行不了

    // 1.4 堆区
    int *p = func1();
    cout << *p << endl;
    cout << *p << endl;
    // 2. new关键字
    delete p;           // 释放堆区
    cout << *p << endl; // 释放后发现改地址数据不在是10；黑马老师的vs会报非法访问，vscode可以执行
    // 3 引用---别名：通过别名才做这块内存
    int aa = 10;
    int &bb = aa;
    cout << "a = " << aa << endl;
    cout << "b = " << bb << endl;
    bb = 100;
    cout << "a = " << aa << endl;
    cout << "b = " << bb << endl;
    showValue(20);

    return 0;
}
// int * func()
// {
// 	int a = 10;
// 	return &a;
// }
int *func1()
{
    int *a = new int(10);
    return a;
}
