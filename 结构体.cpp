#include <iostream>
using namespace std;
struct Student
{
public:
    int age = 10; //结构体中定义的成员变量或成员函数默认都是public属性的。
    string name;
    int score;
    void play();
} ss; //定义结构体的时候顺便创建变量ss
void Student::play()
{
    cout << "定义结构体函数" << endl;
}
int main()
{
    //1.声明结构体变量:前面最好加上struct，也可以省略
    Student s;
    struct Student s1 = {30, "123", 100};

    Student *s2 = &s1;
    printf("s.age=%d\ts1.name=%s\n", s1.age, &s1.name);
    cout << "s1.age=" << s1.age << endl;
    cout << "s2->age=" << s2->age << endl;
    ss.play();
    //2.结构体数组
    Student array[2] = {{30, "张三", 100}, {29, "李四", 80}};
    return 0;
}