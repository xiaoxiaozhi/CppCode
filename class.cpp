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
 *    3.2 构造函数调用方式
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
 *    this指针的本质是一个指针常量，指针的指向不可修改
 *    this指针是隐含每一个非静态成员函数内的一种指针，this指针不需要定义，直接使用即可
 *    空指针调用成员函数，调用showAttr时候报错了，这是因为name完整调用形态是this->name ,由于对象指向空指针，所以这里报错了
 *    showFuction就没有报错，它没有使用this
 * 10.const修饰成员函数
 *    成员函数后加const后我们称为这个函数为常函数、常函数内不可以修改成员属性、成员属性声明时加关键字mutable后，在常函数中依然可以修改
 *
 *    10.1 增加mutable关键字，让常函数修改普通变量
 *    10.2 常对象：声明对象前加const称该对象为常对象、常对象只能调用常函数
 *         必须显示调用构造函数，否则报错
 *         ConstClass const cc = ConstClass(); 也可以  const ConstClass cc = ConstClass(); 二者等价
 * 11.友元
 *    11.1 全局函数做友元，在类最上面 friend 全局函数声明 friend void googGay(Building &building);
 *    11.2 类做友元 ，在类最上面 friend 类
 *    11.3 其他类成员函数做友元
 * 12.运算符重载
 *    对于内置数据类型编译器知道怎么使用运算符，但是对自定义类型怎么使用运算符呢？这时候就要用到运算符重载，键盘敲单词 operater(编译器会自动补全一大堆运算符)
 *    成员函数重载    Rectangle operator+(const Rectangle &rectangle)
 *    全局函数重载    Rectangle operator+(const Rectangle &r1, const Rectangle &r2)
 *    注意const不加会报错
 * 13.继承
 *    减少重复代码，子类也称为派生类，父类也叫基类。继承语法   class 子类 : 继承方式(public private protected) 父类
 *    13.1继承方式：public公共继承，继承到父类的 public和protected属性方法变成子类的公共和保护
 *             protected保护继承，继承到父类的 public和protected属性方法到了子类变成子类的保护权限
 *             private私有继承，父类中public和protected到了子类都变成子类的私有权限，子类可以访问这些改变了权限的方法和属性
 *    父类私有权限的属性和方法，到了子类还是私有的。子类访问不到父类隐私属性和方法
 *    保护权限类外访问不到
 *    13.2继承中的对象内存模型：父类3个int属性，子类1个int属性，子类对象总共占16字节。父类所有的非静态属性都被子类继承了，
 *        访问不到父类私有属性是因为被编译器隐藏了
 *    13.3继承中构造函数和析构函数执行顺序 Base无参构造函数、Son无参构造函数、Son析构函数、Base析构函数。 先调用父类构造再调用子类，回收先调用子类析构，再调用父类
 *        如果父类构造和析构都是私有属性，那么子类创建和销毁时就无法调用。注意父类构造和析构不能是私有
 *    13.4继承中处理同名：子类中同名成员直接访问，父类中同名成员加作用域访问。 s.m_A和s.Base::m_A。函数的调用方式和属性一致
 *        如果父类同名函数有重载出现，子类调用重载函数出错，给我的感觉是，对于重名，子类.属性和方法 就是调用子类的，子类.父类::属性和方法就是调用父类的，用作用域来区分同名
 *    13.5同名静态成员变量：同上  通过类名访问静态变量 Son::m_A  Son::Base::m_A
 *    13.6多继承语法：允许有多个父类  class 子类 :继承方式 父类1 ， 继承方式 父类2。由于容易引发同名问题，开发中不建议多继承
 *    如果不重名，子类可以直接调用父类属性和方法,不用加作用域
 *    13.7 菱形继承
 *    A--->(B,C)--->D   B,C继承了A，D又多继承了B,C。菱形继承会产生一个问题，比如age属性，A类存在一个age属性，B和C继承了A，所以B和C各自都有age属性
 *    这时候D继承了B和C导致D有两份age。然后我们对age赋值，D.B::age=10  D.C::age=20 。D没有专属age，只能对父类的age赋值。怎么解决这个问题？
 *    增加关键字 virtual 利用虚继承解决菱形问题，class B: virtual public A 、class C: virtual public A
 *    看下图 菱形继承与菱形虚继承对象模型区别
 *    http://m.qpic.cn/psc?/V54UN84b0OHfN43eIX713mRT5H07gkzM/bqQfVz5yrrGYSXMvKr.cqYyroh.Mij3fXp8kBg0aLAiyFE0*8y3Q9BIDO6ZZUdgWx8L9whheYhviz4co463V6FpZliYLlzm7JNAGfIhS*NE!/b&bo=Dwd5AgAAAAABB1M!&rf=viewer_4
 *    vbptr 虚基类指针 指向vbtable(virtual base table).B和C通过虚继承，内部不再赋值A类的属性，而是生成了一个虚基类指针指向A类的属性。
 *    这样就只有一份age。 D.B::age=10  D.C::age=20 修改的是同一份age。
 * 14.多态
 *    多态分为两类
 *    静态多态: 函数重载 和 运算符重载属于静态多态，复用函数名，同一函数名不同参数参数
 *    动态多态: 派生类和虚函数实现运行时多态
 *    静态多态和动态多态区别：
 *    静态多态的函数地址早绑定 - 编译阶段确定函数地址         void speak()
 *    动态多态的函数地址晚绑定 - 运行阶段确定函数地址 virtual void speak()
 *    多态 1.有继承关系 2.子类必须重写父类的虚方法
 *    14.1 多态内部原理
 *    virtual void speak() 虚函数会在类内部有一个生成一个四字节的指针 vfptr  virtual function pointer 虚函数指针指向虚函数表 vftable
 *    如果子类重写了父类的虚函数，子类这样不叫重写虚函数void speak()，这样才叫  virtual void speak()。子类虚函数表的speak会替换成子类的speak虚函数
 *    动态多态 Animal &animal =sheep; animal.speak; 调用的是小羊的speak animsl.speak();
 *    14.2 纯虚函数和抽象类
 *    在多态中，通常父类中虚函数的实现是毫无意义的，主要都是调用子类重写的内容因此可以将虚函数改为纯虚函数
 *    纯虚函数语法：virtual 返回值类型 函数名 （参数列表）= 0 ; virtual void func()=0; vit当类中有了纯虚函数，这个类也称为抽象类
 *    14.3 虚析构函数和纯析构函数
 *    多态使用时，如果子类中有属性开辟到堆区，那么父类指针在释放时无法调用到子类的析构代码
 *    解决方式：将父类中的析构函数改为虚析构或者纯虚析构
 *    虚析构和纯虚析构共性：可以解决父类指针释放子类对象都需要有具体的函数实现
 *    虚析构和纯虚析构区别：如果是纯虚析构，该类属于抽象类，无法实例化对象
 *    虚析构语法：virtual ~类名(){}
 *    纯虚析构语法：virtual ~类名() = 0; 类外再实现 类名::~类名(){}。纯虚析构有点麻烦类内声明一个纯虚析构，类外还要再实现
 *    Fruit *f = new Cherry(); delete f;//只会调用fruit的析构函数，Fruit析构函数前加virtual变成虚析构之后，就可以调用子类Cherry的析构函数了
 *    有个问题main函数中其他的 对象都没有调用delete最后都调用了析构函数，怎么到了虚析构函数，就要调用delete才能执行析构函数？？？
 */

