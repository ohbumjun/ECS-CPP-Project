#include "ColliderSystem.h"
#include "ECS.h"
#include "TransformComponent.h"

void ColliderSystem::updateComponents(float delta)
{
	std::list<EntityID>& BulletEntities = const_cast<std::list<EntityID>&>(m_ECS->GetBulletEntities());
	std::list<EntityID>& EnemyEntities  = const_cast<std::list<EntityID>&>(m_ECS->GetEnemyEntities());

	auto iterBullet    = BulletEntities.begin();
	auto iterBulletEnd = BulletEntities.end();

	auto iterEnemy    = EnemyEntities.begin();
	auto iterEnemyEnd = EnemyEntities.end();

	for (; iterBullet != iterEnemyEnd;)
	{
		bool BulletErase = false;
		TransformComponent* BulletTransform = m_ECS->GetEntityComponent<TransformComponent>(*iterBullet);

		for (; iterEnemy != iterEnemyEnd; ++iterEnemy)
		{
			TransformComponent* EnemyTransform = m_ECS->GetEntityComponent<TransformComponent>(*iterEnemy);

			if (BulletTransform->GetPosY() == EnemyTransform->GetPosY())
			{
				if (BulletTransform->GetPosX() >= EnemyTransform->GetPosX() - 1 &&
					BulletTransform->GetPosX() <= EnemyTransform->GetPosX() + 1)
				{
					// Bullet 제거하기 
					BulletErase = true;
					iterBullet = BulletEntities.erase(iterBullet);
					break;
				}
			}

			if (BulletErase)
				continue;

			++iterBullet;
		}
	}
}
