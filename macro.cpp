#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;
/**
 * C++ 宏有哪些用法 宏函数的参数不是值也不是参数，是替换。来自deepseek
 * 1. 对象宏（Object-like Macro）：用于定义常量。 #default PI 314 只支持整形
 * 2. 宏函数 类似函数调用，可以带参数
 * 3. 多行宏：如果宏内容很长，可以使用反斜杠\换行
 * 4. 链接运算符将两个标记连接成一个(根据例子，不清楚这样做有什么用)
 * 5. 条件编译：
 * 6. 字符串化 #define STRINGIFY(x) #x
 * 7. 变参宏 ...表示可变参数部分。在宏体中，可以使用__VA_ARGS__来代表这些可变参数。 搞不懂 没有打印所有参数
 */
#define PI 314                    // 1
#define MAX(a, b) (a > b ? a : b) // 2
#define SWAP(a, b)          \
    do                      \
    {                       \
        typeof(a) temp = a; \
        a = b;              \
        b = temp;           \
    } while (0)

#define combin(x, y) x##y
// 5 条件编译 #ifdef #ifndef #else #elif #endif
#ifdef PI
#define WAY 15926
#else
#define NO 123
#endif
// 6. 字符串化
#define STRINGIFY(x) #x
// 7. 变参宏
#define PRINT(...) cout <<(__VA_ARGS__)<< endl

int main()
{
    // 1.
    cout << PI << endl;
    // 2.
    cout << MAX(1, 2) << endl;
    // 3.
    //  SWAP(11,22);//错误不能交换两个右值
    int x = 10, y = 20;
    SWAP(x, y);
    cout << "x=" << x << ", y=" << y << endl; // x
    // 4.
    int xy = 10;
    int result = combin(x, y); //
    cout << "result=" << result << endl;
// 5. 条件编译
#if PI == 314
    cout << "PI = 3.14" << endl;
#else
    cout << "PI!=" << endl;
#endif
    // 7. 变参宏
    PRINT("sdf",123,456);
    return 0;
}