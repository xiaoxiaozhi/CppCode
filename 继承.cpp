#include <iostream>
using namespace std;
//1.访问权限：派生类可以访问基类中所有的非私有成员。因此基类成员如果不想被派生类的成员函数访问，则应在基类中声明为 private。
//2.基类的构造函数、析构函数和拷贝构造函数、基类的重载运算符、基类的友元函数。
class A
{
public:
    A()
    {
        cout << "creat A    " << endl;
    }
    void fun1()
    {
        cout << "this is parent" << endl;
    }
};
class B : virtual public A //访问修饰符默认是private，我们几乎不使用 protected 或 private 继承，，通常使用 public 继承
{
public:
    B()
    {
        cout << "creat B " << endl;
    }
    void fun1()
    {
        cout << "this is B" << endl;
    }
};
class C : virtual public A //虚继承:为了解决多继承时的命名冲突和冗余数据问题，C++ 提出了虚继承，使得在派生类中只保留最上面基类的成员A。
{
public:
    void fun1()
    {
        cout << "this is C" << endl;
    }
};
class D : public B, public C
{
public:
    void fun1()
    {
        cout << "this is D" << endl;
    }
};
int main()
{
    D d; //A创建了两遍，用虚继承避免
    d.fun1();
    A *a = &d;
    a->fun1();
    return 0;
}