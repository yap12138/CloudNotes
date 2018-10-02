#include <thread>
#include <mutex>    // std::mutex, std::unique_lock
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>   // std::mem_fn

std::mutex g_lock;

/**
 * 1+2+ .. + 1000的多线程实现
 */
void add(int& num, int& sum)
{
    while (true) {
        std::lock_guard<std::mutex> lock(g_lock);
        if (num < 1000) {
            num++;
            sum += num;
        }
        else {
            break;
        }
    }
}

/**
 * std::lock_guard是RAII模板类的简单实现，功能简单
 */
void using_lock_guard()
{
    int num = 0;
    int sum = 0;
    std::vector<std::thread> vec;
    for (int i = 0; i < 10; ++i) {
        std::thread t = std::thread(add, std::ref(num), std::ref(sum));
        vec.emplace_back(std::move(t));
    }
    /*
    但是在C++ 11之后，mem_fun和mem_fun_ref已经被标记为deprecated，即将被废弃，取而代之的是使用mem_fn。之所以不再使用mem_fun和mem_fun_ref,是因为它们的一下缺点：

    1) 它们只能处理那些只接受一个参数或者没有参数的成员函数,而mem_fn可以处理任意个参数的情况；
    2) 根据处理的是对象指针还是对象本身需要在mem_fun和mem_fun_ref之间做选择，而mem_fn可以处理这两种情况；
    */
    std::for_each(vec.begin(), vec.end(), std::mem_fn(&std::thread::join));
    std::cout << sum << std::endl;
}

/****************** std::unique_lock ******************/

std::mutex mtx;
std::once_flag flag;    //只执行一次，配合call_once使用

void print_block(int n, char ch)
{
    using namespace std::literals;
    // unique_lock有多组构造函数, 这里std::defer_lock不设置锁状态
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
    // 尝试加锁, 如果加锁成功则执行
    if (lock.try_lock()) {
        std::cout << ch << std::endl;
        std::this_thread::sleep_for(1ms);   // 让其他线程得到运行机会，方便测试
    }
}

void run_one(int& n)
{
    std::call_once(flag, [&n]{ n++; }); // 只执行一次, 适合延迟加载;
}

void using_unique_lock()
{
    std::vector<std::thread> vec;
    int num = 0;
    char ch = '1';
    for (auto i = 0; i < 10; ++i) {
        vec.emplace_back(print_block, 500, ch++);
        vec.emplace_back(run_one, std::ref(num));
    }
    for (auto& t : vec) {
        t.join();
    }
    std::cout << num << std::endl;
}

/****************** 使用切换场合 ******************/
struct bank_account {
    explicit bank_account(int balance) : balance(balance) {}
    int balance;
    std::mutex m;
};
 
void transfer(bank_account &from, bank_account &to, int amount)
{
    // 锁定两个互斥而不死锁，用免死锁算法避免死锁。
    std::lock(from.m, to.m);
    // 保证二个已锁定互斥在作用域结尾解锁
    std::lock_guard<std::mutex> lock1(from.m, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(to.m, std::adopt_lock);
 
// 等价方法：
//    std::unique_lock<std::mutex> lock1(from.m, std::defer_lock);
//    std::unique_lock<std::mutex> lock2(to.m, std::defer_lock);
//    std::lock(lock1, lock2);
 
    from.balance -= amount;
    to.balance += amount;
}

void diff_transfer()
{
    bank_account my_account(100);
    bank_account your_account(50);
 
    std::thread t1(transfer, std::ref(my_account), std::ref(your_account), 10);
    std::thread t2(transfer, std::ref(your_account), std::ref(my_account), 5);
 
    t1.join();
    t2.join();

    std::cout << "my account: " << my_account.balance << std::endl;
    std::cout << "your account: " << your_account.balance << std::endl;
}

int main(int argc, char const *argv[])
{
    // using_lock_guard();
    // using_unique_lock();
    diff_transfer();
    return 0;
}
