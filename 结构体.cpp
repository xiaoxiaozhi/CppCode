#include <iostream>
using namespace std;
struct Student
{
    int age = 10;
    string name;
    int score;
} ss; //����ṹ���ʱ��˳�㴴������ss

int main()
{
    //1.�����ṹ�����:ǰ����ü���struct��Ҳ����ʡ��
    struct Student s;
    Student s1 = {30, "����", 100};

    Student *s2 = &s1;
    cout << "s.age=" << s.age << "\ts.name=" << s.name << endl;
    cout << "s1.age=" << s1.age << endl;
    cout << "ָ��s2->age=" << s2->age << endl;
    //2.�ṹ������
    Student array[2] = {{30, "����", 100}, {29, "����", 80}};
    return 0;
}