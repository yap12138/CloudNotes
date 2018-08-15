#include <iostream>
#include <vector>
 
struct Noisy
{
    Noisy() { std::cout << "constructed\n"; }
    Noisy(const Noisy&) { std::cout << "copy-constructed\n"; }
    Noisy(Noisy&&) { std::cout << "move-constructed\n"; }
    ~Noisy() { std::cout << "destructed\n"; }
};
 
std::vector<Noisy> f()
{
    std::vector<Noisy> v = std::vector<Noisy>(3); // 从临时量 (C++17 前)
                                                  // 从纯右值 (C++17 起)
                                                  // 初始化 v 时复制消除
    return v; // 从 v 到结果对象的 NRVO （ C++17 中不保证）
}             // 若禁用优化，则调用移动构造函数
 
void g(std::vector<Noisy> arg)
{
    std::cout << "arg.size() = " << arg.size() << '\n';
}
 
int main()
{
    std::vector<Noisy> v = f(); // 从临时量 (C++17 前)
                                // 从纯右值 (C++17 起)
                                // 初始化 v 时复制消除
    g(f());                     // 从 f() 返回的临时量 (C++17 前)
                                // 从纯右值 f() (C++17 起)
                                // 初始化 g() 的形参时复制消除
}