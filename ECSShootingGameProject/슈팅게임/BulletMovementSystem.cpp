#include "BulletMovementSystem.h"
#include "ECS.h"
#include "TransformComponent.h"

void BulletMovementSystem::updateComponents(float delta)
{
	std::list<EntityID>& listBulletEntities = const_cast<std::list<EntityID>&>(m_ECS->GetBulletEntities());

	auto iter    = listBulletEntities.begin();
	auto iterEnd = listBulletEntities.end();

	for (; iter != iterEnd;)
	{
		TransformComponent* Transform = m_ECS->GetEntityComponent<TransformComponent>(*iter);

		Transform->IncPosX(1);

		// 파괴하기
		if (Transform->GetPosX() > 70)
		{
  			m_ECS->DestroyEntity(*iter, EntityType::Bullet);

			// listBulletEntities 목록에서 제거하기 
			iter = listBulletEntities.erase(iter);
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
