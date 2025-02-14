#include "pch.h"
#include "EntityReigstry.h"
#include "Entity.h"
#include "Player.h"

EntityRegistry::EntityRegistry(Camera& camera, InputRegistry& input_registry, AssetManager& asset_manager)
	: asset_manager(asset_manager),
    player(make_shared<Player>(camera, input_registry, asset_manager))
{
	this->entity_pool.emplace_back(this->player);
}

Player& EntityRegistry::getPlayer()
{
	return (*this->player);
}

EntityRegistry::iterator    EntityRegistry::begin()
{
    return iterator(this, 0);
}

EntityRegistry::iterator    EntityRegistry::end()
{
    return iterator(this, this->entity_pool.size());
}

EntityRegistry::const_iterator    EntityRegistry::begin() const
{
    return const_iterator(this, 0);
}

EntityRegistry::const_iterator    EntityRegistry::end() const
{
    return const_iterator(this, this->entity_pool.size());
}

/*
1. 청크의 중심점과 플레이어 사이의 거리가 128블럭 이내인 청크를 랜덤 틱이 동작하는 청크라고 함. (2차원 거리임!)
2. 이 청크들에서 매 스폰 사이클마다 팩 스폰 시도를 함 (최대 3번의 팩 스폰 시도).
3. 청크 안에서 랜덤하게 위치를 선택하고, opaque블럭이 아니라면 스폰 시도를 함. opaque라면 바로 break함.
4. 선택된 위치에서 +-5블럭(x, z에 대해)에 팩 크기만큼 몹 소환을 시도함. y좌표는 땅바닥에 맞도록 조정함.

디스폰
1. 거리가 32에서 128 사이인 몹은 특정 확률로 디스폰을 시도함. 128 이상은 강제 디스폰
2. 몹을 저장하게 된다면, 현재 몹이 존재하는 청크에 연동해서 저장함.


*/

EntityRegistry::iterator::iterator(EntityRegistry* registry, std::size_t index)
    : registry(registry), index(index) {}

EntityRegistry::iterator::iterator()
    : registry(nullptr), index(0) {}

EntityRegistry::iterator::reference EntityRegistry::iterator::operator*() const
{
	return (*this->registry->entity_pool[this->index]);
}

EntityRegistry::iterator::pointer EntityRegistry::iterator::operator->() const
{
    return registry->entity_pool[this->index].get();
}

EntityRegistry::iterator& EntityRegistry::iterator::operator++()
{
    ++this->index;
    return *this;
}

EntityRegistry::iterator EntityRegistry::iterator::operator++(int)
{
    iterator temp = *this;
    ++(*this);
    return temp;
}
bool EntityRegistry::iterator::operator==(const iterator& other) const
{
    return (registry == other.registry) && (this->index == other.index);
}
bool EntityRegistry::iterator::operator!=(const iterator& other) const
{
    return !(*this == other);
}

/*-----------------------------------------CONST ITERATOR--------------------------------------------*/

EntityRegistry::const_iterator::const_iterator(EntityRegistry const* registry, std::size_t index)
    : registry(registry), index(index) {}

EntityRegistry::const_iterator::const_iterator()
    : registry(nullptr), index(0) {}

EntityRegistry::const_iterator::reference EntityRegistry::const_iterator::operator*() const
{
    return (*this->registry->entity_pool[this->index]);
}

EntityRegistry::const_iterator::pointer EntityRegistry::const_iterator::operator->() const
{
    return registry->entity_pool[this->index].get();
}

EntityRegistry::const_iterator& EntityRegistry::const_iterator::operator++()
{
    ++this->index;
    return *this;
}

EntityRegistry::const_iterator EntityRegistry::const_iterator::operator++(int)
{
    const_iterator temp = *this;
    ++(*this);
    return temp;
}
bool EntityRegistry::const_iterator::operator==(const const_iterator& other) const
{
    return (registry == other.registry) && (this->index == other.index);
}
bool EntityRegistry::const_iterator::operator!=(const const_iterator& other) const
{
    return !(*this == other);
}