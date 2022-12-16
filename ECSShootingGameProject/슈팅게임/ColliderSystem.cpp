#include "ColliderSystem.h"
#include "ECS.h"
#include "TransformComponent.h"

void ColliderSystem::updateComponents(float delta)
{
	const std::list<EntityID>& BulletConstEntities = m_ECS->GetBulletEntities();
	const std::list<EntityID>& EnemyConstEntities  = m_ECS->GetEnemyEntities();

	std::vector<std::pair<int, int>> vecEnemyPairs;
	std::vector<std::pair<int, int>> vecBulletPairs;

	{
		auto iterBullet = BulletConstEntities.begin();
		auto iterBulletEnd = BulletConstEntities.end();

		for (; iterBullet != iterBulletEnd; ++iterBullet)
		{
			TransformComponent* BulletTransform = m_ECS->GetEntityComponent<TransformComponent>(*iterBullet);

			int PosX = BulletTransform->GetPosX();
			int PosY = BulletTransform->GetPosY();

			vecBulletPairs.push_back(std::make_pair(PosY, PosX));
		};
	}

	{
		auto iterEnemy = EnemyConstEntities.begin();
		auto iterEnemyEnd = EnemyConstEntities.end();


		for (; iterEnemy != iterEnemyEnd; ++iterEnemy)
		{
			TransformComponent* EnemyTransform = m_ECS->GetEntityComponent<TransformComponent>(*iterEnemy);

			int PosX = EnemyTransform->GetPosX();
			int PosY = EnemyTransform->GetPosY();

			vecEnemyPairs.push_back(std::make_pair(PosY, PosX));
		};
	}

	std::list<EntityID>& BulletEntities = const_cast<std::list<EntityID>&>(m_ECS->GetBulletEntities());
	std::list<EntityID>& EnemyEntities = const_cast<std::list<EntityID>&>(m_ECS->GetEnemyEntities());

	auto iterBullet = BulletConstEntities.begin();
	auto iterBulletEnd = BulletConstEntities.end();

	std::sort(vecEnemyPairs.begin(), vecEnemyPairs.end());
	std::sort(vecBulletPairs.begin(), vecBulletPairs.end());

	for (; iterBullet != iterBulletEnd;)
	{
		bool BulletErase = false;
		TransformComponent* BulletTransform = m_ECS->GetEntityComponent<TransformComponent>(*iterBullet);

		auto iterEnemy = EnemyConstEntities.begin();
		auto iterEnemyEnd = EnemyConstEntities.end();

		for (; iterEnemy != iterEnemyEnd; ++iterEnemy)
		{
			TransformComponent* EnemyTransform = m_ECS->GetEntityComponent<TransformComponent>(*iterEnemy);

			int BulletPosY = BulletTransform->GetPosY();
			int BulletPosX = BulletTransform->GetPosX();

			int EnemyPosY  = EnemyTransform->GetPosY();
			int EnemyPosX  = EnemyTransform->GetPosX();

			if (BulletPosY == EnemyPosY)
			{
				if (BulletPosX + 1 >= EnemyPosX - 2 &&
					BulletPosX <= EnemyPosX + 2)
				{
					// Bullet 제거하기 
    				BulletErase = true;

					m_ECS->DestroyEntity(*iterBullet, EntityType::Bullet);
					m_ECS->DestroyEntity(*iterEnemy,  EntityType::Enemy);

					iterBullet = BulletEntities.erase(iterBullet);
					iterEnemy  = EnemyEntities.erase(iterEnemy);

					break;
				}
				else
				{
  					bool H = true;
				}
			}
		}

		if (BulletErase)
			continue;

		++iterBullet;
	}
}
