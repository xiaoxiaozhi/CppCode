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
 * 
 *   
 * 
 *
 *
 *TODO 函数指针，指向函数的指针， 视频没有要自己总结
 */
// 改变 传递值的 两种参数
void reset(int *p); // 指针形参
void reset(int &p); // 传引用参数
int func(int a, int b = 10, int c = 10) {
	return a + b + c;
}


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