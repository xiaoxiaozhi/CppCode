#include <iostream>
using namespace std;
/**
 * 1. struct和class区别
 *    在C++中 struct和class唯一的区别就在于 默认的访问权限不同,struct 默认权限为公共、class 默认权限为私有
 * 2.
*/


//1.类访问修饰符
//2.拷贝构造函数
//3.友元函数
//4.内联函数、this指针、静态成员
//5.重载
// class BoxFriend;
class Box
{

public:
    int height;
    int length = 10;
    int breath;
    int getLength(void);
    void setLength(int length);
    static int objCount; //在创建第一个对象时，所有的静态数据都会被初始化为零。我们不能把静态成员的初始化放置在类的定义中
    int getBreath(void)  //在类内部定义的函数都是内联函数，不必专门写关键字inline；内联函数的好处是加快运行速度，原理用空间换时间。所以一般都是小函数
    {
        this->breath; //this 指针：每一个对象都能通过 this 指针来访问自己的地址。this 指针是所有成员函数的隐含参数
        return breath;
    }
    static int getCount()
    {
        return objCount; //静态成员函数没有 this 指针，只能访问静态成员
    }
    Box operator-() //一元运算符
    {
        length = -length;
        return *this;
    }
    Box operator+(const Box &box) //二元运算符重载
    {
        Box b;
        b.breath += box.breath;
        b.length += box.length;
        b.height += box.height;
        b.noWay += box.noWay;
        return b;
    }
    Box();                                   //构造函数
    Box(int height, int length, int breath); //带参数的构造函数
    ~Box();                                  //析构函数，每次删除锁创建对象时调用 （delete 对象 即删除对象）
    Box(const Box &obj);                     //拷贝构造函数：如果在类中没有定义拷贝构造函数，编译器会自行定义一个。如果类带有指针变量，并有动态内存分配，则它必须有一个拷贝构造函数。拷贝构造

protected: //与private很相似，在派生类中可以访问
private:   //成员和类的默认访问修饰符是 private；只能被本类成员（类内）和友元访问，不能被派生类访问；
    int noWay = 10;
    int *ptr;
    friend void printnoWay(Box box); //友元函数，可以访问类的private 和 proteced 成员，他们不是类成员，在这里类似于声明一个接口
    friend class BoxFriend;          //友元函数、类声明在哪里无所谓
    // friend void BoxFriend::printNoway(); //指定一个类中的函数为友元函数
};
Box::Box()
{
    objCount++;
}
Box::Box(int height, int length, int breath) : height(height), length(length), breath(breath) //初始化列表初始化字段
{
    // 为指针分配内存
    ptr = &height;
}
Box::~Box()
{
    cout << "对象销毁时执行" << endl;
}
Box::Box(const Box &obj) //可以自定义拷贝构造函数
{
    // noWay = obj.breath;//随便赋值
    cout << "1调用拷贝构造函数" << endl;
}
void printnoWay(Box box)
{
    cout << "直接访问私有属性noWay=" << box.noWay << endl;
}
void Box::setLength(int len) //成员函数定义无法像java一样自动生成
{
    length = len;
}
int Box::getLength() //在类外部用范围解析运算符 :: 定义函数
{
    return 10;
}
int Box::objCount = 10; //静态成员在外部初始化
class SmallBox : public Box
{
};
class BoxFriend
{
public:
    void printNoway()
    {
        Box box;
        box.noWay;
    }
};
int main()
{
    cout << "Inital Stage Count: " << Box::getCount() << endl; //静态成员函数即使在类对象不存在的情况下也能被调用，静态函数只要使用类名加范围解析运算符 :: 就可以访问。
    Box box;                                                   //声明一个对象,如果默认构造函数只声明不定义会报错
    Box copy = box;                                            //调用拷贝构造函数
    printnoWay(box);
    cout << "Final Stage Count: " << Box::getCount() << endl;
    Box b2 = box + copy; //操作符重载
    -b2;
    cout << "重载后: " << b2.length << endl;
}