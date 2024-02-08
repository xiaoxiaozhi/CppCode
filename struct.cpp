#include <iostream>
using namespace std;
/**
 * 1.结构体属于用户自定义的数据类型，允许用户存储不同的数据类型
 * 2.结构体的定义和使用
 *   struct 结构体名 { 结构体成员列表 }；
 *   通过结构体创建变量的方式有三种：struct 结构体名 变量名 、struct 结构体名 变量名 = { 成员1值 ， 成员2值...}、定义结构体时顺便创建变量
 *   2.1 结构体数组
 *       将自定义的结构体放入到数组中方便维护
 * 3.结构体指针
 *   结构体变量利用操作符 . 访问成员
 *   结构体指针利用操作符 -> 访问成员
 * 4.结构体嵌套
 * 5.将结构体作为函数参数
 *   5.1 const修饰形参防止修改
 * 
 *   
 *   
 *   
 */
struct student
{
	// 成员列表
	string name; // 姓名
	int age;	 // 年龄
	int score;	 // 分数
} stu3;
struct teacher{
	int id;
	string name;
	int age;
	struct student stu; //子结构体 学生
};
//5. 将结构体作为函数参数
//值传递
void printStudent(student stu )
{
	stu.age = 28;
	cout << "子函数中 姓名：" << stu.name << " 年龄： " << stu.age  << " 分数：" << stu.score << endl;
}

//地址传递
void printStudent2(student *stu)
{
	stu->age = 28;
	cout << "子函数中 姓名：" << stu->name << " 年龄： " << stu->age  << " 分数：" << stu->score << endl;
}
//5.1 const使用场景
void printStudent(const student *stu) //加const防止函数体中的误操作
{
	//stu->age = 100; //操作失败，因为加了const修饰
	cout << "姓名：" << stu->name << " 年龄：" << stu->age << " 分数：" << stu->score << endl;

}
int main()
{
	// 2. 结构体三种定义方式
	student s; // 前面最好加上struct，也可以省略
	struct student s1 = {"赵六", 12, 100};
	//2.1 结构体数组
    struct student arr[3]=
	{
		{"张三",18,80 },
		{"李四",19,60 },
		{"王五",20,70 }
	};
	// 3.结构体指针
	student *s2 = &s1;
	printf("s.age=%d\ts1.name=%s\n", s1.age, s1.name);
	cout << "s1.age=" << s1.age << endl;
	cout << "s2->age=" << s2->age << endl;
	//4.结构体嵌套
	struct teacher t1;
	t1.stu = {"张三",18,80 };
	cout<<"老师的学生---"<<t1.stu.name<<" 学生年龄"<<t1.stu.age<<endl;


	return 0;
}