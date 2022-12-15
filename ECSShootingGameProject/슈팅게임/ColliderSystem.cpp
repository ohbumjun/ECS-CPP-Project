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

	for (; iterBullet != iterBulletEnd;)
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
					// Bullet �����ϱ� 
    					BulletErase = true;

					m_ECS->DestroyEntity(*iterBullet, EntityType::Bullet);
					m_ECS->DestroyEntity(*iterEnemy, EntityType::Enemy);

					iterBullet = BulletEntities.erase(iterBullet);
					iterEnemy  = EnemyEntities.erase(iterEnemy);
					break;
				}
			}
		}

		if (BulletErase)
			continue;

		++iterBullet;
	}
}
