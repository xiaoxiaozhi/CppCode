#include <iostream>
using namespace std;
/**
 * 1. 指针的定义
 *    指针变量定义语法： 数据类型 * 指针名；  数据类型 * 指针名 = &变量
 * 2. 解引用
 *    指针变量可以通过" * "操作符，操作指针变量指向的内存空间，这个过程称为解引用
 * 7. 指针和数组、字符串
 *    实践证明  指针+1 刚好是下一个元素的地址可能内部有处理，
 *    bool类型数组 指针+1 0x61fd93第一个元素地址 0x61fd94第二个元素地址
 *    int类型数组 指针+1 0x61fda0第一个元素地址 0x61fda4第二个元素地址
 *    比较特别的是字符型数组，cout << strp << " " << strp + 1 << endl; 打印地址就是打印字符串，
 *    打印strp从第一个元素开始的字符串，打印 stp+1 就是打印从第二个元素开始的字符串
 *    看网上的回答是 输出流被重载过了，正常来说应该输出地址，在vscode输出的是字符串
 *    https://www.zhihu.com/question/52252124
 * 8. 指针和函数
 *    利用指针作函数参数，可以修改实参的值
 * --------------------------------------来自网络查找
 * void * 可以接收任意指针类型  void * value = new int(0); 接收int指针， 使用的时候 *(int*)value 这种强制转换并不推荐
 * *(static_cast<int*>(ptr))类型安全c++推荐做法 
 * (void*)30 黑马老师这样转换我编译报错，正确做法应该是 static_cast<void*>(new int(30))
 * int* restrict num = new int(1); restrict 关键字的作用是， num指向的内存空间只能由num操作，该片内存空间不能让其他指针操作
 *
 */
void swap2(int *p1, int *p2);
int main()
{
    // 1.指针定义,两种方式
    int a = 10;
    int *p; // 定义指针1
    p = &a; // 指针赋值
    // 1.1
    int *p1 = &a;       // 定义指针2
    int *ptr = nullptr; //未初始化的指针 也是有值的指向一块不明确的内存，如果直接解引用 *ptr会报错，所以我们最好指定它为nullptr,在C++11标准引入了 nullptr 关键字，用来表示空指针
    // 2.指针使用,解引用
    cout << "p指向内存-->内容" << *p << "指针值" << p << endl; // 解引用
    // 3.指针所占字节:32位占4个字节 64位操作系统占8字节
    cout << "指针P占" << sizeof(p) << "字节" << endl;
    cout << "int类型指针占" << sizeof(int *) << "字节" << endl;
    // 4.空指针。暂时不知道指向哪里 内存0~255 不允许用户访问
    int *p2 = NULL;
    // 5.野指针：自己分配的地址，使用报错。地址只能由系统分配.vscode 可以运行，其它ide不能运行
    //  int *p3 = (int *)0x12334;
    //  cout << "取野指针P3内容" << *p3 << endl;
    // 6.const 修饰指针:
    const int *p4 = &a;       // 常量指针：指向可以修改，指向的值不可以修改,const修饰值(*p4),所以不能修改值
    int *const p5 = &a;       // 指针常量：指向不可以修改，指向的值可以修改，const修饰指针，所以不能修改指向
    const int *const p6 = &a; // 既修饰指针又修饰常量，都不可以改
    // 7. 指针和数组
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int *pp = arr; // 指向数组的指针
    cout << "第一个元素： " << arr[0] << endl;
    cout << "指针访问第一个元素： " << *pp << pp << endl;
    cout << "指针访问第二个元素： " << *(pp + 1) << endl;
    char str[] = "hello word";
    char *strp = str;
    cout << *strp << "---" << *(strp + 1) << endl; // 为什么指针+1
    cout << strp << "---" << strp + 1 << endl;     // 为什么不打印地址而是字符串？？？
    printf("strp---%s\n", strp);                   // 打印C风格类型的字符串
    const char *cc = "字符串";                     // 字符串是一个常量，直接赋值给char*会报警告，前面要加个const转成常量指针让他们类型一致
    cout << "常量char指针引用字符串---" << cc << endl;
    bool bArray[] = {true, false};
    bool *bArrayP = bArray;
    cout << bArrayP << " " << bArrayP + 1 << endl;
    cout << pp << " " << pp + 1 << endl;
    // 8.
    int aa = 1;
    int c = 2;
    swap2(&aa, &c); // 地址传递会改变实参
    cout << "aa = " << aa << endl;
    cout << "c = " << c << endl;
}
// 地址传递
void swap2(int *p1, int *p2)
{
    int temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}