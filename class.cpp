#include <iostream>
#include <string>
using namespace std;
/**
 * 1. struct和class区别
 *    在C++中 struct和class唯一的区别就在于 默认的访问权限不同,struct 默认权限为公共、class 默认权限为私有
 * 2. 类访问修饰符
 *    public private protect. 如果不设置访问修饰符，创建对象后，vscode能给你补全属性，但是读取和写入都会报错
 * 3. 对象初始化和清理
 *    c++会自动创建构造函数和析构函数并在初始化和销毁的时候调用它们，不过编译器提供的构造函数和析构函数是空实现
 *    构造函数语法：类名(){}
 *    析构函数语法：~类名(){} 析构函数不可以有参数，因此不可以发生重载
 *    3.1 构造函数种类
 *        有参构造和无参构造、普通构造和拷贝构造
 *    3.2 调用构造函数
 *        Box b 调用无参构造函数  Box b(...)括号法调用有参构造函数 Box b = Box(...)显示法调用有参构造函数，括号法就是显示法 Box b=10 隐式法调用有参构造函数
 *        注意调用无参构造函数不要加小括号 Box b() 这样会让编译器认为这是在声明一个方法
 *    3.3 匿名对象
 *        Box() 没有赋值给变量，执行后会马上被回收； 不要用 拷贝构造函数创建匿名对象，编译器会把括号去掉例如
 *        Box b ; Box(b) 等价于 Box b； 相当于重复变量名了，编译器会报错
 *    3.4 拷贝构造函数的调用时机
 *        函数值传递例如 void XXX(Box b) 传入一个Box对象会发现拷贝构造函数被调用了，实际上形参在这里复制了实参，所以值传递不会改变实参的值
 *        值方式返回局部对象
 *        Box XXX(){
 *          Box b;
 *          return b;//这里其实并不会返回局部对象而是根据对象b创建了新的对象返回,调用了拷贝构造函数。实测发现没有调用拷贝，但是视频里调用了，未解之谜
 *       }
 *    3.5 构造函数定义规则
 *        如果用户定义有参构造函数，c++不在提供默认无参构造，但是会提供默认拷贝构造
 *        如果用户定义拷贝构造函数，c++不会再提供其他构造函数
 * 4. 深拷贝和浅拷贝
 *    编译器提供的拷贝构造函数叫做浅拷贝，拷贝原理，把对象1的属性值复制给对象2的属性，但是该李忠height是堆区，当对象1执行析构函数的时候
 *    对象2也执行析构函数，这时候由于对象1的析构函数已经释放了，所以对象2再释放就会报错。这就是浅拷贝带来的问题，堆区内存重复释放
 *    为了解决这个问题，重新写一个拷贝构造函数,重新申请一个堆区空间，这种情况叫深拷贝
 *    总结：如果属性有在堆区开辟的，一定要自己提供拷贝构造函数，防止浅拷贝带来的问题
 * 5. 初始化列表
 *    初始化列表赋默认值
 * 6. 类对象作为类成员
 *    先构造类对象，再构造成员对象
 * 7. 静态成员
 *    所有对象共享同一份数据、在编译阶段分配内存、类内声明，类外初始化
 *    7.1 静态属性不能在类内初始化，必须要在类外初始化 类型 类::属性=value,静态成员也是有访问权限的，private 无法访问
 *    通过变量访问静态成员，通过类名访问静态成员 Orange::name
 *    7.2 静态成员函数只能访问静态成员变量,静态函数不需要类外定义， 通过变量访问静态函数，通过类名访问静态成员 Orange::display()
 * 8. C++对象模型
 *    不含任何成员的对象占据1字节空间。是为了标记空对象在空间中的位置。
 *    静态成员变量不属于对象，sizeof得到大小不包括静态成员变量大小，非静态成员函数也不属于对象,不占据空间
 *    成员变量、成员函数是分开存储的
 *    sizeof 匿名对象 空间都是1字节，这是为什么？？？？
 * 9. this指针
 *    this指针是隐含每一个非静态成员函数内的一种指针，this指针不需要定义，直接使用即可
 *    空指针调用成员函数，调用showAttr时候报错了，这是因为name完整调用形态是this->name ,由于对象指向空指针，所以这里报错了
 *    showFuction就没有报错，它没有使用this
 *
 *
 *
 *
 *
 */
