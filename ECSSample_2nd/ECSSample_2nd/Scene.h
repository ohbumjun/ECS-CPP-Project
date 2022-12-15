#pragma once

#include "ECS.h"
#include "Entity.h"
#include "ComponentPool.h"

class EntityDesc
{
	
};

class Scene
{
public :
	EntityID CreateEntity()
	{
		if (!freeEntities.empty())
		{
			EntityIndex newIdx = freeEntities.back();
			freeEntities.pop_back();
			EntityID newID = CreateEntityId(newIdx, GetEntityVersion(entities[newIdx].id));
			entities[newIdx].id = newID;
			return entities[newIdx].id;

		}

		Entity newEntity(CreateEntityId(EntityIndex(entities.size()), 0), ComponentMask());
		entities.push_back(newEntity);
		return entities.back().id;
	}

	// 그런데 해당 위치에 새로운 Entity가 할당되면, ComponentPool 에서 component 정보를 reset 해줘야 하는거 아닌가 ?


	template<typename T>
	void RemoveComponent(EntityID id)
	{
		if (entities[GetEntityIndex(id)].id != id)
			return;

		int componentId = getComponentTypeID<T>();

		entities[GetEntityIndex(id)].mask.reset(componentId);

		// componentPool 에서는 안지워주는 건가?
	}

	template<typename T, typename ...TArgs>
	inline T* AddComponent(EntityID id, TArgs&& ...args)
	{
		int componentId = getComponentTypeID<T>();

		EntityIndex idx = GetEntityIndex(id);

		// Not Enough Component Pool
		if (componentPools.size() <= componentId)
		{
			componentPools.resize(componentId + 1, nullptr);
		}

		// new component, make a new pool
		if (componentPools[componentId] == nullptr)
		{
			componentPools[componentId] = new ComponentPool(sizeof(T));
		}

		T* comp = new (componentPools[componentId]->get(idx)) T(std::forward<TArgs>(args)...);

		 // set bit info
		entities[idx].addComponent<T>(comp);

		return entities[idx].getComponent<T>();
	}

	template<typename T>
	T* GetEntityComponent(EntityID id)
	{
		int componentId = getComponentTypeID<T>();

		EntityIndex idx = GetEntityIndex(id);

		if (!entities[idx].mask.test(componentId))
			return nullptr;

		T* pComponent = static_cast<T*>(componentPools[componentId]->get(idx));

		return pComponent;
	}

	void DestroyEntity(EntityID id)
	{
		// 해당 slot 을 invalid 하게 만들기 + version 증가시키기
		EntityID newID = CreateEntityId(EntityIndex(-1), GetEntityVersion(id) + 1);
		entities[GetEntityIndex(id)].id = newID;
		entities[GetEntityIndex(id)].mask.reset();

		freeEntities.push_back(GetEntityIndex(id));
	}

	const std::vector<Entity>& GetEntities() const
	{
		return entities;
	}

private :
	std::vector<Entity> entities;
	std::vector<ComponentPool> componentPools;
	std::vector<EntityIndex> freeEntities;
};