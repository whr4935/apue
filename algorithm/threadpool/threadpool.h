#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool {
    using Task = std::function<void()>;
    std::vector<std::thread> pool;
    std::queue<Task> tasks;
    std::mutex mLock;
    std::condition_variable cvTask;
    std::atomic<bool> stoped;
    std::atomic<int> idlThrNum;

public:
    ThreadPool(unsigned short size = 4)
        : stoped(false)
    {
        idlThrNum = size < 1 ? 1 : size;
        for (size = 0; size < idlThrNum; ++size) {
            pool.emplace_back([this] {
                while (!this->stoped) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->mLock);
                        this->cvTask.wait(lock, [this] {
                            return this->stoped.load() || !this->tasks.empty();
                        });
                        if (this->stoped && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    idlThrNum--;
                    std::cout << "do work:" << std::this_thread::get_id() << std::endl;
                    task();
                    idlThrNum++;
                }
            });
        }
    }

    ~ThreadPool()
    {
        stoped.store(true);
        cvTask.notify_all();
        for (std::thread& thread : pool) {
            if (thread.joinable())
                thread.join();
        }
    }

    template <typename F, typename... Args>
    auto commit(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
    {
        if (stoped.load())
            throw std::runtime_error("commit on ThreadPool is stoped!");

        using RetType = decltype(f(args...));
        auto task = std::make_shared<std::packaged_task<RetType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<RetType> future = task->get_future();
        {
            std::lock_guard<std::mutex> lock(mLock);
            tasks.emplace(
                [task] {
                    (*task)();
                });
        }
        cvTask.notify_one();
        return future;
    }
};

#endif
