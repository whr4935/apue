#include <c_plus_plus_primer.h>
#include <atomic>
#include <thread>


static void test1()
{
    std::atomic_flag f = ATOMIC_FLAG_INIT;
    f.clear();
    std::cout << f.test_and_set() << std::endl;
    
}

/////////////////////////////////////////////////////////
namespace t2
{
std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y()
{
    x.store(true, std::memory_order_relaxed);
    y.store(true, std::memory_order_relaxed);
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_relaxed));
    if (x.load(std::memory_order_relaxed))
        ++z;
}

void t2()
{
    int i = 100;
    while (i--) {
        x = false;
        y = false;
        z = 0;
        std::thread a(write_x_then_y);
        std::thread b(read_y_then_x);
        a.join();
        b.join();
        assert(z.load() != 0);
    }
}

};


/////////////////////////////////////////////////////////
int test_atomic()
{
    //test1();
    t2::t2();

    return 0;
}
