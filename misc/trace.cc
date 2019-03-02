#include <iostream>
#include <chrono>
#include <thread>

#define TRACE() Logger __flogger(__FUNCTION__, __LINE__)

struct Logger
{
    Logger(const char* func, const int line)
    : mFunc(func)
    , mLine(line)
    {
        mClock = std::chrono::high_resolution_clock::now();
        std::cout << func << "():" << line << " >> begin" << std::endl;
    }

    ~Logger() {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                (std::chrono::high_resolution_clock::now() - mClock)
                );
        std::cout << mFunc << "():" << mLine << " << end " << duration.count() << " ms" << std::endl;
    }

private:
    const char* mFunc;
    const int mLine;
    std::chrono::high_resolution_clock::time_point mClock;
};

int main(int argc, char *argv[])
{
    TRACE();

    std::cout << "hello " << std::endl;    
    std::this_thread::sleep_for(std::chrono::microseconds(5000));
    return 0;
}
