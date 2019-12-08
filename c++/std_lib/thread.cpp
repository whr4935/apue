#include <c_plus_plus_primer.h>
#include <thread>
#include <chrono>
#include <future>

void fun(int& v)
{
    std::cout << "begin thread..." << std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << ++v << std::endl;
    std::cout << "thread exit..." << std::endl;
}

class A 
{
public:
    int print(int v = 0) {
        std::cout << this << std::endl;
        std::cout << "A::print " << v << std::endl;
    }
};

static void test1()
{
    int v = 10;
    std::thread t(fun, std::ref(v));
    auto f = std::mem_fn(&std::thread::join);
    f(&t);
    //t.detach();
    //t.join();
    std::cout << "orign v: " << v << std::endl;

    auto f2 = std::mem_fn(&A::print);
    A a;
    std::cout << "address of a: " << &a << std::endl;
    f2(a, 4);
}

//////////////////////////////////////////////////////
template <typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T& result)
    {
        result = std::accumulate(first, last, result);
    }
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = std::distance(first, last);

    if (!length)
        return init;
    
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length+min_per_thread-1)/min_per_thread;

    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads!=0?hardware_threads:2, max_threads);
    unsigned long const block_size = length/num_threads;

    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads-1);

    Iterator block_start = first;
    for (unsigned long i = 0; i < num_threads-1; ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(accumulate_block<Iterator,T>(),
                block_start, block_end, std::ref(results[i]));
        block_start = block_end;
    }

    accumulate_block<Iterator,T>()(block_start, last, results[num_threads-1]);
    std::for_each(threads.begin(), threads.end(), std::bind(&std::thread::join, std::placeholders::_1));
    //std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    return std::accumulate(results.begin(), results.end(), init);
}


static void test2()
{
    std::vector<int> v;
    //std::cout << v.size() << std::endl;
    for (int i = 0; i < 1024*1024; ++i) {
        v.push_back(rand());
    }

    int init = 0;
    //std::cout << "accumulate: " << std::accumulate(v.begin(), v.end(), init) << std::endl;
    std::cout << "parallel_accumulate: " << parallel_accumulate(v.begin(), v.end(), init) << std::endl;
}

//////////////////////////////////////////////////////
static void foo()
{
    thread_local int n = 9;
    std::cout << std::this_thread::get_id() << "   " << ++n << "  " << &n << std::endl;

}

static void test3()
{
    foo();
    foo();
    std::this_thread::sleep_for(std::chrono::microseconds(10));

    std::thread t1(foo); 
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::thread t2(foo);

    t1.join();
    t2.join();

    std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> t;
    std::cout << t.time_since_epoch().count() << std::endl;
}

///////////////////////////////////////
// test thread                       //
///////////////////////////////////////
int test_thread()
{
    std::cout << std::endl;
    test1();
    //std::cout << "test1 return" << std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(2));

    //std::cout << "hardware_concurrency: " << std::thread::hardware_concurrency() << std::endl;
    //std::cout << "thread ID: " << std::this_thread::get_id() << std::endl;

    std::future<int> future = std::async(std::launch::async, [] {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "async task finished!" << std::endl;
                std::cout << "thread id: " << std::this_thread::get_id() << std::endl;
                return 10;
            });

    std::cout << std::this_thread::get_id() << " get result: " << future.get() << std::endl;

    return 0;
}


int main(int argc, char *argv[])
{
    
    return 0;
}
