#include <tuple>
#include <vector>
#include <iostream>

template <size_t... I>
std::vector<size_t> make_index_vector(std::index_sequence<I...>)
{
    return {I...};
}

template <typename F, typename T, size_t... I>
auto apply_impl(F f, const T& t, std::index_sequence<I...>) // integer_sequence<size_t, I...>
{
    return f(std::get<I>(t)...);    // 可变参数模板包展开
}

template <typename F, typename T>
auto apply(F f, const T& t)
{
    return apply_impl(f, t, std::make_index_sequence<std::tuple_size<T>::value>()); // 用std::tuple_size获取元组的元素个数n，再利用std::make_index_sequence生成索引下标0~n-1。
}

int test_func(int a, double b, const char* str)
{
    std::cout << str << std::endl;
    return a + b;
}

int main(int argc, char const *argv[])
{
    // demo 1
    // auto tuple = std::make_tuple(123, 'a', "adasd");
    // std::cout << std::get<0>(tuple) << std::endl;
    // std::cout << std::get<1>(tuple) << std::endl;
    // std::cout << std::get<2>(tuple) << std::endl;

    // // 下面是C++14的特性
    // std::cout << std::get<int>(tuple) << std::endl;
    // std::cout << std::get<char>(tuple) << std::endl;
    // std::cout << std::get<const char*>(tuple) << std::endl;

    // demo 2
    // auto vec = make_index_vector(std::make_index_sequence<10>());
    // for (const auto& i : vec) {
    //     std::cout << i << ' ';
    // }
    // std::cout << std::endl;

    // demo 3
    auto params = std::make_tuple(3, 4.5, "it's my apply implementation.");
    auto res = apply(test_func, params);
    std::cout << "result: " << res << std::endl;
    return 0;
}
