#include <iostream>
#include "swap.h"
using namespace std;
/**
 * 黑马视频 https://www.bilibili.com/video/BV1et411b73Z?p=50&vd_source=9cc1c08c51cf20bda524430137dc77bb
 * 1.函数定义
 *   返回值类型 函数名 （参数列表）{
 *     函数体语句
 *     return 表达式
 *   }
 * 2.函数份文件编写
 *   创建后缀名为.h的头文件
 *   创建后缀名为.cpp的源文件
 *   在头文件中写函数的声明
 *   在源文件中写函数的定义
 * 3.值传递、地址传递、引用传递  
 *   后两者效果一样都能改变实参，
 *   void mySwap01(int a, int b)   值传递
 *   void mySwap02(int* a, int* b) 地址传递
 *   void mySwap03(int& a, int& b) 引用传递
 * 4.引用作为函数返回值
 *   int& test02(){return 变量}  返回值类型& 函数体返回变量  
 * 5.默认参数
 * 
 * 6.函数指针  来自千询
 *   函数指针是指向函数的指针，可以用来动态地调用函数，传递函数作为参数，或者在运行时确定调用的函数。函数指针的声明和使用可以大大增加程序的灵活性和可扩展性。
 *   函数指针的声明形式为 返回类型 (*指针名称)(参数类型,参数类型)
 *   通过函数 指针可以直接调用函数 int result = ptr(3, 5);  // 调用 add 函数
 *   函数指针作为函数参数 void process(int (*func)(int, int))
 *   使用 typedef 简化声明  typedef int (*FuncPtr)(int, int); FuncPtr ptr = add;  // 使用 typedef 声明的函数指针
 *   
 */
// 改变 传递值的 两种参数
void reset(int *p); // 指针形参
void reset(int &p); // 传引用参数
int func(int a, int b = 10, int c = 10) {
	return a + b + c;
}

//6. 函数指针
int add(int a, int b) {
    return a + b;
}
int (*ptr)(int, int) = add;  // 指向 add 函数的指针
int main()
{
    int a = 6;
    reset(&a); // 指针形参
    cout << "a = " << a << endl;
    reset(a); // 引用参数
    cout << "a = " << a << endl;
    int a1 = 100;
	int b = 200;
	// swap1(a1, b);//要修改cmake才能多文件编译
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