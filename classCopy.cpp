#include <iostream>
#include <string>
using namespace std;
class Box
{
public:
    int age = 10;
    Box()
    {
        cout << "Box无参构造函数!" << endl;
    } // 无参构造函数
    Box(const Box &box)
    {
        cout << "拷贝构造函数" << endl;
    }
};
Box getBox()
{
    Box b;
    return b;
}
void setBox(Box b)
{
    
}
int main()
{
    // getBox();
    Box b = getBox();
    cout << getBox().age << endl;
    setBox(b);
    return 0;
}