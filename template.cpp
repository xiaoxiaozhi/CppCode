#include <iostream>
#include <typeinfo>
using namespace std;
/**
 * 模版
 * C++另一种编程思想称为 泛型编程 ，主要利用的技术就是模板；C++提供两种模板机制:函数模板和类模板
 * 1.函数模版
 *   建立一个通用函数，其函数返回值类型和形参类型可以不具体制定，用一个虚拟的类型来代表。
 *   声明函数模版template<typename T> 换行 T func(T t1,T t2)
 *   template --- 声明创建模板
 *   typename --- 声明其后面的符号是一种数据类型，可以用class代替
 *   模版能自动类型推导，如果有多个参数推导出来参数类型必须一致，也就是说模版函数传进去的参数一样
 *   还能显示指定类型 fun<类型>()，模版的意义就是类型参数化，提高复用能力
 *   模版必须要确定T的数据类型,创建模版时没有指定，调用的时候必须显示的指定 func<int>();
 * 2.利用函数模版进行排序
 *   函数模版实际应用暂时不看
 * 3.普通函数与函数模版区别
 *   普通函数可以发生隐式类型转换，函数模版用类型推导不可以发生隐式类型转换，用显式指定可以发生隐式类型转换,值传递显式指定无法发生类型转换
 * 4.普通函数和模版函数调用规则
 *   如果普通函数和模版函数重名，优先调用普通函数，普通函数只有声明的话就会报错；想调用模版函数就要通过空函数列表强制调用模版函数 myPrint<>();
 *   4.1 函数模版也可以发生重载
 *   4.2 如果函数模版可以发生更好的匹配优先调用函数模版
 *   4.3 模版的局限性 比如说addNum 模版，传入的是自定义类型根本就没有加法，此时就会报错，为了解决这列问题提供了模版的重载
 *       利用具体化的模板，可以解决自定义类型的通用化 template<> 模版函数 () T替换成具体类型，遇到自定义类型会优先调用具体化模版
 * 5.类模版
 *   定义：template<typename T，typename E> 换行 class DiyClass{T t1，E e1}
 *   类模板在定义的时候和函数模版基本一致，但是类模板没有类型推导，只能显式指定参数类型
 *   类模板可以设置默认参数类型,默认类型支持隐式转换，int->double 'c'->string
 *   C++中的内置类型（如整数、浮点数、指针等）之间可以发生隐式类型转换，自定义类型可以定义转换函数例如 operator int() { return m_value; }
 *   5.1 类模板成员函数创建时机
 *       只要不调用就不会去创建，模版的类型T 对象可以执行任何函数和方法。感觉不通用只能匹配适合的类。如果类模板指定的参数类型没有这个方法或者构造函数错误
 *       就会报错。c++模版远不如java的泛型好用
 *   5.2 类模板作为函数参数
 *       5.2.1指定传入的类型 --- 直接显示对象的数据类型，实际开发中这种最常用
 *       5.2.2参数模板化 --- 将对象中的参数变为模板进行传递, 参数模版化与制定传入类型比较相似
 *       5.2.3整个类模板化 --- 将这个对象类型 模板化进行传递
 *   5.3 类模板与继承
 *       当子类继承的父类是一个类模板时，子类在声明的时候，要指定出父类中T的类型，如果不指定，编译器无法给子类分配内存
 *       如果想灵活指定出父类中T的类型，子类也需变为类模板
 *
 *
 */
class Person
{
public:
    Person(int a, string n) : age(a), name(n) {}
    int age;
    string name;
    void show()
    {
        cout << "姓名---" << name << " 年龄---" << age << endl;
    }
};
// 5.1 类模板成员函数创建时机
template <typename T>
class MyClass
{
public:
    T obj; // 如果指定的参数没有默认构造函数，会立马报错
    void func()
    {
        obj.show();
    }
};

