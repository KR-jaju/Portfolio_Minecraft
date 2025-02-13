#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <stdexcept>
#include "Future.h"
#include "ControlBlock.h"

template<typename T>
class Promise {
public:
    Promise() : control_block(new ControlBlock()), future_retrieved(false) {}
    Promise(Promise&& other) noexcept : control_block(other.control_block), future_retrieved(other.future_retrieved)
    {
        other.control_block = nullptr;
        other.future_retrieved = false;
    }
    Promise& operator=(Promise&& other) noexcept
    {
        if (this == &other)
            return (*this);
        this->cleanup();
        this->control_block = other.control_block;
        other.control_block = nullptr;
        return *this;
    }
    Promise(const Promise&) = delete;
    Promise& operator=(const Promise&) = delete;
    ~Promise()
    {
        this->cleanup();
    }

    Future<T> getFuture()
    {
        if (this->control_block == nullptr)
            throw std::runtime_error("Promise is invalid or already moved");
        if (this->future_retrieved)
            throw std::runtime_error("Future has already been retrieved");

        this->future_retrieved = true;
        return Future<T>(this->control_block);
    }

    void setValue(const T& val)
    {
        ControlBlock* cb = this->control_block;
        {
            std::lock_guard<std::mutex> lock(this->control_block->mutex);

            if (cb->state == ControlBlock::State::Ready)
                throw std::runtime_error("value already set");
            if (cb->state == ControlBlock::State::Broken)
                throw std::runtime_error("broken_promise");
            if (cb->state == ControlBlock::State::Pending)
            {
                this->control_block->value = val;
                this->control_block->state = ControlBlock::State::Ready;
            }
        }
        this->control_block->cv.notify_all();
    }

    void setValue(T&& val)
    {
        ControlBlock* cb = this->control_block;
        {
            std::lock_guard<std::mutex> lock(this->control_block->mutex);

            if (cb->state == ControlBlock::State::Ready)
                throw std::runtime_error("value already set");
            if (cb->state == ControlBlock::State::Broken)
                throw std::runtime_error("broken_promise");
            if (cb->state == ControlBlock::State::Pending)
            {
                this->control_block->value = std::move(val);
                this->control_block->state = ControlBlock::State::Ready;
            }
        }
        this->control_block->cv.notify_all();
    }
private:
    using ControlBlock = ControlBlock<T>;

    ControlBlock* control_block;
    bool future_retrieved;

    void    cleanup()
    {
        ControlBlock* cb = this->control_block;

        if (cb == nullptr)
            return;
        {
            std::lock_guard<std::mutex> lock(cb->mutex);

            if (cb->state == ControlBlock::State::Pending)
                cb->state = ControlBlock::State::Broken;
        }
        cb->cv.notify_all();
        if (cb->ref_count.fetch_sub(1, std::memory_order_acq_rel) == 1)
            delete cb;
    }
};
