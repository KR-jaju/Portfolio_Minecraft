#pragma once

#pragma once

#include <iostream>
#include <list>
#include <unordered_map>

/*

TODO: 야심차게 만들었지만 쓸모가 없었다. 언젠가 쓸모를 찾을 때까지 유기

*/

template <typename Key, typename T>
class LRUCache {
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<Key const, T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;

    LRUCache(int capacity) : capacity(capacity) {}
  /*  LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;
    LRUCache(LRUCache&&) = default;
    LRUCache& operator=(LRUCache&&) = default;*/

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
        {
            return;
        }
        if (this->cache_list.size() == capacity)
        {
            value_type const& it = this->cache_list.back();

            this->cache_map.erase(it.first);
            this->cache_list.pop_back();
        }
        this->cache_list.emplace_front(value);
        this->cache_map[value.first] = this->cache_list.begin();
    }

    size_type size() const { return (this->cache_list.size()); }
    mapped_type& front() { return (this->cache_list.front()); }
    mapped_type const& front() const { return (this->cache_list.front()); }
    mapped_type& back() { return (this->cache_list.back()); }
    mapped_type const& back() const { return (this->cache_list.back()); }

    size_type   count(Key const& key) const
    {
        return (this->cache_map.count(key));
    }
private:
    mutable std::list<value_type> cache_list;
    std::unordered_map<Key, typename std::list<value_type>::iterator> cache_map;
    size_type capacity;
};
