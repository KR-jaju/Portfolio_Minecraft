#pragma once

#include <iostream>
#include <list>
#include <unordered_map>

template <typename Key, typename T>
class OrderedMap {
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<Key const, T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using iterator = typename std::list<value_type>::iterator;
    using const_iterator = typename std::list<value_type>::const_iterator;

    OrderedMap() {}
    /*  OrderedMap(const OrderedMap&) = delete;
      OrderedMap& operator=(const OrderedMap&) = delete;
      OrderedMap(OrderedMap&&) = default;
      OrderedMap& operator=(OrderedMap&&) = default;*/

      // 데이터를 가져오기
    mapped_type& at(Key const& key)
    {
        auto it = this->cache_map.find(key);

        if (it == this->cache_map.end()) // key가 자료구조에 없음
            throw std::runtime_error("Out of bound");
        this->cache_list.splice(this->cache_list.begin(), this->cache_list, it->second);
        return it->second->second;
    }

    mapped_type const& at(Key const& key) const
    {
        auto it = this->cache_map.find(key);

        if (it == this->cache_map.end())
            throw std::runtime_error("Out of bound");
        this->cache_list.splice(this->cache_list.begin(), this->cache_list, it->second);
        return it->second->second;
    }

    void insert(value_type const& value) {
        auto it = this->cache_map.find(value.first);

        if (it != this->cache_map.end())
            return;
        this->cache_list.emplace_front(value);
        this->cache_map[value.first] = this->cache_list.begin();
    }

    void pop_back()
    {
        value_type const& it = this->cache_list.back();

        this->cache_map.erase(it.first);
        this->cache_list.pop_back();
    }

    size_type size() const { return (this->cache_list.size()); }
    bool empty() const { return (this->cache_list.empty()); }
    value_type& front() { return (this->cache_list.front()); }
    value_type const& front() const { return (this->cache_list.front()); }
    value_type& back() { return (this->cache_list.back()); }
    value_type const& back() const { return (this->cache_list.back()); }

    iterator begin() { return (this->cache_list.begin()); }
    iterator end() { return (this->cache_list.end()); }
    const_iterator begin() const { return (this->cache_list.begin()); }
    const_iterator end() const { return (this->cache_list.end()); }

    iterator find(Key const& key)
    {
        auto it = this->cache_map.find(key);

        if (it == this->cache_map.end())
            return (this->end());
        this->cache_list.splice(this->cache_list.begin(), this->cache_list, it->second);
        return (it->second);
    }
    const_iterator find(Key const& key) const
    {
        auto it = this->cache_map.find(key);

        if (it == this->cache_map.end())
            return (this->end());
        this->cache_list.splice(this->cache_list.begin(), this->cache_list, it->second);
        return (it->second);
    }

    size_type   count(Key const& key) const
    {
        return (this->cache_map.count(key));
    }
private:
    mutable std::list<value_type> cache_list;
    std::unordered_map<Key, iterator> cache_map;
    size_type capacity;
};