// 5.类模板
template <typename T, typename E>
class Animal
{
public:
    Animal(T n, E a) : name(n), age(a) {}
    T name;
    E age;
    void showInfo()
    {
        cout << "姓名 " << name << " 年龄  " << age << endl;
    }
};
// 5.2.1 显示指定类模板参数类型
// void printAnimal(Animal<string, int> &ani)
// {
//     cout << "---显示指定类模板参数类型---" << endl;
//     ani.showInfo();
// }
// 5.2.2 参数模板化
template <typename T, typename E>
void printAnimal(Animal<T, E> &ani)
{
    cout << "---参数模板化---" << endl;
    ani.showInfo();
    cout << "T的类型为---" << typeid(T).name() << endl;
    cout << "E的类型为---" << typeid(E).name() << endl;
}
// 5.2.3 整个类模板化
template <typename T>
void printAnimal(T &ani)
{
    cout << "---整个类模板化---" << endl;
    ani.showInfo();
}
template <typename T, typename E = double>
class Animal1
{
public:
    Animal1(T n, E a) : name(n), age(a) {}
    T name;
    E age;
    void showInfo()
    {
        cout << "姓名 " << name << " 年龄  " << age << endl;
    }
};

// 1.函数模版
template <typename T>
void mySwap(T &a, T &b) // 函数模板
{
    T c = a;
    a = b;
    b = c;
}
template <typename T>
void func()
{
    cout << "模版必须要确定T的数据类型" << endl;
}
template <typename T>
T addNum(T t1, T t2)
{
    return t1 + t2;
}
template <>
Person addNum(Person p1, Person p2)
{
    return Person(p1.age + p2.age, "person---3");
}
template <typename T>
void myPrint(T t1, T t2)
{
    cout << "函数模版---myPrint" << endl;
}
template <typename T>
void myPrint(T t1, T t2, T t3)
{
    cout << "函数模版重载---myPrint" << endl;
}
void myPrint(int a, int b)
{
    cout << "普通函数---myPrint" << endl;
}

template <unsigned M, unsigned N>                   // 非类型模板参数 非类型参数表示一个值而不是一个类型,值在编译的时候就能确定
int compare(const char (&p)[N], const char (&q)[M]) //
{
    if (N > M)
        return 1;
    if (N < M)
        return -1;
    return 0;
}
// 类模板------------------------------------------------------------------------------------------------------------------

int main()
{
    int a = 4, b = 5;
    cout << "交换前---"
         << "a = " << a << " b = " << b << endl;
    // 1.函数模版会自动类型推导
    mySwap(a, b);
    // 1.显示指定类型
    mySwap<int>(b, a);
    cout << "交换后---"
         << "a = " << a << " b = " << b << endl;
    cout << compare("13", "helo") << endl;
    // 1.模版必须要确定T的数据类型
    //  func();//没有确定类型T调用报错
    func<int>();
    // 3.普通函数与函数模版区别
    char c = 'c', d = 'd';
    // mySwap(a,c);//值传递报错，无法发生类型推导
    cout << addNum<int>(a, d) << endl;
    // 4.普通函数和函数模版的调用规则
    myPrint(10, 20); // 结果优先调用普通函数
    // 4.1 模版函数重载
    myPrint(10, 20, 30);
    // 4.2 如果函数模版可以发生更好的匹配优先调用函数模版
    myPrint('c', 'd'); // 如果发生隐式转化，编译器会优先调用模版，避免普通函数发生隐式转换
    // 4.3
    cout << "具体化模版---" << (Person(10, "123"), Person(20, "456")).age << endl; // 这是什么语法？？？为什么括号可以调用对象
    // 5.类模板
    Animal<string, int> ani("sd", 100);
    ani.showInfo();
    Animal1<string> ani1("类模板设置默认参数类型", 'c');
    ani1.showInfo();
    // 5.2
    printAnimal(ani);
    printAnimal(ani1);
    return 0;
}