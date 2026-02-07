// /d:/CppCode/Chapter_4Expressions.cpp
// 简单的 C++ 程序，展示基本表达式与输出

#include <iostream>
using namespace std;
/**
 * 第四章 表达式
 * 4.1.1 基本概念
 *       一元运算符、二元运算符、三元运算符
 *       作用与一个运算对象的运算符是一元运算符，例如 -a 表示取负 &a 表示取地址 *表示解引用
 *       作用于两个运算对象的是二元运算符，例如 a + b 、a == b
 *       函数调用也是一种特殊的运算符，对运算对象数量没有限制
 *       运算符两侧的对象通常会转换成统一类型.例如 bool char short 转换成 int 、 float转成double
 *       重载运算符作用于类对象
 *       C++的表达式分为右值（rvalue）和左值（lvalue）。当一个对象被用作右值的时候，用的是对象的值（内容）；当对象被用作左值时，用的是对象的地址。需要右值的地方可以用左值代替，反之则不行
 * sizeof运算符
 * 4.11 类型转换
 *      隐式和显式：隐式转换由编译器自动进行，如 int 和 double 算数运算int会转成double。隐式转换被设计的尽量避免损失精度；
 *      显式转换需要程序员明确指定，如 static_cast<int>(3.14)
 *      4种显式转换 static_cast：静态类型转换 dynamic_cast：动态类型转换 const_cast：常量类型转换 reinterpret_cast：重新解释类型转换
 *      static_cast：
 *      基础类型之间的转换（如 int ↔ double、char ↔ int）；
 *      子类指针 / 引用 → 父类指针 / 引用（向上转型，安全）；
 *      void* 与其他类型指针的转换；static_cast 是 “编译期类型安全转换”，它有严格的规则：不能直接将 T** 转换为 void**（二级指针的跨类型转换），编译器会直接报错。
 *      不能用于 “父类--->子类”
 *      dynamic_cast：
 *      用于运行时检查的多态类型转换，核心是 “向下转型”（父类→子类），确保转换安全。核心要求：转换的类必须包含虚函数（有虚函数才是多态类，才能在运行时识别类型）。
 *      父类指针 / 引用 → 子类指针 / 引用（向下转型），运行时检查对象实际类型；
 *      交叉转型（同一基类的不同子类之间转换）。
 *      const_cast：
 *      唯一能移除 / 添加变量的 const/volatile 限定符的转换
 *      移除 “真 const”（对象本身被声明为 const）的 const 限定并修改其值，会触发未定义行为
 *      volatile 是 C++ 中的 “禁止优化” 修饰符，用于标记可能被程序外部因素修改的变量（如硬件寄存器、多线程共享变量、中断处理变量，每次访问都直接读写内存，而非寄存器
 *      移除 volatile 后，导致读取到过期值、写入不生效。
 *      reinterpret_cast：
 *      这是 C++ 中最底层、最危险的强制类型转换，核心是对内存二进制位的 “暴力重解释
 *      本质：不做任何类型检查，仅将一段内存的二进制位 “重新解释” 为目标类型；
 *      适用：仅指针 / 引用、整数与指针、函数指针之间的转换；
 *      风险：高度依赖平台（大小端、内存对齐），不可移植，普通业务逻辑严禁使用；
 */
int main()
{
    // 4.11 const_cast 添加const 移除const
    int a = 10;
    const int *cp = &a;             // 指向常量的指针，不能通过*p修改a的值
    int *p = const_cast<int *>(cp); // 移除const属性
    // 如果a是一个常量
    //  const int a = 10;
    //  int *p = const_cast<int *>(cp); // 移除const属性,能编译也不会报错，但是会导致不可知的行为，不建议
    //  cont int* cp = const_cast<const int*>(p); // 添加const属性
    *p = 20; // 通过p修改a的值
    std::cout << "a = " << a << std::endl;
    // 4.11 reinterpret_cast
    // 场景1：int指针 → char指针（解读int的二进制字节）
    int num = 0x12345678; // 假设小端系统（低字节存低地址）
    // 重解释int*为char*，指向num的第一个字节
    char *char_ptr = reinterpret_cast<char *>(&num);
    // 输出num的第一个字节（小端系统下为0x78）
    cout << "第一个字节的十六进制值：" << (int)*char_ptr << endl;
    return 0;
}