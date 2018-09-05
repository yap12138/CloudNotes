#include <atomic>
#include <thread>
#include <iostream>
#include <sys/time.h>
#include <pthread.h>
#include <mutex>

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
        while(flag.exchange(true))
            ;
    }

    void unlock(){   //release spin lock
        flag.store(false);
    }   
};

class spin_mutex {
  std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
  spin_mutex() = default;
  spin_mutex(const spin_mutex&) = delete;
  spin_mutex& operator= (const spin_mutex&) = delete;

  void lock() {
    while(flag.test_and_set(std::memory_order_acquire))
      ;
  }

  void unlock() {
    flag.clear(std::memory_order_release);
  }
};

int num = 0;

#if defined(PTHREAD_WAY)
pthread_spinlock_t spin_lock;
#elif defined(ATOMIC_WAY)
spin_lock sm;
#else
spin_mutex sm;
#endif // PTHREAD_WAY
 
void thread_proc()
{
    for(int i=0; i<10000000; ++i){
        #if defined(PTHREAD_WAY)
        pthread_spin_lock(&spin_lock);
        ++num;
        pthread_spin_unlock(&spin_lock);
        #elif defined(ATOMIC_WAY)
        sm.lock();
        ++num;
        sm.unlock();
        #else
        std::lock_guard<spin_mutex> lock(sm);
        ++num;
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
// C++11并不要求std::atomic的实现必须是无锁的，如果自旋锁内部的flag不是无锁的类型那么这个自旋锁就没有存在的意义了
// C++11提供了一个无锁的二值(bool)原子类型std::atomic_flag。使用std::atomic_flag就可以实现一个真正有用的自旋锁
// 效率： pthread_spin_lock > spin_mutex > spin_lock