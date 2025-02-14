#pragma once

#include "InputRegistry.h"
#include "Entity.h"
#include "Player.h"

class Player;
class Entity;

class EntityRegistry
{
public:
	class iterator
	{
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Entity;
        using pointer = Entity*;
        using reference = Entity&;

		iterator(EntityRegistry* registry, std::size_t index);
		iterator();
        reference operator*() const;
		pointer operator->() const;
        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;
    private:
        EntityRegistry* registry;
        std::size_t index;
	};
	class const_iterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = Entity;
		using pointer = Entity const*;
		using reference = Entity const&;

		const_iterator(EntityRegistry const* registry, std::size_t index);
		const_iterator();
		reference operator*() const;
		pointer operator->() const;
		const_iterator& operator++();
		const_iterator operator++(int);
		bool operator==(const const_iterator& other) const;
		bool operator!=(const const_iterator& other) const;
	private:
		EntityRegistry const* registry;
		std::size_t index;
	};

	EntityRegistry(Camera& camera, InputRegistry& input_registry, AssetManager &asset_manager);

	template<typename T>
	std::shared_ptr<T> createEntity()
	{
		std::shared_ptr<T> entity = make_shared<T>(this->asset_manager);

		this->entity_pool.emplace_back(entity);
		return (entity);
	}
	Player& getPlayer();

	iterator	begin();
	iterator	end();
	const_iterator	begin() const;
	const_iterator	end() const;
private:
	AssetManager& asset_manager;
	std::vector<std::shared_ptr<Entity>> entity_pool; // 모든 엔티티들의 벡터
	std::shared_ptr<Player> player;
};

/*
플레이어, 중요, 블럭, 비중요 엔티티를 관리하는 클래스.

1. 플레이어가 청크 경계를 넘어가면, 끝쪽에 있던 엔티티들은 회수/저장됨
2. 몹이 스스로 청크 경계를 넘어가면? 넘어갈 수 없도록 막는 것이 좋겠다. 사건의 지평선처럼 경계에 쌓일 수도 있을거같음.
따라서 몹이 회수/저장되는 조건은 무조건 플레이어에 달렸다.

렌더링이 되는 지역 밖에서도 업데이트가 되게 하는게 좋겠는데?
그럼 그냥 벡터에 넣어놓고 업데이트 하다가, 플레이어가 이동하면, 그 조건을 업데이트하면서 보고, 회수에 들어가게 만드는게 가장 쉽고 좋겠다.

포인터를 쓰면 나갔다 들어오면 대상 지정이 초기화되어버림.
왜냐하면 무조건 보조저장장치로 들어가야하기 때문임.
반대로 id를 쓰면 바로 접근이 되지 않으면 개같아짐.

ai마다 필요한 필드가 다르다보니, 저장하는 데이터도 달라짐 -> 시리얼라이즈가 빡셈
그냥 저장 안 할래. 위치랑 AI종류만 저장할래.

그럼 프리리스트, 할당됐는가? 만 있으면 됨.

일단 거리에 따라서 바로 불러올 수 있어야하므로, 트리관련 구조가 나을지도 모름.
청크마다 얼마나 많은 데이터가 들어갈지 알 수 없으므로, ;;; 개빡센데


블럭 아이디 32 * 32 * 32
엔티티 리스트 n

*/