#include <c++/thread>
#include <c++/future>
#include <c++/iostream>

/**
 * compiler: MinGW64 
 */

void t_task() noexcept
{
    uint32_t cnt = 0;
    for(size_t i = 0; i < 100; i++)
    {
        ++cnt;
        // if (cnt % 10 == 0)
        //     std::cout << cnt << std::endl;
    }
    std::cout << "thread " << std::this_thread::get_id() << ": exit" << std::endl;
}

/**
 * std::thread对象在析构时，如果该对象是可联结的（joinable），即使线程执行函数已完成也导致程序的终止。
 * [参考] Effective Modern C++. 条款37：使std::thread类型对象在所有路径皆不可联结。
 */
void joinable_in_destruct()
{
    try {
        std::thread t1(t_task);
        std::cout << "finish construct std::thread" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    std::cout << "out of thread scope" << std::endl;
}

/************* async *************/

int f_task(int a, int b) noexcept
{
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    return a + b;
}

template <typename F, typename... Ts>
inline std::future<typename std::result_of<F(Ts...)>::type>
reallyAsync(F&& f, Ts&&... params)
{
    return std::async(std::launch::async, std::forward<F>(f), std::forward<Ts>(params)...);
}

/**
 * 1. 优先使用std::async，而非std::thread的设计。async允许调度器把指定函数运行在请求函数结果的线程中(async对象对应策略为std::launch::deferred，
 *    对future对象调用了get或者wait的线程)，而不一定创建一个软件线程。
 * 2. future对象如果是由std::async异步启动的<共享状态>的最后一个引用，在future的析构函数中会保持阻塞，直到该异步任务结束。
 * [参考] Effective Modern C++. 条款35, 36
 * [参考] Effective Modern C++. 条款38：对变化多端的线程句柄析构函数行为保持关注
 */
void async_and_future()
{
    using namespace std::literals;  // 使用C++14持续时长后缀
    auto fut = reallyAsync(f_task, 5, 6);
    std::cout << "finish construct std::async" << std::endl;
    while (fut.wait_for(500ms) != std::future_status::ready) {
        std::cout << '.';
        std::cout.flush();
    }
    std::cout << "\nget result: " << fut.get() << std::endl;
}

int main(int argc, char const *argv[])
{
    joinable_in_destruct();
    async_and_future();
    return 0;
}
 