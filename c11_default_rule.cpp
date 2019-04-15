#include <iostream>
#include <thread>

using namespace std;

struct item
{
    item() = default;
    item(const item& other) { cout << "item c" << endl; }
    item(item&& other) { cout << "item m" << endl; }

    ~item() {
        cout << "item d" << endl;
    }
};

// foo只声明移动操作，其默认复制操作不会生成
template <typename T>
class foo
{
public:
    template <typename I>   // 万能引用模板
    explicit foo(I&& val) : _val(forward<I>(val)) {}
    foo(foo&& other) : _val(move(other._val))
    {
        cout << "foo m" << endl;
    }
    
private:
    T _val;
};

// 按值传递形参
template<typename T>
void func(foo<T> param)
{

}

// 默认复制构造函数、默认移动构造函数都生产
struct nom
{
    string s;
};

void func(nom n)
{
    nom t = n;
    cout << t.s << endl;
}

void func_m(nom&& n)
{
    nom t = move(n);
    cout << t.s << endl;
}

int main(int argc, char const *argv[])
{
    // item it;
    // {
    //     item it_r = it;
    // }
    // cout << endl;
    // {
    //     foo<item> a = foo<item>(it);
    // }
    // cout << endl;
    // {
    //     foo<item> a(it);
    //     // func(a); // 声明移动操作，会删除默认复制操作。
    //     func(move(a));
    // }
    {
        nom a { "it is my rule"};
        func(a);
        func_m(move(a));
        func(a);
    }
    return 0;
}

// 移动操作生成条件：
//   1：未声明析构函数
//   2：未声明复制操作
//   3：未声明移动操作
// 声明移动操作将删除复制操作，除非显示声明或者使用default。