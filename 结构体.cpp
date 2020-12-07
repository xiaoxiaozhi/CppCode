#include <iostream>
using namespace std;
struct Student
{
    int age = 10;
    string name;
    int score;
} ss; //定义结构体的时候顺便创建变量ss

int main()
{
    //1.声明结构体变量:前面最好加上struct，也可以省略
    struct Student s;
    struct Student s1 = {30, "张三", 100};

    Student *s2 = &s1;
    cout << "s.age=" << s.age << "\ts1.name=" << s1.name << endl;
    cout << "s1.age=" << s1.age << endl;
    cout << "s2->age=" << s2->age << endl;
    //2.结构体数组
    Student array[2] = {{30, "张三", 100}, {29, "李四", 80}};
    return 0;
}