#include <iostream>
using namespace std;

/** class非模板, 成员函数为模板 */
class A
{
  public:
    template <typename T>   //模板构造函数
    A(T temp) { cout << temp << endl; }
    template <typename T>   //模板成员函数
    void A1(T temp) { cout << temp << endl; }
};

template <typename T>   //普通函数模板
void test(T temp)
{
    cout << temp << endl;
}

/** class为模板, 成员函数非模板 */
template<typename T>
class B
{
  public:
    B(T param) { cout << param << endl; }

    void B1(T param) { cout << param << endl; }
};

int main(int argc, const char* argv[])
{
    test<int>(12);  //普通模板函数
    A aa(12);       //请注意这一行
    aa.A1<int>(15.5); //成员函数是模板函数, 显示声明int， 可用<>可不用

    B<int> bb(77);  //需要限定B的类型参数
    bb.B1(88.9);    //参数和类模板参数类型一致
    return 0;
}