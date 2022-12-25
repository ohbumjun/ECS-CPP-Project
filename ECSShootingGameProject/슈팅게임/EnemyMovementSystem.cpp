#include "EnemyMovementSystem.h"
#include "ECS.h"
#include "TransformComponent.h"

void EnemyMovementSystem::updateComponents(float delta)
{
	std::list<EntityID>& listEnemyEntities = const_cast<std::list<EntityID>&>(m_ECS->GetEnemyEntities());

	auto iter = listEnemyEntities.begin();
	auto iterEnd = listEnemyEntities.end();

	for (; iter != iterEnd;)
	{
		TransformComponent* Transform = m_ECS->GetEntityComponent<TransformComponent>(*iter);

		Transform->IncPosX(-1);

		// 파괴하기
		if (Transform->GetPosX() < 2)
		{
			m_ECS->DestroyEntity(*iter, EntityType::Enemy);

			// listEnemyEntities 목록에서 제거하기 
			iter = listEnemyEntities.erase(iter);
			continue;
		}
		++iter;
	}

	/*
	for (long long i = 0; i < 9000000000000000; ++i)
	{
		for (long long j = 0; j < 9000000000000000; ++j)
		{
			for (long long k = 0; k < 9000000000000000; ++k)
			{
				for (long long a = 0; a < 9000000000000000; ++a)
				{
					for (long long b = 0; b < 9000000000000000; ++b)
					{
					}
				}
			}
		}
	}
	*/
}
