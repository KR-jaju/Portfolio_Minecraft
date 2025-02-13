#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <stdexcept>

template <typename T>
struct ControlBlock
{
    enum class State
    {
        Pending,
        Ready,
        Canceled,
        Broken
    };
    std::atomic<int> ref_count{ 1 };
    std::mutex mutex;
    std::condition_variable cv;
    std::optional<T> value;
    State state = State::Pending;
};