class NullPoint
{
public:
    void showFuction()
    {
        cout << "showFuction---" << endl;
    }
    void showAttr()
    {
        cout << "showAttr---" << name << endl;
    }
    string name;
};
class Original
{
};
class Original_1
{
    // 8. C++对象模型和this指针
    int m_a;
    static int m_b;          // 不属于对象
    int func() { return 1; } // 非静态成员函数也不属于对象,不占据空间
};
int Original_1::m_b = 10;
class Orange
{
    // 7.静态成员
public:
    static string name;
    int weight = 10;
    static void display()
    {
        cout << "显示名字---" << name << endl;
    }
};
string Orange::name = "我是橘子";

class Apple
{
    string name;

public:
    Apple(string name)
    {
        this->name = name;
    }
};
//
class Triangle
{
    int side1;
    int side2;
    int side3;

public:
    // 5.初始化列表---给属性赋默认值
    Triangle() : side1(30), side2(40), side3(50) {}
    // 5.初始化列表
    Triangle(int a, int b, int c) : side1(a), side2(b), side3(c) {}
};

class Rectangle
{
    int width;
    int *height;
    // 6. 类对象作为类成员
    Triangle t;
    // Apple a;//对于没有默认构造函数的成员我在下面写了 赋值构造函数还是不行，和视频讲的不一样

public:
    Rectangle()
    {
        cout << "Rectangle无参构造函数" << endl;
    }
    // Rectangle(string name):a(name){}
    Rectangle(int width, int height)
    {
        this->width = width;
        this->height = new int(height); // 堆区
        cout << "Rectangle有参构造函数" << endl;
    }
    // 4.重新申请一个堆区空间，深拷贝
    Rectangle(const Rectangle &r)
    {
        // 编译器浅拷贝默认做的事
        // this->width = r.width;
        // this->height = r.height；

        // 深拷贝，重新申请堆区
        this->width = r.width;
        this->height = new int(*r.height);
    }
    ~Rectangle()
    {
        // 析构函数的作用就是在释放在堆区开辟的空间
        if (height != NULL)
        {
            delete height;
            height = NULL;
        }
        cout << "Rectangle析构函数" << endl;
    }
};

class Box
{
public:
    // 3.1 构造函数种类
    Box()
    {
        cout << "无参构造函数!" << endl;
    } // 无参构造函数
    Box(int height, int length, int breath)
    {
        this->height = height; // this 是指针
        this->length = length;
        this->breath = breath;
        cout << "有参构造函数!" << endl;
    } // 有参构造函数
    ~Box()
    {
        cout << "析构函数!" << endl;
    } // 析构函数，每次删除锁创建对象时调用 （delete 对象 即删除对象）
    Box(const Box &obj)
    {
        height = obj.height;
        this->length = obj.length;
        this->breath = obj.breath;
        cout << "拷贝构造函数!" << endl;
    } // 拷贝构造函数：如果在类中没有定义拷贝构造函数，编译器会自行定义一个。如果类带有指针变量，并有动态内存分配，则它必须有一个拷贝构造函数。拷贝构造

