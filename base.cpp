#include <iostream>
#include <string.h>
using namespace std;
// 【黑马程序员匠心之作|C++教程从0到1入门编程,学习编程不再难】 https://www.bilibili.com/video/BV1et411b73Z/?p=4&share_source=copy_web&vd_source=d858c6ae113d28a5ee3e69f10b5d3b41
// 1. C++定义常量两种方式
//    1.1 #define 宏常量： #define 常量名 常量值 通常在文件上方定义，表示一个常量
//    1.2 const修饰的变量 const 数据类型 常量名 = 常量值 通常在变量定义前加关键字const，修饰该变量为常量，不可修改
// 2. 数据类型
//    2.1 short(短整型)	2字节 int 4字节 long Windows为4字节，Linux为4字节(32位)，跟int好像没区别，long long 8字节
//    2.2 sizeof关键字 sizeof( 数据类型 / 变量)
//    2.3 float	    4字节	7位有效数字
//        double	8字节	15～16位有效数字
//        有效数字是指在一个数中，从该数的第一个非零数字起，直到末尾数字止的数字称为有效数字
//        浮点数默认都是 double类型  0.1f加f转为单精度类型
//    2.4 字符型char ch = 'a';用单引号将字符括起来，不要用双引号，只能有一个字符,字符型变量只占有1字节
//    2.5 转义字符用于表示不能显示出来的字符 \t \n \\等等
//    2.6 字符串，有两种风格
//        C     风格 char 变量名[] = "字符串值", 再次对字符串赋值就不能这样了 用字符串处理函数 strcpy();赋值超过初始化时的长度，奇怪的是mingw64可以通过编译，msys2不可以，网络搜索也不可以
//        C++   风格  string 变量名 = "字符串值" 注意要引入 #include <string.h>
//    2.7 布尔类型：bool类型只有两个值： 占用一个字节
//        true --- 真（本质是1）
//        false --- 假（本质是0）
//    2.8 一维数组
//        char buf[1024] = { 0 };数组初始化方式,每个元素都为0; 另一种写法效果等价  char buf[1024] = { };
//        数据类型 数组名[ 数组长度 ];
//        数据类型 数组名[ 数组长度 ] = { 值1，值2 ...};
//        数据类型 数组名[ ] = { 值1，值2 ...};
//        实践发现 (int)&arr[0] 会报错，int* 转 int 会丢失精度，sizeOf(&arr[0])发现是8字节，应该转成long long或者使用printf函数, uintptr_t 用于指针地址类型与整形转换
//    2.9 二维数组
//        数据类型 数组名[ 行数 ][ 列数 ];
//        数据类型 数组名[ 行数 ][ 列数 ] = { {数据1，数据2 } ，{数据3，数据4 } };
//        数据类型 数组名[ 行数 ][ 列数 ] = { 数据1，数据2，数据3，数据4};
//        数据类型 数组名[ ][ 列数 ] = { 数据1，数据2，数据3，数据4};
//--------以下内容来自网络----------
// 3. 变量初始化
// int i 声明int变量时不是必须要进行初始化的,如果变量未被初始化，它将包含一个未知的值，这可能会导致不可预测的行为,做好初始化
// 在C++11之后的标准中，引入了统一的初始化语法 int i{}; i值为0
//
//c 标准库处理字符串的函数
// strcpy()  strncpy(dest, src, 5);  size_t len = strlen(str);  // 获取字符串的长度   未完待总结
//
// 1.1
#define day 7
int main()
{
    //--------------------黑马程序员-------------------------
    // 1.
    cout << "define 常量 ---" << day << endl;
    const int cconst = 1;
    cout << "const 常量---" << cconst << endl;
    // 2.
    cout << "sizeOf---long " << sizeof(long) << endl;
    cout << "sizeOf---long long " << sizeof(long long) << endl;
    cout << "sizeOf---int变量 " << sizeof(cconst) << endl;
    // 2.3
    cout << "科学计数法---" << 2e2 << endl;  // e * 10 ^ 2
    cout << "科学计数法---" << 2e-2 << endl; // e * 0.1 ^ 2
    // 2.4
    char ch = 97;
    cout << "直接用ASCII码给字符型变量赋值---" << ch << endl;
    // 2.6
    char str1[] = "hello world"; // C风格要用 双引号括起来
    // strcpy(str1,"你是知道的12345678909876543210123456789");//"C:\\Program Files\\mingw64\\bin\\gdb.exe",可以通过编译其他不行
    // printf("C风格字符串1---%s\n",str1);
    cout << "C风格字符串1---" << str1 << endl;
    //  char* str2 = "char* str2";//它表示在试图将一个字符串常量转换为char*类型的指针时发生了错误,可以使用const char*类型的指针来指向字符串常量
    const char *str2 = "char* str2"; //
    cout << "C风格字符串2---" << str2 << endl;
    printf("C风格字符串str1长度%d\n", strlen(str1));
    printf("C风格字符串str2长度%d\n", strlen(str2));
    string str = "hello world";
    cout << "C++风格字符串---" << str << endl;
    // 2.8
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    cout << "整个数组所占内存空间为： " << sizeof(arr) << endl;
    cout << "每个元素所占内存空间为： " << sizeof(arr[0]) << endl;
    cout << "数组的元素个数为： " << sizeof(arr) / sizeof(arr[0]) << endl;
    // 、可以通过数组名获取到数组首地址
    // printf("数组首地址为：%d\n", arr);
    cout << "数组首地址为：" << (uintptr_t)arr << endl;
    cout << "数组中第一个元素地址为： " << (long long)&arr[0] << endl;
    cout << "数组中第二个元素地址为： " << (long long)&arr[1] << endl;
    // 2.9 二维数组
    int arr1[2][3] = {{1, 2, 3}, {4, 5, 6}};
    cout << "二维数组大小： " << sizeof(arr1) << endl;
    cout << "二维数组一行大小： " << sizeof(arr1[0]) << endl;
    cout << "二维数组元素大小： " << sizeof(arr1[0][0]) << endl;
    cout << "二维数组行数： " << sizeof(arr1) / sizeof(arr1[0]) << endl;
    cout << "二维数组列数： " << sizeof(arr1[0]) / sizeof(arr1[0][0]) << endl;
    // 地址
    cout << "二维数组首地址：" << arr1 << endl;
    cout << "二维数组第一行地址：" << arr1[0] << endl;
    cout << "二维数组第二行地址：" << arr1[1] << endl;
    cout << "二维数组第一个元素地址：" << &arr1[0][0] << endl;
    cout << "二维数组第二个元素地址：" << &arr1[0][1] << endl;

    // 引用-------------------------------------------------------------
    int a = 1;  // 与a相比下面 两个变量属于复合类型（类就修饰符+变量名）
    int &n = a; // 给a取一个别名（&属于声明的一部分 int &）(类型修饰符 * &)
    // cout << "a的别名&n=" << n << endl;
    cout << "调用别名 n = " << n << endl;
    // int &m;   //报错，别名必须在定义时初始化
    int *p = &a; // 指针（*属于声明的一部分 int &）(类型修饰符 * &)
    int *&r = p; // 指向指针的引用 从右向左读，有助于理解这个复杂的类型

    int &&x = 5; // 上面都是左值引用现在是右值引用, int &x=5 不允许这样做，所以就有了 int &&x = 5
    cout << "右值引用" << x << endl;
    // // const---------------------------------------------------------------
    int b = 10;
    const int &c = b; // 对常量的引用，不能修改引用变量的值
    const int &d = c * 2;
    // int &e = c * 2; //报错 如果引用右侧是一个表达式，就会创建一个临时变量，
    // 实际上是&e = temp，引用目的是为了改变被引用对象的值，但temp对应一个表达式，此时引用的目的消失了，所以C++不允许这种操作
    const double pi = 3.14;
    // double *p = &pi;//错误，p是一个普通指针
    // const double *p = &pi; //指针常量才能指向常量， *p 不能赋值 但是可以指向另一个 变量
    p = &b; // 指针常量指向另一个变量

    int *const curErr = &b; // 指针是一个常量，不能改变指向；
    *curErr = 10;           // 但是能改变值

    const int max = 20; // 常量表达式，由数据类型和初始值（编译时段就能确定）共同决定
    // const int temp = getSize()//不是一个常量表达式，因为初始值要到运行时段才能确定
    int min = 10;          // 不是常量表达式，数据类型不是const
    constexpr int mf = 20; // C++ 11新标准  constexpr 方便编译器检验是否是一个常量表达式
    // constexpr int mf = getSize(); //如果getSize()是一个常量函数，那它就是一个常量表达式
    const int *pp = nullptr; // nullptr是c++中空指针类型的关键字

    return 0;
}