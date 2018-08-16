#include <iostream>
#include <stdexcept>
 
// C++11 constexpr 函数使用递归而非迭代
// （ C++14 constexpr 函数可使用局部变量和循环）
constexpr int factorial(int n)
{
    return n <= 1? 1 : (n * factorial(n - 1));
}
 
// 字面类
class conststr {
    const char* p;
    std::size_t sz;
public:
    template<std::size_t N>
    constexpr conststr(const char(&a)[N]): p(a), sz(N - 1) {}
 
    // constexpr 函数通过抛异常来提示错误
    // C++11 中，它们必须用条件运算符?:这么做
    constexpr char operator[](std::size_t n) const
    {
        return n < sz ? p[n] : throw std::out_of_range("");
    }
    constexpr std::size_t size() const { return sz; }
};
 
// C++11 constexpr 函数必须把一切放在单条 return 语句中
// （ C++14 无该要求）
constexpr std::size_t countlower(conststr s, std::size_t n = 0,
                                             std::size_t c = 0)
{
    return n == s.size() ? c :
           'a' <= s[n] && s[n] <= 'z' ? countlower(s, n + 1, c + 1) :
                                        countlower(s, n + 1, c);
}
 
// 输出函数要求编译时常量，以测试
template<int n>
struct constN
{
    constN() { std::cout << n << '\n'; }
};
 
int main()
{
    std::cout << "4! = " ;
    constN<factorial(4)> out1; // 在编译时计算
 
    volatile int k = 8; // 不允许使用 volatile 者优化
    std::cout << k << "! = " << factorial(k) << '\n'; // 运行时计算
 
    std::cout << "the number of lowercase letters in \"Hello, world!\" is ";
    constN<countlower("Hello, world!")> out2; // 隐式转换到 conststr

    static constexpr int N = 3; //静态常量， 去掉static则错误
    constexpr const int *NP = &N;

    static int j = 0;
    static constexpr int i = 2;
    constexpr const int *p = &i; // const pointer, const data
    constexpr int *p1 = &j; // const pointer, non-const data

    return 0;
}