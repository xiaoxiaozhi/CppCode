#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
/**
 * 为了建立数据结构和算法的一套标准,诞生了STL(Standard Template Library,标准模板库)\
 * STL大体分为六大组件，分别是:容器、算法、迭代器、仿函数、适配器（配接器）、空间配置器
 * 1.vector
 *   STL最常用容器理解为数组,v.push_back()插入数据，
 *   1.1 迭代器 通过迭代器遍历
 *       起始迭代器：v.begin()  指向容器第一个元素
 *       结束迭代器: v.end()   指向容器最后一个元素下一个位置
 *   1.2 while 遍历
 *   1.3 for  遍历
 *   1.4 利用STL提供的遍历 方法 for_each 需要提前导入头文件 #include <algorithm>。为什么能把函数当做参数传入这是因为c++有指向函数的指针以及函数对象
 *       具体内容接下来会讲
 *   1.5 存放自定义类型
 *   1.6 存放自定义类型指针
 *   1.7 vector嵌套容器 TODO
 * 
 *
 *
 */
void myPrint(int &num)
{
    cout << "for_each---vector---" << num << endl;
}
class Person
{
public:
    Person(string n, int a) : name(n), age(a) {}
    int age;
    string name;
};
int main()
{
    // 1.创建vector容器
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    // 1.1 声明迭代器遍历
    vector<int>::iterator iBegin = v.begin();
    vector<int>::iterator iEnd = v.end();
    // 1.2 whil遍历方式
    while (iBegin != v.end())
    {
        cout << "while---vector---" << *(iBegin++) << endl;
    }
    // 1.3 for遍历
    for (vector<int>::iterator iBegin = v.begin(); iBegin != v.end(); iBegin++)
    {
        cout << "for---vector---" << *iBegin << endl;
    }
    // 1.4 for_each 遍历
    for_each(v.begin(), v.end(), myPrint);
    // 1.5 vector存放自定义数据类型
    vector<Person> vp;
    vp.push_back(Person("张三", 11));
    vp.push_back(Person("李四", 22));
    vp.push_back(Person("王五", 33));
    for (vector<Person>::iterator iBegin = vp.begin(); iBegin != vp.end(); iBegin++)
    {
        cout << "自定义类型---姓名：" << iBegin->name << " 年龄：" << iBegin->age << endl;
    }
    // 1.6 存放自定义类型指针
    vector<Person *> vpp;
    vpp.push_back(new Person("王五", 101)); // vpp.push_back(&Person("王五", 101)); 不能这么写，表达式结束临时对象就会被释放
    vpp.push_back(new Person("赵六", 202));
    vpp.push_back(new Person("孙谦", 303));
    for (vector<Person *>::iterator iBegin = vpp.begin(); iBegin != vpp.end(); iBegin++)
    {
        cout << "自定义类型指针---姓名：" << (*iBegin)->name << " 年龄：" << (*iBegin)->age << endl; //->优先级比解引用高，所以要用括号
    }
    
    return 0;
}