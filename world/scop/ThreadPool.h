#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <future>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    enum class Priority {
        Immediate = 4,
        High = 3,
        Normal = 2,
        Low = 1,
        Background = 0
    };
    using JobID = uintptr_t;
private:
    class JobBase
    {
    public:
        virtual ~JobBase() = default;
        virtual void invoke() = 0;
    };
    template <typename F>
    class JobWrapper : public JobBase
    {
    public:
        JobWrapper(F&& callable) : callable(std::move(callable)) {}
        void invoke() { callable(); }
    private:
        F callable;
    };
    struct Job
    {
        std::unique_ptr<JobBase> job;
        Priority priority;

        template <typename F>
        Job(Priority priority, F callable) : job(std::make_unique<JobWrapper<F>>(std::move(callable))), priority(priority) {}
        JobID getID() const { return (reinterpret_cast<JobID>(this->job.get())); }
        void operator()() { this->job->invoke(); }
        bool operator<(Job const& other) const { return (static_cast<int>(this->priority) < static_cast<int>(other.priority)); }
    };
public:
    ThreadPool(size_t thread_count = std::thread::hardware_concurrency());
    ~ThreadPool();

    void join();
    bool joinable() const;

    bool cancel(JobID id);

    template <typename F>
    JobID enqueue(Priority priority, F&& callable)
    {
        if (this->stop_flag)
            throw std::runtime_error("ThreadPool »ç¿ë ÁßÁöµÊ");
        Job job(priority, std::move(callable));
        JobID job_id = job.getID();

        assert(job_id != 0); // nullptr job X
        {
            std::lock_guard<std::mutex> lock(this->mutex); 

            this->jobs.emplace(std::move(job));
            this->is_pending.emplace(job_id, true);
        }
        this->barrier.notify_one();
        return (job_id);
    }
private:
    //template <typename F>
    //Job(Priority priority, F job) : priority(priority), job(std::make_unique<JobWrapper<F>>(std::move(job))) {}
    //Job(Job const&) = delete;
    //Job& operator=(Job const&) = delete;
    //Job(Job&& job) = default;
    //Job& operator=(Job&& job) = default;
    //bool operator<(Job const& other) const { return (static_cast<int>(this->priority) < static_cast<int>(other.priority)); }
    //void operator()() const { this->job->invoke(); }
    std::vector<std::thread> workers;
    std::priority_queue<Job> jobs;
    std::unordered_map<JobID, bool> is_pending;
    std::condition_variable barrier;
    mutable std::mutex mutex;
    bool stop_flag;

    void workerMain();
};
