#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <set>
#include <string>

/**
 * 探讨和记录有关万能引用进行重载的方案
 */

std::multiset<std::string> names;   // 全局数据结构

template <typename S>
void log(std::chrono::system_clock::time_point now, S&& tag)
{
    using namespace std::chrono;
    time_t t = system_clock::to_time_t(now);    // 转换成c接口 time_t
    std::tm tm = *std::localtime(&t);   // std::localtime返回的指针不需要delete。因为它指向的是某个static对象，所以localtime是线程不安全的。
    auto time = std::put_time(&tm, "%b %d %Y %H:%M:%S");    // put_time可以把std::tm变成一个特殊的对象。
    std::cout << "[" << time << "] " << tag << std::endl;
}

/**
 * 原型。效率不尽如人意
 */
// void logAndAdd(const std::string& name)
// {
//     auto now = std::chrono::system_clock::now();
//     log(now, "logAndAdd");  // 制备日志条目
//     names.emplace(name);
// }

/**
 * 无法处理重载
 */
// template <typename T>
// void logAndAdd(T&& name)
// {
//     auto now = std::chrono::system_clock::now();
//     log(now, "logAndAdd");
//     names.emplace(std::forward<T>(name));
// }

std::string nameFromIndex(int idx) noexcept
{
    return std::to_string(idx);
}

/*************** 1. 标签分派 ***************/
template <typename T>
void logAndAdd(T&& name);   // forward declaration

template <typename T>
void logAndAddImpl(T&& name, std::false_type)
{
    auto now = std::chrono::system_clock::now();
    log(now, name);
    names.emplace(std::forward<T>(name));
}

void logAndAddImpl(int idx, std::true_type)
{
    logAndAdd(nameFromIndex(idx));
}

template <typename T>
void logAndAdd(T&& name)
{
    logAndAddImpl(std::forward<T>(name), 
        std::is_integral<std::remove_reference_t<T>>()
    );
}

/*************** 2. 对万能引用的模板施加限制 ***************/
class Person
{
public:
    // template <typename T>    // 1)无限制版本，导致无法调用复制和移动构造函数
    // template <               // 2)使用is_same版本
    //     typename T, 
    //     typename = typename std::enable_if_t<
    //         !std::is_same<Person,       // std::is_same判断两个类型是否一致，但是无法判定派生关系
    //                     std::decay_t<T> // std::decay<T>用来移除T带有的引用、const、volatile修饰词
    //                     >::value
    //     >
    // >
    template <                  // 3)使用is_base_of版本
        typename T, 
        typename = typename std::enable_if_t<
            !std::is_base_of<Person,       // std::is_same判断两个类型是否一致，但是无法判定派生关系
                            std::decay_t<T> // std::decay<T>用来移除T带有的引用、const、volatile修饰词
                            >::value
            &&
            !std::is_integral<std::remove_reference_t<T>>::value    // 如果不是整形...
        >
    >
    explicit Person(T&& n)
        : name(std::forward<T>(n)) { display(); }

    explicit Person(int idx)
        : name(nameFromIndex(idx)) { display(); }

    Person(const Person& rhs) = default;
    Person(Person&& rhs) = default;

private:
    void display()
    { std::cout << name << std::endl; }

private:
    std::string name;
};

class SpecialPerson : public Person
{
public:
    SpecialPerson() : Person(count++) {}
    SpecialPerson(const SpecialPerson& rhs)
        : Person(rhs) {}
    SpecialPerson(SpecialPerson&& rhs)
        : Person(std::move(rhs)) {}

    static int count;
};

int SpecialPerson::count = 333;

int main(int argc, char const *argv[])
{
    logAndAdd("Patty Dog");
    logAndAdd(7777);
    
    Person q((short)8888);
    Person p("Nancy");
    auto CloneOfP(p);

    SpecialPerson sp;
    auto CloneOfSp(sp);
    return 0;
}

/**
 * 引用折叠：
 * 如果任一引用为左值引用，则结果为左值引用。否则（即两个皆为右值引用），结果为右值引用。
 * 
 * 对于T&& p, 如果实参为左值，则p类型为Person&，T则为Person&；
 * 如果实参为右值，则p类型为Person，T则为Person。
 * 
 * std::forward实现的原理正是使用了引用折叠。
 */