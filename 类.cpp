#include <iostream>
using namespace std;
//1.��������η�
//2.�������캯��
//3.��Ԫ����
//4.����������thisָ�롢��̬��Ա
//5.����
// class BoxFriend;
class Box
{

public:
    int height;
    int length;
    int breath;
    int getLength(void);
    void setLength(int length);
    static int objCount; //�ڴ�����һ������ʱ�����еľ�̬���ݶ��ᱻ��ʼ��Ϊ�㡣���ǲ��ܰѾ�̬��Ա�ĳ�ʼ����������Ķ�����
    int getBreath(void)  //�����ڲ�����ĺ���������������������ר��д�ؼ���inline�����������ĺô��Ǽӿ������ٶȣ�ԭ���ÿռ任ʱ�䡣����һ�㶼��С����
    {
        this->breath; //this ָ�룺ÿһ��������ͨ�� this ָ���������Լ��ĵ�ַ��this ָ�������г�Ա��������������
        return breath;
    }
    static int getCount()
    {
        return objCount; //��̬��Ա����û�� this ָ�룬ֻ�ܷ��ʾ�̬��Ա
    }
    Box operator+(const Box &box) //���������
    {
        Box b;
        b.breath += box.breath;
        b.length += box.length;
        b.height += box.height;
        b.noWay += box.noWay;
    }
    Box();                                   //���캯��
    Box(int height, int length, int breath); //�������Ĺ��캯��
    ~Box();                                  //����������ÿ��ɾ������������ʱ����
    Box(const Box &obj);                     //�������캯�������������û�ж��忽�����캯���������������ж���һ������������ָ����������ж�̬�ڴ���䣬����������һ���������캯������������

protected: //��private�����ƣ����������п��Է���
private:   //��Ա�����Ĭ�Ϸ������η��� private��ֻ�ܱ������Ա�����ڣ�����Ԫ���ʣ����ܱ���������ʣ�
    int noWay = 10;
    int *ptr;
    friend void printnoWay(Box box); //��Ԫ���������Է������private �� proteced ��Ա�����ǲ������Ա������������������һ���ӿ�
    friend class BoxFriend;          //��Ԫ����������������������ν
    // friend void BoxFriend::printWidth(Box box); //ָ��һ�����еĺ���Ϊ��Ԫ����
};
Box::Box()
{
    objCount++;
}
Box::Box(int height, int length, int breath) : height(height), length(length), breath(breath) //��ʼ���б��ʼ���ֶ�
{
    // Ϊָ������ڴ�
    ptr = &height;
}
Box::~Box()
{
    cout << "1��������ʱִ��" << endl;
}
Box::Box(const Box &obj) //�����Զ��忽�����캯��
{
    // noWay = obj.breath;//��㸳ֵ
    cout << "1���ÿ������캯��" << endl;
}
void printnoWay(Box box)
{
    cout << "ֱ�ӷ���˽������noWay=" << box.noWay << endl;
}
void Box::setLength(int len) //��Ա���������޷���javaһ���Զ�����
{
    length = len;
}
int Box::getLength() //�����ⲿ�÷�Χ��������� :: ���庯��
{
    return 10;
}
int Box::objCount = 10; //��̬��Ա���ⲿ��ʼ��
class SmallBox : public Box
{
};
class BoxFriend
{
public:
    void printNoway()
    {
        Box box;
        box.noWay;
    }
};
int main()
{
    cout << "Inital Stage Count: " << Box::getCount() << endl; //��̬��Ա������ʹ������󲻴��ڵ������Ҳ�ܱ����ã���̬����ֻҪʹ�������ӷ�Χ��������� :: �Ϳ��Է��ʡ�
    Box box;                                                   //����һ������,���Ĭ�Ϲ��캯��ֻ����������ᱨ��
    Box copy = box;                                            //���ÿ������캯��
    printnoWay(box);
    cout << "Final Stage Count: " << Box::getCount() << endl;
    Box b2 = box + copy; //����������
}