class Fruit
{
public:
    virtual void func() = 0;
    virtual ~Fruit()
    {
        cout << "水果调用了析构函数" << endl;
    }
    //使用纯虚析构效果相同
    // virtual ~Fruit() = 0;
};
// Fruit::~Fruit(){}

class Cherry : public Fruit
{
    int *hex;

public:
    Cherry()
    {
        hex = new int(10);
    }
    // 如果不覆写纯虚函数，Cherry也是一个抽象函数，下文 new Cherry()的就会报错，因为抽象类不能实例化
    void func()
    {
        cout << "樱桃继承了纯虚函数" << endl;
    }
    ~Cherry()
    {
        cout << "樱桃调用了析构函数" << endl;
        if (hex != NULL)
        {
            delete hex;
            hex = NULL;
        }
    }
};

class Animal
{
public:
    int age;
    // 静态多态的函数地址早绑定 - 编译阶段确定函数地址
    // 动态多态的函数地址晚绑定 - 运行阶段确定函数地址
    virtual void speak()
    {
        cout << "动物在说话" << endl;
    }
};
class Sheep : virtual public Animal
{
public:
    void speak()
    {
        cout << "小羊在说话" << endl;
    }
};
class Tuo : virtual public Animal
{
public:
    void speak()
    {
        cout << "骆驼在说话" << endl;
    }
};
// 我们希望传入什么对象，那么就调用什么对象的函数
// 如果函数地址在编译阶段就能确定，那么静态联编
// 如果函数地址在运行阶段才能确定，就是动态联编
void doSpeak(Animal &animal)
{
    animal.speak();
}
// 13.7 菱形继承
class SheepTuo : public Sheep, public Tuo
{
public:
    void speak()
    {
        cout << "羊驼在说话" << endl;
    }
};
class Base
{
public:
    int m_AA = 10;
    int m_A = 100;

protected:
    int m_B;
    Base()
    {
        cout << "Base无参构造函数" << endl;
    }
    ~Base()
    {
        cout << "Base析构函数" << endl;
    }

private: // 私有成员只是被隐藏了，但是还是会继承
    int m_C;
};

// 公共继承
class Son : public Base
{
public:
    Son()
    {
        cout << "Son无参构造函数" << endl;
    }
    ~Son()
    {
        cout << "Son析构函数" << endl;
    }
    int m_A = 200;
    int m_D;
};
class GoodGay;
class Building;
class GoodGayA
{
public:
    void visit(Building &building); // Building类还没有定义这时候只能接收一个Building对象而不能自己创建，感觉可以先声明，里面的函数在之后实现
};
class Building
{
    // 11.1 告诉编译器 goodGay全局函数 是 Building类的好朋友，可以访问类中的私有内容
    friend void googGay(Building &building);
    // 11.2
    friend class GoodGay;
    // 11.3
    friend void GoodGayA::visit(Building &building); // 在类外声明
    // 11.友元
public:
    Building() : settingRoom("客厅"), bedRoom("卧室") {}
    string settingRoom;

private:
    string bedRoom;
};
// 全局函数
void googGay(Building &building)
{
    cout << "好基友函数正在访问私有属性---" << building.bedRoom << endl;
}
class GoodGay
{
    Building building;

public:
    void visit()
    {
        cout << "好基友GoodGay类正在访问私有属性---" << building.bedRoom << endl;
    }
};

