#include <iostream>
using namespace std;
//1.����Ȩ�ޣ���������Է��ʻ��������еķ�˽�г�Ա����˻����Ա������뱻������ĳ�Ա�������ʣ���Ӧ�ڻ���������Ϊ private��
//2.����Ĺ��캯�������������Ϳ������캯���������������������������Ԫ������
class A
{
public:
    A()
    {
        cout << "creat A " << endl;
    }

};
class B : virtual public A //�������η�Ĭ����private�����Ǽ�����ʹ�� protected �� private �̳У���ͨ��ʹ�� public �̳�
{
};
class C : virtual public A //��̳�:Ϊ�˽����̳�ʱ��������ͻ�������������⣬C++ �������̳У�ʹ������������ֻ�������������ĳ�ԱA��
{
};
class D : public B, public C
{
};
int main()
{
    D d; //A���������飬����̳б���
    return 0;
}
