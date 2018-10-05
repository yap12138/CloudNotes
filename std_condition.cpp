#include <condition_variable>
#include <mutex>
#include <thread>
#include <future>
#include <iostream>
#include <atomic>
#include <vector>

std::mutex mtx;
// 两个条件变量方式，适用于任意多个线程
std::condition_variable wa;
std::condition_variable wb;
std::promise<void> ready;

std::atomic<bool> a_ready = ATOMIC_VAR_INIT(false);

// 单条件变量方式,仅仅适用于2个线程
std::atomic_bool a_turn = ATOMIC_VAR_INIT(true);
std::condition_variable cond;

// 使用future<void>通信启动
void print_a()
{
    // ready.get_future().get();
    for (int i = 0; i < 10; ++i) 
    {
        std::unique_lock<std::mutex> lock(mtx);
        wb.wait(lock, [](){ return a_ready == false; });

        std::cout << "A ";

        a_ready = true;
        lock.unlock();
        wa.notify_one();
    }
}

void print_b()
{
    for (int i = 0; i < 10; ++i)
    {
        std::unique_lock<std::mutex> lock(mtx);
        wa.wait(lock, []() { return a_ready == true; });

        std::cout << "B ";

        a_ready = false;
        lock.unlock();
        wb.notify_one();
    }
    // std::cout << std::endl;
}

void print_a_simple()
{
    auto flag = ready.get_future(); // get_future只能调用一次，否则引发future_error异常
    if (flag.valid())   // 判断共享状态是否可用（在第一次调用get之前为true）
        flag.get();     // 获取并等待条件发生（promise设置值/异常前调用将阻塞当前线程）
    for (int i = 0; i < 10; ++i)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cond.wait(lock, []() { return a_turn == true; });
        
        std::cout << "A ";

        a_turn = false;
        cond.notify_one();
    }
}

void print_b_simple()
{
    for (int i = 0; i < 10; ++i)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cond.wait(lock, []() { return a_turn == false; });
        
        std::cout << "B ";

        a_turn = true;
        cond.notify_one();
    }
    // std::cout << std::endl;
}

void alternate_print()
{
    // 两个线程轮流打印A，B字符
    std::thread ta(print_a_simple), tb(print_b_simple);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    ready.set_value();  // 一次性事件通信，使用std::future<void>。[参考] Effective Modern C++. 条款39

    ta.join();
    tb.join();
}

void alternate_print(int thread_cnt)
{
    std::vector<std::thread> contains;
    for (int i = 0; i < thread_cnt; ++i)
    {
        if ((i&1) == 0) 
            contains.emplace_back(print_a);
        else
            contains.emplace_back(print_b);
    }
    ready.set_value();

    for (auto&& t : contains)
    {
        t.join();
    }
}

int main(int argc, char const *argv[])
{
    // alternate_print();
    alternate_print(4);
    return 0;
}
