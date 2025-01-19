#pragma once

#include "InputRegistry.h"
#include "EntityReigstry.h"

class EntitySystem
{
public:
	EntitySystem(InputRegistry& input_registry, EntityRegistry& entity_registry);
	void	update(float dt);
private:
	InputRegistry& input_registry;
	EntityRegistry& entity_registry;
};

/*
1. 블럭 엔티티, 중요 엔티티는 사라지면 안됨, 파일에 저장이 돼야함.
2. 중요하지 않은 엔티티는 특정 범위를 나가면 사라짐.
3. 엔티티 수의 제한은 없을지도 모름.
4. 엔티티 간의 상호작용은..

Entity System은 몹의 자동 생성을 다루는 것이 좋으려나?
EntityRegistry가 몹의 생성, 삭제, 저장을 다루는 것이 좋을 것 같다.


Physics system
*/