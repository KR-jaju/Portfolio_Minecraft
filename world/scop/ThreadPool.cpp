#include "pch.h"
#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t thread_count)
    : stop_flag(false)
{
    workers.reserve(thread_count);
    for (size_t i = 0; i < thread_count; ++i)
        workers.emplace_back([this]() { this->workerMain(); });
    this->is_pending.max_load_factor(0.7);
}

ThreadPool::~ThreadPool()
{
    if (this->joinable())
        this->join();
}

void ThreadPool::join()
{
    {
        std::unique_lock<std::mutex> lock(this->mutex);

        this->stop_flag = true;
    }
    this->barrier.notify_all();
    for (auto& t : this->workers)
    {
        t.join();
    }
}

bool ThreadPool::joinable() const
{
    std::unique_lock<std::mutex> lock(this->mutex);
    
    return (!this->stop_flag);
}

/*
Pending일 때만 취소 가능
*/
bool ThreadPool::cancel(JobID id)
{
    std::unique_lock<std::mutex> lock(this->mutex);

    if (this->is_pending.count(id) == 0) // 잡이 큐에서 대기 중이지 않다면
        return (false); // 취소 실패
    this->is_pending.at(id) = false; // Lazy deletion
    return (true); // 취소 성공
}

void ThreadPool::workerMain()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(this->mutex);
        wait_for_job:
        this->barrier.wait(lock, [this]() { return !this->jobs.empty() || this->stop_flag; });
        if (this->stop_flag && this->jobs.empty()) // stop_flag가 true고 일이 모두 처리됐다면
            return;
        Job job = std::move(const_cast<Job&>(this->jobs.top()));
        JobID job_id = job.getID();

        this->jobs.pop();
        while (!this->is_pending.at(job_id)) // 취소됐다면?
        {
            this->is_pending.erase(job_id); // 상태 제거
            if (this->jobs.empty()) // 더 이상 뽑을 잡이 없음
                goto wait_for_job;
            job = std::move(const_cast<Job&>(this->jobs.top()));
            job_id = job.getID(); // 다음 잡 뽑기
            this->jobs.pop();
        }
        this->is_pending.erase(job_id); // 상태 제거
        lock.unlock();
        job(); //TODO : 익셉션 처리 만들어야함
    }
}
