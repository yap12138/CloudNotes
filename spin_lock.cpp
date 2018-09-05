#include <atomic>
#include <thread>
#include <iostream>
#include <sys/time.h>
#include <pthread.h>

// choose implementation.
// #define PTHREAD_WAY
 
class spin_lock {
private:
    std::atomic<bool> flag = ATOMIC_VAR_INIT(false);
public:
    spin_lock() = default;
    spin_lock(const spin_lock&) = delete;
    spin_lock& operator=(const spin_lock) = delete;

    void lock(){   //acquire spin lock
        bool expected = false;
        while(!flag.compare_exchange_strong(expected, true))
            expected = false;
    }

    void unlock(){   //release spin lock
        flag.store(false);
    }   
};

int num = 0;

#if defined(PTHREAD_WAY)
pthread_spinlock_t spin_lock;
#else
spin_lock sm;
#endif // PTHREAD_WAY
 
void thread_proc()
{
    for(int i=0; i<10000000; ++i){
        #if defined(PTHREAD_WAY)
        pthread_spin_lock(&spin_lock);
        ++num;
        pthread_spin_unlock(&spin_lock);
        #else
        sm.lock();
        ++num;
        sm.unlock();
        #endif // PTHREAD_WAY
    }   
}

int64_t get_current_timestamp()
{
    struct timeval now = {0, 0};
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000 * 1000 + now.tv_usec;
}

int main(int argc, char const *argv[])
{
    #if defined(PTHREAD_WAY)
    pthread_spin_init(&spin_lock, PTHREAD_PROCESS_PRIVATE);
    #endif // PTHREAD_WAY

    int64_t start = get_current_timestamp();
 
    std::thread t1(thread_proc), t2(thread_proc);
    t1.join();
    t2.join();
 
    std::cout << "num: " << num << std::endl;
    int64_t end = get_current_timestamp();
    std::cout << "cost: " << end-start << std::endl;

    #if defined(PTHREAD_WAY)
    pthread_spin_destroy(&spin_lock);
    #endif // PTHREAD_WAY

    return 0;
}

// 总结，对于那些仅需要阻塞很短时间的场景，spin lock效率高于mutex，
// 但是在使用spin lock的临界区内，不可以有sleep操作，否则会造成死锁。
// mutex适用于那些可能会阻塞很长时间的场景。
// 可用c++11的 atomic来实现spin lock，但效率远远低于pthread提供的自旋锁。