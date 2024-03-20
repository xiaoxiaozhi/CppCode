#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
/**
 *   1.string是C++风格的字符串，char * 是一个指针；string是一个类，类内部封装了char*，string 类内部封装了很多成员方法
 *   例如：查找find，拷贝copy，删除delete 替换replace，插入insert
 *   string管理char*所分配的内存，不用担心复制越界和取值越界等，由类内部进行负责
 *   2.string构造函数
 *   string(); //创建一个空的字符串 例如: string str;
 *   string(const char* s); //使用字符数组初始化
 *   string(const string& str); //拷贝构造函数，创建字符串
 *   string(int n, char c); //使用n个字符c初始化
 *   感觉第二种和第三种是一样的东西，字符串是常量不可修改，所以用const char* s引用
 *   3. string赋值操作
 *   string& operator=(const char* s); //char*类型字符串 赋值给当前的字符串
 *   string& operator=(const string &s); //把字符串s赋给当前的字符串
 *   string& operator=(char c); //字符赋值给当前的字符串
 *   string& assign(const char *s); //把字符串s赋给当前的字符串
 *   string& assign(const char *s, int n); //把字符串s的前n个字符赋给当前的字符串
 *   string& assign(const string &s); //把字符串s赋给当前字符串
 *   string& assign(int n, char c); //用n个字符c赋给当前字符串
 *   4. string拼接
 *   string& operator+=(const char* str); //重载+=操作符
 *   string& operator+=(const char c); //重载+=操作符
 *   string& operator+=(const string& str); //重载+=操作符
 *   string& append(const char *s);  //把字符串s连接到当前字符串结尾
 *   string& append(const char *s, int n); //把字符串s的前n个字符连接到当前字符串结尾
 *   string& append(const string &s); //同operator+=(const string& str)
 *   string& append(const string &s, int pos, int n);//字符串s中从pos开始的n个字符连接到字符串结尾
 *   TODO 还有查找、替换、比较、删除、存取、插入、获取子字符串
 */
int main()
{

    // 2. string构造函数
    string s;
    char c[] = "char c[]";
    string s1(c);
    string s2("释放"); //
    string s3(5, 'c');
    cout << "string(const char* s)---" << s1 << endl;
    cout << "string(const string& str)---" << s2 << endl;
    cout << "string(int n, char c)---" << s3 << endl;
    // 3. string 赋值操作
    string s4 = "operator=(const char* s);";
    cout << "赋值操作1---" << s4 << endl;
    string s5 = s4;
    cout << "赋值操作2---operator=(const string &s)" << endl; // s5
    string s6;
    s6.assign("string& assign(const char *s, int n);", 50); // 超出前面字符串长度，不会报错但是会乱码
    cout << "赋值操作5---" << s6 << endl;                   // s5
    return 1;
}