#include <mutex>
#include <atomic>
#include <iostream>
#include <thread>
#include <chrono>


template<typename T>
class Singleton
{
public:
    static T* instance() {
        T* t = sInstance.load(std::memory_order_acquire);
        if (t == nullptr) {
            std::lock_guard<std::mutex> l(mLock);
            t = sInstance.load(std::memory_order_relaxed);
            if (t == nullptr) {
                t = new T;
                sInstance.store(t, std::memory_order_release);
            }
        }
        return t;
    }

private:
    Singleton() = delete;
    ~Singleton() = delete;

    static std::atomic<T*> sInstance;
    static std::mutex mLock;
};

template<typename T>
std::atomic<T*> Singleton<T>::sInstance;

template<typename T>
std::mutex Singleton<T>::mLock;

struct Test
{
    Test() {
        std::cout << "test ctor" <<std::endl;
    }

    void doWork()
    {
    }
};

int test_signleton()
{
    auto t1 = std::chrono::high_resolution_clock::now();

    Test* p = Singleton<Test>::instance();
    std::thread t([] {
        Singleton<Test>::instance()->doWork();
    });

    p->doWork();
    t.join();

    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "cost: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << std::endl;


    return 0;
}

