#include <iostream>
using namespace std;
int main()
{
    //1.指针定义,两种方式
    int a = 10;
    int *p;       //定义指针1
    p = &a;       //指针赋值
    int *p1 = &a; //定义指针2
    int *ptr;

    //2.指针使用,解引用
    cout << "p指向内存-->内容" << *p << endl; //解引用
    //3.指针所占字节:32位占4个字节 64位操作系统占8字节
    cout << "指针P占" << sizeof(p) << "字节" << endl;
     cout << "指针ptr占" << sizeof(ptr) << "字节" << endl;
    cout << "int类型指针占" << sizeof(int *) << "字节" << endl;
    //4.空指针。暂时不知道指向哪里 内存0~255 不允许用户访问
    int *p2 = NULL;
    //5.野指针：自己分配的地址，使用报错。地址只能由系统分配.vscode 可以运行，其它ide不能运行
    int *p3 = (int *)0x12334;
    cout << "取野指针P3内容" << *p3 << endl;
    //6.const 修饰指针:
    const int *p4 = &a;       //常量指针：指向可以修改，指向的值不可以修改,const修饰值(*p4),所以不能修改值
    int *const p5 = &a;       //指针常量：指向不可以修改，指向的值可以修改，const修饰指针，所以不能修改指向
    const int *const p6 = &a; //既修饰指针又修饰常量，都不可以改
}