    int height;
    int length = 10;
    int breath;
    int getLength(void);
    void setLength(int length);
    static int objCount; // 在创建第一个对象时，所有的静态数据都会被初始化为零。我们不能把静态成员的初始化放置在类的定义中
    int getBreath(void)  // 在类内部定义的函数都是内联函数，不必专门写关键字inline；内联函数的好处是加快运行速度，原理用空间换时间。所以一般都是小函数
    {
        this->breath; // this 指针：每一个对象都能通过 this 指针来访问自己的地址。this 指针是所有成员函数的隐含参数
        return breath;
    }
    static int getCount()
    {
        return objCount; // 静态成员函数没有 this 指针，只能访问静态成员
    }
    Box operator-() // 一元运算符
    {
        length = -length;
        return *this;
    }
    Box operator+(const Box &box) // 二元运算符重载
    {
        Box b;
        b.breath += box.breath;
        b.length += box.length;
        b.height += box.height;
        b.noWay += box.noWay;
        return b;
    }

protected: // 与private很相似，在派生类中可以访问
private:   // 成员和类的默认访问修饰符是 private；只能被本类成员（类内）和友元访问，不能被派生类访问；
    int noWay = 10;
    int *ptr;
    friend void printnoWay(Box box); // 友元函数，可以访问类的private 和 proteced 成员，他们不是类成员，在这里类似于声明一个接口
    friend class BoxFriend;          // 友元函数、类声明在哪里无所谓
    // friend void BoxFriend::printNoway(); //指定一个类中的函数为友元函数
};
Box doWork2()
{
    Box b;
    cout << "返回对象的地址----" << &b << endl;
    return b;
}
void printnoWay(Box box)
{
    cout << "直接访问私有属性noWay=" << box.noWay << endl;
}
void Box::setLength(int len) // 成员函数定义无法像java一样自动生成
{
    length = len;
}
int Box::objCount = 10; // 静态成员在外部初始化
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
    // Rectangle r ;
    // // r.width = 100;
    // cout<<"三角形宽---"<<r.width<<endls;
    // 3.1 构造函数种类
    Box b;                 // 无参构造函数，
    Box b5();              // 默认构造函数不要加括号，编译器会认为这是在声明一个方法
    Box bb(100, 101, 102); // 有参构造函数
    Box bbb(b);            // 拷贝构造函数
    // 3.2 调用构造函数
    Box b2;         // 显示法调用午餐构造函数
    Box b3 = Box(); // 显示法、括号法 调用无参构造函数
    Box b4 = b;     // 隐式法调用有参构造函数，这里调用的是拷贝构造函数
    // 3.2 匿名对象
    Box(); // 匿名对象，没有变量名，执行完后会被马上回收
    cout << "-----拷贝构造函数调用时机-----" << endl;
    Box b6 = doWork2(); // 实测发现没有像视频里说的那样调用了拷贝构造函数，局部变量被返回出来了，和视频讲的不一样
    cout << "b6地址----" << &b6 << endl;
    cout << "b6地址----" << &b6 << endl;
    // 4.深拷贝和浅拷贝
    Rectangle r(100, 102); //
    Rectangle r1 = r;      // 把自定义拷贝构造函数，注释掉就可以看到，浅拷贝的错误
    // 5. 初始化列表
    Triangle t;
    Triangle t1(10, 20, 30);
    // 6.
    //  Rectangle("123");
    // 7. 静态成员
    cout << "通过对象访问静态成员---" << Orange().name << endl;
    cout << "通过类名访问静态成员---" << Orange::name << endl;
    Orange::display();
    // 8.
    Original ori;
    cout << "一个不含任何成员的对象占据多大空间---" << sizeof(ori) << endl;
    Original_1 o1;
    cout << "有成员的对象占据多大空间---" << sizeof(o1) << endl;
    // 9.this指针
    NullPoint *np = NULL;
    np->showFuction();
    // np->showAttr();//该方法使用了this 又因为这里的this是空指针所以报错


    
    // cout << "Inital Stage Count: " << Box::getCount() << endl; // 静态成员函数即使在类对象不存在的情况下也能被调用，静态函数只要使用类名加范围解析运算符 :: 就可以访问。
    // Box box;                                                   // 声明一个对象,如果默认构造函数只声明不定义会报错
    // Box copy = box;                                            // 调用拷贝构造函数
    // box.printnoWay(box);
    // cout << "Final Stage Count: " << Box::getCount() << endl;
    // Box b2 = box + copy; // 操作符重载
    // -b2;
    // cout << "重载后: " << b2.length << endl;
}