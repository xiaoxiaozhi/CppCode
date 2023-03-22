#include <iostream>
#include "swap.h"
using namespace std;
//[手把手教会VSCode的C++环境搭建，多文件编译，Cmake，json调试配置（ Windows篇）](https://www.bilibili.com/video/BV13K411M78v/?p=2&share_source=copy_web&vd_source=d858c6ae113d28a5ee3e69f10b5d3b41)
//1.g++编译单个文件
//   g++ ./main.cpp 生成a.exe(不包含调试信息)  g++ -g ./main.cpp 生成带调试信息的a.exe
//   点击右侧小甲壳虫提示让你创建launch.json 选择环境 C++ GDB/llDB  然后选择配置 g++.exe生成和调试活动，之后自动生成launch.json。
//   这时候就可以打断点调试.运行-->启动调试 或者F5调试。具体信息查看launch.json  里面添加了说明
//2.g++编译多个文件
//  引入其它文件 例如#include "swap.h"，"preLaunchTask":字段只能对单个文件生成调试文件，这时候我们要把他注释掉，用g++生成多个文件的调试文件
//   g++ -g .\main.cpp .\swap.cpp -o out.exe   注意不带-g 调试运行的效果和直接运行没有区别。然后手动指定调试文件的位置(program在这里指定)
//3.cmake 编译多个文件
//  Makefile 编译大型项目，但是书写比较困难，cmake能方便的生成Makefile。手动创建CmakeLists.txt,编写，此时vscode会创建build文件夹,
//  cd build,执行cmake ..生成Makefile，执行mingw32-make.exe 生成可执行文件。在launch.json中指定该文件位置即可调试
//  如果没有build文件，我们手动创建mkdir build ,cd build,cmake ..,mingw32-make.exe (这种方式我没有走通)
//4.自定义preLaunchTask(g++)
//  每次都要手动执行cmake .. ,执行名w32-make.exe效率太低我们创建一个 task来实现
//  点击菜单栏的Terminal->config Tasks 选择C/C++:g++.exe build active task 
//  输入 修改编译参数为 "main.cpp", "swap.cpp",然后替换lanuch.json的 "preLaunchTask": "刚才创建的task的label"
//5.自定义preLaunchTask(cmake)
//

int main(int argc,char **argv)
{
    int val1 = 0;
    int val2 = 2;
    cout<<"befor"<<endl;
    cout<<"val1 = "<<val1<<endl;
    cout<<"val2 = "<<val2<<endl;
    swap(val1,val2);
    cout<<"after"<<endl;
    // cout<<"val1 = "<<val1<<endl;
    // cout<<"val2 = "<<val2<<endl;
    return 0;
}