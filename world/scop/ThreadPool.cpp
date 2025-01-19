#include "pch.h"
#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadCount)
    : stopFlag(false)
{
    for (size_t i = 0; i < threadCount; ++i)
    {
        workers.emplace_back([this]() {
            while (true)
            {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(queueMutex);

                    condition.wait(lock, [this]() {
                        return stopFlag || !tasks.empty();
                        });
                    if (stopFlag && tasks.empty())
                        return;
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
            });
    }
}
//void ThreadPool::enqueue(std::function<void()> fn)
//{
//    {
//        std::unique_lock<std::mutex> lock(queueMutex);
//
//        tasks.emplace(std::move(fn));
//    }
//    condition.notify_one();
//}

void ThreadPool::enqueue(std::function<void()>&& fn)
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);

        tasks.emplace(std::move(fn));
    }
    condition.notify_one();
}


ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);

        stopFlag = true;
    }
    condition.notify_all();
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

