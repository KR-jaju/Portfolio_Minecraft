#pragma once

#include <array>
#include <queue>

template <typename T, size_t L>
class MultiLevelQueue {
public:
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = T const&;

    /*
    레벨 0이 제일 front
    */
    reference front()
    {
        for (size_type level = 0; level < L; ++level)
            if (!this->queues[level].empty())
                return (this->queues[level].front());
        throw std::out_of_range("All queues are empty");
    }
    const_reference front() const
    {
        for (size_type level = 0; level < L; ++level)
            if (!this->queues[level].empty())
                return (this->queues[level].front());
        throw std::out_of_range("All queues are empty");
    }
    reference back()
    {
        for (size_type level = L - 1; level >= 0; --level)
            if (!this->queues[level].empty())
                return (this->queues[level].back());
        throw std::out_of_range("All queues are empty");
    }
    const_reference back() const
    {
        for (size_type level = L - 1; level >= 0; --level)
            if (!this->queues[level].empty())
                return (this->queues[level].back());
        throw std::out_of_range("All queues are empty");
    }

    constexpr bool empty() const
    {
        for (size_type level = 0; level < L; ++level)
            if (!this->queues[level].empty())
                return (false);
        return (true);
    }

    constexpr size_type size() const
    {
        size_type total_size = 0;

        for (size_type level = 0; level < L; ++level)
            total_size += this->queues[level].size();
        return (total_size);
    }

    void push(T const& item, size_type level)
    {
        if (level >= L)
            throw std::out_of_range("Invalid level index");
        this->queues[level].push(item);
    }

    void push(T&& item, size_type level)
    {
        if (level >= L)
            throw std::out_of_range("Invalid level index");
        this->queues[level].push(std::move(item));
    }

    template <class... Args>
    decltype(auto) emplace(Args&&... args, size_type level)
    {
        return this->queues[level].emplace(std::forward<Args>(args)...);
    }

    void pop()
    {
        for (size_type level = 0; level < L; ++level)
        {
            if (!this->queues[level].empty())
            {
                this->queues[level].pop();
                return;
            }
        }
        throw std::out_of_range("All queues are empty");
    }

private:
    std::array<std::queue<T>, L> queues;
};
