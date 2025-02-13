#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <stdexcept>
#include "ControlBlock.h"

template<typename T> class Promise;

template<typename T>
class Future {
public:
    Future() :control_block(nullptr) {}
    Future(Future&& other) noexcept : control_block(other.control_block)
    {
        other.control_block = nullptr;
    }
    Future& operator=(Future&& other) noexcept
    {
        if (this == &other)
            return (*this);
        this->cleanup();
        this->control_block = other.control_block;
        other.control_block = nullptr;
        return *this;
    }
    Future(const Future&) = delete;
    Future& operator=(const Future&) = delete;
    ~Future()
    {
        this->cleanup();
    }
    T get()
    {
        ControlBlock* cb = this->control_block;

        if (cb == nullptr)
            throw std::runtime_error("Invalid future (no shared state)");
        std::unique_lock<std::mutex> lock(cb->mutex);

        cb->cv.wait(lock, [this, cb] { return cb->state != ControlBlock::State::Pending; });
        if (cb->state == ControlBlock::State::Broken)
            throw std::runtime_error("broken_promise");
        if (cb->state == ControlBlock::State::Canceled)
            throw std::runtime_error("Future was canceled");
        return *(cb->value);
    }
    bool cancel()
    {
        ControlBlock* cb = this->control_block;
        std::lock_guard<std::mutex> lock(cb->mutex);

        if (cb->state != ControlBlock::State::Pending)
            return (false);
        cb->state = ControlBlock::State::Canceled;
        return (true);
    }
    bool valid() const
    {
        return (this->control_block != nullptr);
    }
    bool isDone() const
    {
        ControlBlock* cb = this->control_block;
        std::lock_guard<std::mutex> lock(cb->mutex);

        return (cb->state == ControlBlock::State::Ready);
    }
private:
    friend class Promise<T>;
    using ControlBlock = ControlBlock<T>;
    
    ControlBlock* control_block;

    explicit Future(ControlBlock* control_block) : control_block(control_block)
    {
        control_block->ref_count.fetch_add(1, std::memory_order_relaxed);
    }
    void    cleanup()
    {
        ControlBlock* cb = this->control_block;

        if (cb == nullptr)
            return;
        this->cancel();
        if (cb->ref_count.fetch_sub(1, std::memory_order_acq_rel) == 1)
            delete this->control_block;
    }
};