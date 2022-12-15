#pragma once

#include <vector>
#include <memory>
#include <assert.h>
#include "Entity.h"
#include "Flag.h"
#include "System.h"
#include "ComponentPool.h"

// �Լ� ���ø� == �ζ��� ? �ƴϴ� ! �Լ� ���ø��� inline Ű���带 �ٿ���� �Ѵ�.
const int InputSystemIdx = 0;
const int EnemyMovementSystemIdx = 1;
const int BulletMovementSystemIdx = 2;
const int EnemyDrawSystemIdx = 3;
const int PlayerBulletDrawSystemIdx = 4;
const int ColliderSystemIdx = 5;

class ECS
{
public:
	ECS() = default;
	~ECS()
	{
		delete [] m_BG;
	}

	bool Init();

	void Draw();
	void Update(int Tick);
	void Refresh();

	/*
	Entity Related
	*/

	// �׷��� �ش� ��ġ�� ���ο� Entity�� �Ҵ�Ǹ�, ComponentPool ���� component ������ reset ����� �ϴ°� �ƴѰ� ?
	EntityID CreateEntity();
	EntityID CreateBulletEntity();
	EntityID CreateEnemyEntity();
	void DestroyEntity(EntityID id, EntityType Type);

	template<typename T>
	inline void RemoveComponent(EntityID id)
	{
		if (vecEntities[GetEntityIndex(id)]->id != id)
			return;

		int componentId = getComponentTypeID<T>();

		vecEntities[GetEntityIndex(id)]->mask.reset(componentId);

		// componentPool ������ �������ִ� �ǰ�?
	}

	template<typename T, typename ...TArgs>
	inline T* AddComponent(EntityID id, TArgs&& ...args)
	{
		int componentId = getComponentTypeID<T>();

		EntityIndex idx = GetEntityIndex(id);

		// Not Enough Component Pool
		if (vecComponentPools.size() <= (size_t)componentId)
		{
			vecComponentPools.resize(componentId + 1, nullptr);
		}

		// new component, make a new pool
		if (vecComponentPools[componentId] == nullptr)
		{
			vecComponentPools[componentId] = new ComponentPool(sizeof(T));
		}

		T* comp = new (vecComponentPools[componentId]->get(idx)) T(std::forward<TArgs>(args)...);

		// set bit info
		vecEntities[idx]->addComponent<T>();

		return comp;
	}

	template<typename T>
	inline T* GetEntityComponent(EntityID id)
	{
		int componentId = getComponentTypeID<T>();

		EntityIndex idx = GetEntityIndex(id);

		if (!vecEntities[idx]->mask.test(componentId))
			return nullptr;

		T* pComponent = static_cast<T*>(vecComponentPools[componentId]->get(idx));

		return pComponent;
	}

	/*
	System Related
	*/
	template<typename T>
	inline T* AddSystem()
	{
		std::unique_ptr<T> newSystem = std::make_unique<T>();

		if (!newSystem->Init())
			assert(false);

		newSystem->SetECS(this);

		vecSystems.push_back(std::move(newSystem));

		return (T*)vecSystems[vecSystems.size() - 1].get();
	}

	/*
	Getter
	*/
	const std::vector<std::unique_ptr<Entity>>& GetEntities() const
	{
		return vecEntities;
	}

	ECS* GetECS() const
	{
		return m_Inst;
	}

	// std::vector<std::vector<char>>& GetBG()
	// {
	// 	return m_BG;
	// }

	char** GetBG()
	{
		return m_BG;
	}

	const std::list<EntityID>& GetBulletEntities() const
	{
		return listBulletEntities;
	}

	const std::list<EntityID>& GetEnemyEntities() const
	{
		return listEnemyEntities;
	}

	EntityID GetPlayerEntityID() const
	{
		return vecEntities[0]->id;
	}

	/*
	Setter
	*/
	void AddBulletEntitiyID(EntityID id)
	{
		listBulletEntities.push_back(id);
	}
	void AddEnemyEntitiyID(EntityID id)
	{
		listEnemyEntities.push_back(id);
	}
private:
	// Player�� �ش��ϴ� Entity �� 0��° idx �� ������ ���̴�.
	std::vector<ComponentPool*> vecComponentPools;
	std::list<EntityIndex> listBulletFreeEntities;
	std::list<EntityIndex> listEnemyFreeEntities;
	std::list<EntityID> listBulletEntities;
	std::list<EntityID> listEnemyEntities;
	std::vector<std::unique_ptr<Entity>> vecEntities;
	std::vector<std::unique_ptr<System>> vecSystems;
	char ** m_BG;
	static ECS* m_Inst;
	std::atomic<int> m_SyncNum;
	float m_ThresHold = 0.f;
};

