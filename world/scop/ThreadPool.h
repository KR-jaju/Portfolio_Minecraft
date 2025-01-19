#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(size_t threadCount = std::thread::hardware_concurrency());
    //void enqueue(std::function<void()> fn);
    void enqueue(std::function<void()>&& fn);
    ~ThreadPool();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stopFlag;
};