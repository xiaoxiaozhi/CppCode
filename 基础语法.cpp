#include <iostream>
using namespace std;

int main()
{
    // 引用-------------------------------------------------------------
    int a = 1;  //与a相比下面 两个变量属于复合类型（类就修饰符+变量名）
    int &n = a; //给a取一个别名（&属于声明的一部分 int &）(类型修饰符 * &)
    cout << "调用别名 n = " << n << endl;
    //int &m;   //报错，别名必须在定义时初始化
    int *p = &a; //指针（*属于声明的一部分 int &）(类型修饰符 * &)
    int *&r = p; //指向指针的引用 从右向左读，有助于理解这个复杂的类型

    int &&x = 5; //上面都是左值引用现在是右值引用
    cout << "右值引用" << x << endl;
    // const---------------------------------------------------------------
    int b = 10;
    const int &c = b; //对常量的引用，不能修改引用变量的值
    const int &d = c * 2;
    //int &e = c * 2; //报错 如果引用右侧是一个表达式，就会创建一个临时变量，
    //实际上是&e = temp，引用目的是为了改变被引用对象的值，但temp对应一个表达式，此时引用的目的消失了，所以C++不允许这种操作
    const double pi = 3.14;
    //double *p = &pi;//错误，p是一个普通指针
    const double *p = &pi; //指针常量才能指向常量， *p 不能赋值 但是可以指向另一个 变量
    p = &b;                //指针常量指向另一个变量

    int *const curErr = &b; // 指针是一个常量，不能改变指向；
    *curErr = 10;           // 但是能改变值

    const int max = 20; //常量表达式，由数据类型和初始值（编译时段就能确定）共同决定
    // const int temp = getSize()//不是一个常量表达式，因为初始值要到运行时段才能确定
    int min = 10;          //不是常量表达式，数据类型不是const
    constexpr int mf = 20; //C++ 11新标准  constexpr 方便编译器检验是否是一个常量表达式
    //constexpr int mf = getSize(); //如果getSize()是一个常量函数，那它就是一个常量表达式
    const int *pp = nullptr;// nullptr是c++中空指针类型的关键字
    return 0;
}