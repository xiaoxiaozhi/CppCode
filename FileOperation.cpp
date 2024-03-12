#include <iostream>
#include <string>
#include <fstream>
using namespace std;
/**
 * 文件操作
 * C++中对文件操作需要包含头文件 <fstream>
 * 1.文件类型分为两种：
 *   文本文件 - 文件以文本的ASCII码形式存储在计算机中
 *   二进制文件 - 文件以文本的二进制形式存储在计算机中，用户一般不能直接读懂它们
 * 2.操作文件的三大类:
 *   2.1 ofstream： 写操作 首先包含头文件 创建流对象ofstream ofs; 打开文件路径ofs.open("文件路径",打开方式);、写文件、ofs << "写入的数据";‘
 *       关闭文件ofs.close();  这几步骤
 *   2.2 ifstream： 读操作  读操作除了对象其他步骤和写操作一致，四种读取文件方式
 *   2.3 fstream ： 读写操作
 * 3.操作二进制文件
 *   方式与上文差不多，打开方式多了个。ios::out | ios::binary 写方式组合   ios::in | ios::binary 读方式组合
 *   用函数write写数据,二进制写文件 能够写入自定义类型。上面的文本操作只能操作。自定义类里面不要用c++的string容易出问题，要用char[]
 * 打开方式 读写公用；读写对象.isopen() 判断是否打开文件成功；EOF关键字代表文件末尾
 * TODO 没有教怎么设置路径；读完的文件无法复读暂时还没有找到方法;读写二进制文件没看----
 */
class FileOperate
{
    const char *fileNmae;

public:
    FileOperate(const char *name) : fileNmae(name) {}
};
void writeFile();
void readFile();
// 二进制文件操作
void writeBinary();
void readBinary();
int main()
{
    // 2.1 写文件
    writeFile();
    // 2.1 读文件
    readFile();
    // 3. 写二进制文件
    writeBinary();

    return 1;
}
/**'
 * 打开方式	解释
 * ios::in	    为读文件而打开文件
 * ios::out	    为写文件而打开文件
 * ios::ate	    初始位置：文件尾
 * ios::app	    追加方式写文件
 * ios::trunc	如果文件存在先删除，再创建
 * ios::binary	二进制方式
 */
void writeFile()
{
    ofstream ofs;
    // 不指定路径就是本项目下创建，没有讲怎么制定路径
    // ios::out | ios::binary 方式组合
    ofs.open("test.txt", ios::out);
    if (ofs.is_open())
    {
        cout << "writeFile---文件打开成功" << endl;
        ofs << "123";
        ofs << "456" << endl; // endl 换行符
        ofs << "789" << endl;
        ofs << "汉字" << endl; // TODO 不是说以ascii码存储吗？怎么写入汉字也不会乱码？？？
        ofs.close();
    }
    else
    {
        cout << "writeFile---文件打开失败" << endl;
    }
}
/**
 *
 */
void readFile()
{
    // ifstream ifs;
    // ifs.open("test.txt", ios::in);
    // 等价于
    ifstream ifs("test.txt", ios::in);

    if (ifs.is_open())
    {
        cout << "readFile---文件打开成功" << endl;
        // 第一种方式按 数组读取
        char buffer[1024] = {};
        while (ifs >> buffer)
        {
            cout << "第一种方式：按数组读取---" << buffer << endl;
        }
        // 第二种读取方式 按行读取
        char buf[1024] = {};
        while (ifs.getline(buf, sizeof(buf)))
        {
            cout << "第二种方式：按行读取---" << buf << endl;
        }
        // 第三种方式
        string buf1;
        while (getline(ifs, buf1))
        {
            cout << "第三种方式：用全局函数getline()读取---" << buf << endl;
        }
        // 第四种方式，每次读取一个字符
        char c;
        while ((c = ifs.get()) != EOF)
        {
            cout << "第四种方式：按字符读---" << c << endl;
        }

        ifs.close();
    }
    else
    {
        cout << "readFile---文件打开失败" << endl;
    }
}

void writeBinary()
{
    ofstream ofs("binary.bin", ios::out | ios::binary);
    if (ofs.is_open())
    {
        cout << "writeBinary---打开文件成功" << endl;
    }
    else
    {
        cout << "writeBinary---打开文件失败" << endl;
    }
    FileOperate fo("二进制写");
    ofs.write((const char *)&fo,sizeof(fo));
    ofs.close();
}
void readBinary()
{
}