void GoodGayA::visit(Building &building)
{
    cout << "好基友GoodGayA类正在访问私有属性---" << building.bedRoom << endl;
}

class ConstClass
{
public:
    // 10.const修改成员函数
    void doWork() const
    {
        // this->m_A;//this指针的本质是一个指针常量,指针的指向不可修改 但是值可以修改。常函数实际上又给函数体内的this加了一个const，
        // 让this指针变成，指针常量+常量指针，这样值也不能修改
        // m_A = 10;//常函数不可以修改成员属性
        m_B = 10; // 10.1
        cout << "do something" << endl;
    }
    int m_A;
    mutable int m_B;
};
class NullPoint
{
public:
    void showFuction()
    {
        cout << "showFuction---" << endl;
    }
    void showAttr()
    {
        if (this == NULL)
        {
            return; // 添加判空增强健壮性
        }
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

public:
    int width;
    int *height;
    // 6. 类对象作为类成员
    Triangle t;
    // Apple a;//对于没有默认构造函数的成员我在下面写了 赋值构造函数还是不行，和视频讲的不一样
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
    // 12.成员函数运算符重载
    Rectangle operator+(const Rectangle &rectangle)
    {
        Rectangle r;
        r.width = this->width + rectangle.width;
        // r.height = new int(*height + *rectangle.height);//height是指针，没有初始化就没法使用，指针初始化赋值地址、直接传值会报错
        r.height = new int(*height + *rectangle.height);
        return r;
    }
    void display()
    {
        cout << "width---" << this->width << " height---" << *this->height << endl;
    }
};
// 12.全局函数运算符重载
Rectangle operator+(const Rectangle &r1, const Rectangle &r2)
{
    Rectangle r;
    r.width = r1.width + r2.width;
    // r.height = new int(*height + *rectangle.height);//TODO 为什么一定要创建堆 对象，两个值相加怎么不行
    r.height = new int(*r1.height + *r2.height);
    return r;
}

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
    // 3.2 构造函数调用方式
    Box b1(100, 101, 102); // 括号法
    // Box b11();          // 无参构造函数不能用括号法，会被当做方法声明
    Box b2 = Box(b1); // 显示法 调用无参构造函数
    Box b3 = Box();   // 显示法 调用无参构造函数

    Box b4 = b; // 隐式法调用有参构造函数，这里调用的是拷贝构造函数
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
    np->showAttr(); // 该方法使用了this 又因为这里的this是空指针所以报错，然后加个判空增加程序健壮性
    // 10.2 常对象
    ConstClass const cc = ConstClass();
    cc.m_B = 100;
    cout << "常对象修改mutable变量---" << cc.m_B << endl;
    // 11.1 全局函数做友元
    Building build;
    googGay(build);
    GoodGay().visit();       // 11.2
    GoodGayA().visit(build); // 11.3
    // 12。运算符重载
    Rectangle rr = Rectangle(10, 10) + Rectangle(20, 20);
    rr.display();
    // 13.2 继承的内存模型
    Son s;
    cout << "子类的大小是多少---" << sizeof(s) << endl;   //
                                                          // 13.4继承中处理同名
    cout << "访问子类同名属性---" << s.m_A << endl;       //
    cout << "访问父类同名属性---" << s.Base::m_A << endl; //
    // 13.7 菱形继承 ---虚继承
    SheepTuo st;
    st.age = 100; // 在虚基类继承前，该属性不能直接引用，因为不明确，要加上作用域
    st.Sheep::age = 101;
    st.Tuo::age = 102;
    cout << "虚基类继承 age = " << st.age << endl; // st.Sheep::age 、 st.Tuo::age 、  st.age 引用的是一个值
    // 14.多态
    Sheep sheep;
    doSpeak(sheep);
    // 14.2 纯虚函数和抽象类
    Fruit *f = new Cherry();
    f->func();
    delete f; // 只会调用fruit的析构函数，加virtual变成虚析构之后，就可以调用子类Cherry的析构函数了

    // cout << "Inital Stage Count: " << Box::getCount() << endl; // 静态成员函数即使在类对象不存在的情况下也能被调用，静态函数只要使用类名加范围解析运算符 :: 就可以访问。
    // Box box;                                                   // 声明一个对象,如果默认构造函数只声明不定义会报错
    // Box copy = box;                                            // 调用拷贝构造函数
    // box.printnoWay(box);
    // cout << "Final Stage Count: " << Box::getCount() << endl;
    // Box b2 = box + copy; // 操作符重载
    // -b2;
    // cout << "重载后: " << b2.length << endl;
}