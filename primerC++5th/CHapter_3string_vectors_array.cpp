
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
using std::string;
using namespace std;
/**
 * 第三章 字符串、向量、数组
 *    string是字符的集合 vector是对象的集合
 * 3.1. 使用using 我们使用的库函数基本都属于std命名空间，可以使用using声明来简化代码 using std::string;或者 using namespace std;
 *    头文件不应该包含using声明，引用了头文件的cpp文件，容易造成名字冲突
 * 3.2.1 定义和初始化string 参见下列代码
 *       直接初始化和拷贝初始化
 *        使用=号初始化一个变量执行的是拷贝初始化，例如 string str = "Hello";
 *        不适用等号则执行的是直接初始化，例如 string str("Hello");
 * 3. string的常用操作
 *    3.1 读写string cin getline
 *        标准输入cin会自动忽略开头的空白字符（空格、制表符、换行符），读取到下一个空白字符时停止例如 " hello world " 只会读入hello
 *        string字符串输入操作返回左侧的运算符 cin>>str; 返回cin对象本身，可以连在一起使用 cin>>str1>>str2;
 *        标准输出cout<<str;会忠实地输出字符串中的所有字符，包括空格、制表符、换行符等空白字符。
 *        如果想输入流保存空格使用getline(cin,str); 读取一整行，包括空格，直到遇到换行符为止，换行符不会存入str中
 *        getline(cin,str1)函数返回cin对象本身，从给定的输入流中读取字符直到遇到换行符，如果一开始是换行符得到一个空string
 *    3.2 string的empty size
 *        string.size()返回string::size_type类型是一个无符号整形，足够表示任意string的长度
 *    string操作还有很多看3.2.2章节和3.2.3章节表格 cctype.h头文件定义了一组标准库函数处理string
 *    值的注意的是 str[0] 通过下标获取字符引用，能够改变string中的字符
 *    3.3 C++11提供了范围for（range for）语句，可以遍历给定序列中的每个元素并执行某种操作
 *        for (char c : str) { cout << c << endl; } // 依次输出str中的每个字符 c是str中每个字符的副本，不能修改str中的字符
 *        只要把for循环变量定义成索引 for(char &c：str) 在for循环中也能改变string的值
 * 4. 标准库vector
 *    vector是一个类模板，在模板后面跟<>指定实例化的信息。vector能容纳绝大多数元素对象，但是引用不是对象，不包含引用。
 *    vector也能容纳vector对象，在旧的c++标准中，vector<vector<int> >必须添加一个空格，在c++11则不需要
 *    4.1 定义和初始化vector
 *        vector<T> v1; vector保存类型为T的对象。默认构造函数，v1为空。
 *        vector<T> v2(v1); v2是v1的一个副本。 等价于 v2 = v1
 *        vector<T> v3(n,i); v3包含n个值为i的元素。
 *        vector<T> v4(n); v4含有值初始化的元素的n个副本。（n个元素）
 *        vector<T> v5{a,b,c}; v5包含初始值列表中的元素 a,b,c
 *        vector<T> v6 = {a,b,c}; 等价于 v5
 *        特殊情况 使用列表初始化，如果参数不符合要求，c++编译器会常使用直接初始化
 *        例如 vector<string> v7{10};  v7含有10个空字符串
 *        例如 vector<string> v8{10,"hi"};  v8含有10个 "hi"元素
 *    4.2 vector添加元素
 *        使用push_back在vector尾部添加元素 v.push_back(i);
 *        实际上c++要求vector快速高效添加元素，所以在定义时对vector设定大小就没有必要了
 *   4.3 其他vector操作
 *       v.size() 返回类型 size_type 同 string 的 size_type
 *       太多了，自己搜索
 *   3.4.1 使用迭代器
 *       除了for循环和下标索引访问vector元素，还可以使用迭代器。类似指针迭代器也提供了对对象间接访问的能力
 *       所有标准库的容器都可以使用迭代器。虽然string不是容器类，但是它也支持迭代器
 *       v.begin() 返回一个迭代器，指向vector的第一个元素 v.end() 返回一个迭代器，指向vector最后一个元素的下一个位置,尾后迭代器没什么意义只是一个标志位
 *       如果vector为空，v.begin()和v.end()返回相同的迭代器，都是尾后迭代器
 *       *iter 返回迭代器所指向的元素的引用
 *       iter->mem 解引用iter并获取该元素名为mem的成员等价于(*iter).mem
 *       ++iter 使iter指向下一个元素
 *       --iter 使iter指向前一个元素
 *       iter1 == iter2 如果iter1和iter2指向同一个元素返回true 否则返回false
 *       迭代器类型 iterator 和 const_iterator 前者能读写，后者只能读。 如果 vector或string是一个常量对象只能使用const_iterator
 *       c++11 增加了一个方法让不是常量对象的容器也能使用const_iterator  v.cbegin() 和 v.cend()
 *       因为end返回的迭代器并不实际指向某个元素，所以不能对其进行递增或者解引用的操作。
 *       在for或者其他循环语句的判断条件中，最好使用!=而不是<。所有标准库容器的迭代器都定义了==和!=，但是只有其中少数同时定义了<运算符
 *       不能在for循环中想vector添加元素，因为添加元素可能会导致vector重新分配内存，从而使迭代器失效
 *       任何改变vector容量的操作都可能使迭代器失效
 *   3.5 数组
 *       数组也是存放相同对象的容器与vector类似，但是数组的大小是固定的，不能动态改变
 *       数组是一种复合类型，声明形式为a[d]，其中a是数组名称，d是数组维度（dimension）。维度必须是一个常量表达式。
 *       数组声明时就要指定大小，也可用初始化列表初始化数组，此时可以不用指定大小
 *       不能将其他数组的值拷贝给数组初始化，与vector不同数组没有拷贝初始化
 *       int a2[] = {1,2,3,4,5}; // 大小由初始化列表决定
 *       int a3[5] = {1,2}; // 其余元素初始化为0
 *       int a4[1] = {1,2}; // 错误，初始值不能多于数组大小
 *       字符数组的特殊性
 *       char a1[] = {'c','+','+'}; // 普通字符数组
 *       char a2[] = {'c','+','+','\0'}; // 是c风格字符串
 *       char a3[] = "c++"; // 字符串字面值初始化字符数组，并在末尾自动追加字符串结束符 \0。
 *       a1的维度是3， a2和a3的维度是4
 *       a2和a3是等价的，都是c风格字符串.
 *       char* str4 = "c++";//字符指针指向字符串字面量 只读。省略了const 不推荐
 *       const char* str5 = "c++";//字符指针指向C风格字符串 推荐
 *
 *       复杂数组的声明
 *       int *ptrArr[10]; // ptrArr是一个含有10个指向int类型的指针的数组
 *       int &refArr[10]; // 错误，不能定义引用数组, 引用不是对象
 *       int (*Parray)[10] = &arr; // Parray是一个指向含有10个int类型元素的数组的指针
 *       int (&arrRef)[10] = arr;    // arrRef是一个引用，指向一个含有10个int类型元素的数组
 *   3.5.2 访问数组元素
 *         使用for循环或者下标索引访问数组元素
 *   3.5.3 数组和指针
 *         编译器会自动把数组名替换成指向数组首元素的指针，使用数组作为一个auto 变量初始值时，推断得到的类型是指针
 *         int arr[10]; auto p(arr); // p的类型是int* 编译器实际执行的是 auto p(&arr[0]);
 *         使用decltype(arr)返回数组类型 int[10]
 *         迭代器支持的运算数组指针全都支持，++p, --p, p+n, p-n, p1 - p2, p[n], *p 等等
 *         前提是获得数组第一个元素的指针，以及尾元素下一位置的指针 int *p = &arr[10]; 这样不会报错吗？本质是arr+10
 *         用下标获取尾后指针的方式极易出错，c++用两个标准库函数 begin() 和 end() 来获取数组首元素和尾后位置的指针
 *         begin(arr); end(arr);
 *         下标和指针：数组名起始是指向首元素的指针，使用下标实际上是arr[2] 等价于 *(arr + 2)
 *   3.5.4 c风格字符串
 *         c风格字符串不是一种类型，而是一种使用char数组来存放字符串的习惯用法，按次习惯书写的字符串以空字符'\0'结尾
 *         cstring.h头文件定义了一组处理c风格字符串的标准库函数 strlen strcpy strcat strcmp 等等
 *         strlen(str)； 返回c风格字符串str的长度，不包括结尾的'\0'
 *         strcmp(str1,str2); 比较两个c风格字符串的大小，按字典顺序比较，相等返回0，第一个字符串大返回正值，第二个字符串大返回负值
 *         strcpy(dest,src); 将src指向的c风格字符串拷贝到dest指向的内存空间，dest必须足够大以容纳src字符串,覆盖式拷贝，不保留目标字符串原有数据
 *         strcat(dest,src); 将src指向的c风格字符串连接到dest指向的c风格字符串的结尾，dest必须足够大以容纳连接后的字符串,追加
 *         char str[] = {'h','e','l','l','o' }; // 不是c风格字符串，没有用'\0'结尾，使用c风格字符串的函数也会报错
 *         c风格字符串本质是数组无法使用关系运算符比较大小，只能使用strcmp函数比较两个c风格字符串的大小
 *         使用strcmp比较两个c风格字符串的大小，按字典顺序比较，相等返回0，第一个字符串大返回正值，第二个字符串大返回负值
 *         总之使用string比c风格字符串更安全更方便
 *   3.5.5 与旧代码的接口
 *         c++串门提供了一些功能兼容c风格字符串的接口
 *         c_str()成员函数 返回一个指向常量c风格字符串的 const char* 指针（不可修改内容）；，该字符串与string对象内容相同，如果string内容修改，指针失效
 *         data() C++11 及以后：与 c_str() 功能完全一致，返回带 \0 的 const char*； C++11 之前：返回不带 \0 的字符数组指针（仅兼容旧标准）
 *         c风格字符串转string，有两种 
 *         const char* c_str1 = "Hello C++";
 *         string cpp_str1(c_str1); // 拷贝初始化
 *         string cpp_str2 = c_str1; // 直接初始化
 *   3.6   多维数组
 *
 *
 *
 *
 *
 *
 */
