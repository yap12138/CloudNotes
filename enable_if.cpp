#include <type_traits>
#include <iostream>
#include <string>
 
namespace detail { struct inplace_t{}; }
void* operator new(std::size_t, void* p, detail::inplace_t) {
    return p;
}
 
// #1 ，通过返回类型使用
template<class T,class... Args>
typename std::enable_if<std::is_trivially_constructible<T,Args&&...>::value>::type 
    construct(T* t,Args&&... args) 
{
    std::cout << "constructing trivially constructible T\n";
}
 
// #2
template<class T, class... Args>
std::enable_if_t<!std::is_trivially_constructible<T,Args&&...>::value> // 使用帮助类型
    construct(T* t,Args&&... args) 
{
    std::cout << "constructing non-trivially constructible T\n";
    new(t, detail::inplace_t{}) T(args...);
}
 
// #3 ，通过形参启用
template<class T>
void destroy(T* t, 
             typename std::enable_if<std::is_trivially_destructible<T>::value>::type* = 0) 
{
    std::cout << "destroying trivially destructible T\n";
}
 
// #4 ，通过模板形参启用
template<class T,
         typename std::enable_if<
             !std::is_trivially_destructible<T>{} &&
             (std::is_class<T>{} || std::is_union<T>{}),
            int>::type = 0>
void destroy(T* t)
{
    std::cout << "destroying non-trivially destructible T\n";
    t->~T();
}
 
// #5 ，通过模板形参启用
template<class T,
	typename = std::enable_if_t<std::is_array<T>::value> >
void destroy(T* t) // 注意，不修改函数签名
{
    for(std::size_t i = 0; i < std::extent<T>::value; ++i) {
        destroy((*t)[i]);
    }
}
/*
template<class T,
	typename = std::enable_if_t<std::is_void<T>::value> >
void destroy(T* t){} // 错误：与 #5 拥有相同签名
*/
 
// A 的部分特化通过模板形参启用
template<typename T, typename Enable = void>
class A {}; // 初等模板
 
template<typename T>
class A<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
public:
	void foo() {}
}; // 为浮点类型特化

template<typename Function>
std::enable_if_t<!std::is_member_function_pointer<Function>::value> foo(Function&& f) {

}

int main()
{
    std::aligned_union_t<0,int,std::string> u;
 
    construct(reinterpret_cast<int*>(&u));
    destroy(reinterpret_cast<int*>(&u));
 
    construct(reinterpret_cast<std::string*>(&u),"Hello");
    destroy(reinterpret_cast<std::string*>(&u));
 
    A<int> a1; // OK ，匹配初等模板
    A<float> a2; // OK ，匹配部分特化
	a2.foo();
	
	foo([] {}); // OK
	// foo(&A<float>::foo); // 通过std::enable_if_t来限定输入参数的类型必须为非成员函数

	return 0;
}
