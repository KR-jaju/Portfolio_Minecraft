#pragma once

#include <mutex>

template <typename Key, unsigned int n>
class StripedMutex
{
public:
	std::mutex& operator[](Key const& key);
private:
	std::array<std::mutex, n> mutex_array;
};

template <typename Key, unsigned int n>
std::mutex& StripedMutex<Key, n>::operator[](Key const& key)
{
	return (this->mutex_array[std::hash<Key>{}(key) % n]);
}