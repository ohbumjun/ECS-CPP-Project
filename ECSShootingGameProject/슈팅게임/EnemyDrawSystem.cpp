#include "EnemyDrawSystem.h"
#include "ECS.h"
#include "TransformComponent.h"

void EnemyDrawSystem::updateComponents(float delta)
{
	// std::vector<std::vector<char>>& BG = m_ECS->GetBG();
	char** BG = m_ECS->GetBG();

	const std::list<EntityID>& EnemyEntities = m_ECS->GetEnemyEntities();

	auto iter = EnemyEntities.begin();
	auto iterEnd = EnemyEntities.end();

	for (; iter != iterEnd; ++iter)
	{
		TransformComponent* Transform = m_ECS->GetEntityComponent<TransformComponent>(*iter);

		int EnemyX = Transform->GetPosX();
		int EnemyY = Transform->GetPosY();

		BG[EnemyY][EnemyX - 2] = '<';
		BG[EnemyY][EnemyX - 1] = '-';
		BG[EnemyY][EnemyX - 0] = 'O';
		BG[EnemyY][EnemyX + 1] = '-';
		BG[EnemyY][EnemyX + 2] = '>';
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
