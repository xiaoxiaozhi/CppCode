#include <iostream>
using namespace std;
/**
 * 容器
*/
int main()
{
    int ar[] = {1,2,3,4};
    int * p = ar;
    for (size_t i = 0; i < 3; i++)
    {
       cout<< p[i]<<endl;
    }
    
    return 1;
}