int main()
{
    string str = "Hello, World!";
    // 2. 定义和初始化string
    string str1;           // 默认初始化为空字符串
    string str2 = str1;    // 拷贝初始化
    string str3 = "Hello"; // str3是 字符串字面值Hello的副本，拷贝初始化
    string str4(10, 'c');  // str4包含10个字符c, 连续 n个 _ 字符组成的字符串 ，直接初始化
    string str5("World");  // str5是str3的副本，直接初始化
    // 3. string的常用操作
    // 3.1 读写string
    string readStr1, readStr2;
    // cin >> readStr1;
    // cout << readStr << endl;
    // cin >> readStr1 >> readStr2;
    // cout << readStr <<" "<<readStr2 << endl;//多个输出可以连在一起写
    // 3.1 读取位置数量的string
    // while (cin>>readStr1)//为什么cin可以做逻辑运算，没有说清楚。 只要cin没有遇到结束符或者非法输入处于有效状态就返回true
    // {
    //     cout<< readStr1 << endl;
    // }
    // 3.1.2 getline读取整行包括空格
    // while (getline(cin,readStr2))
    // {
    //     cout<< readStr2 << endl;
    //     break;
    // }
    // 4.2
    vector<int> intVec; // 定义一个空的int类型的vector
    for (size_t i = 0; i < 10; i++)
    {
        intVec.push_back(i); // 添加0-9十个元素到vector中
    }
    // 3.4.2 使用迭代器判断string是否为空
    string iterStr = "Iterator Example";
    if (iterStr.begin() != iterStr.end())
    {
        cout << "The iterStr is not empty." << endl;
    }
    // 3.5.2 for循环访问数组元素
    int arr[5] = {1, 2, 3, 4, 5};
    // for(auto &elem : arr)
    // {
    //     cout << elem << " "<<endl;
    // }
    // 3.5.3 尾元素下一位置的指针
    int *p = &arr[5];                                             // 指向尾元素下一位置
    cout << "The value at tail next position is: " << *p << endl; // 尾后指针不指向具体元素，解引用为什么没有报错呢？vscode没报错，其他编辑器可能会报错
    for (int *fp = arr; fp != p; ++fp)
    {
        cout << "fp = " << *fp << " " << endl;
    }
    int *b = begin(arr);
    int *e = end(arr);
    // 3.5.4 比较两个字符串
    string a1 = "apple";
    string a2 = "banana";
    if (a1 < a2)
    {
        cout << a2 << " is less than " << a1 << endl; // 字典顺序比较

    } // 用关系运算符比较两个string大小
    char cstr1[] = "apple";
    char cstr2[] = "banana";
    char cstr3[20];
    // if(cstr2 < cstr1)//错误，使用数组实际上用的是指向首元素的指针，这两个指针没有指向同一数组的元素，所以比较没有意义
    // 连接和拷贝字符串
    string a3 = a1 + a2; // 连接两个string
    cout << "cstr1 length " << strlen(cstr1) << endl;
    cout << "cop ctr1 and ctr2 " << strcmp(cstr1, cstr2) << endl;
    // 使用strcat strcpy连接和拷贝c风格字符串
    strcpy(cstr3, cstr1); // 拷贝cstr1到cstr3
    cout << "cstr3: " << cstr3 << endl;
    strcat(cstr3, cstr2); // 连接cstr2到cstr3
    cout << "cstr3: " << cstr3 << endl;
    // 3.5.5
    string cppStr = "C++ String";
    cppStr.c_str(); // 返回一个指向常量c风格字符串的指针，该字符串与cppStr内容相同
    return 0;
}