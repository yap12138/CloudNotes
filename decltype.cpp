#include <iostream>
 
// 此重载始终在重载集中
// 省略号参数拥有重载决议的最低等级
void test(...)
{
    std::cout << "Catch-all overload called\n";
}
 
// 添加此重载到重载集，
// 若 C 是到类的引用类型且 F 是指向 C 的成员函数的指针
template <class C, class F>
auto test(C c, F f) -> decltype((void)(c.*f)(), void())
{
    std::cout << "Reference overload called\n";
}
 
// 添加此重载到重载集，
// 若 C 是指向类的指针类型且 F 是指向 C 的成员函数的指针
template <class C, class F>
auto test(C c, F f) -> decltype((void)((c->*f)()), void())
{
    std::cout << "Pointer overload called\n";
}
 
struct X { void f() {} };
 
int main(){
  X x;
  test( x, &X::f);
  test(&x, &X::f);
  test(42, 1337);
}

// 在返回类型上使用表达式 SFINAE 的常见手法，其中表达式使用逗号运算符，
// 其左子表达式受到检验（转型到 void 以确保不选择返回类型上的用户定义逗号），而右子表达式拥有期望函数返回的类型。
