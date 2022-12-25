#include "PlayerBulletDrawSystem.h"
#include "ECS.h"
#include "TransformComponent.h"

void PlayerBulletDrawSystem::updateComponents(float delta)
{
	// Bullet
	// std::vector<std::vector<char>>& BG = m_ECS->GetBG();
	char** BG = m_ECS->GetBG();

	const std::list<EntityID>& BulletEntities = m_ECS->GetBulletEntities();

	auto iter = BulletEntities.begin();
	auto iterEnd = BulletEntities.end();

	for (; iter != iterEnd; ++iter)
	{
		TransformComponent* Transform = m_ECS->GetEntityComponent<TransformComponent>(*iter);

		int BulletX = Transform->GetPosX();
		int BulletY = Transform->GetPosY();

		BG[BulletY][BulletX] = '-';
		BG[BulletY][BulletX + 1] = '>';
	}

	// Player
	EntityID PlayerID = m_ECS->GetPlayerEntityID();
	TransformComponent* Transform = m_ECS->GetEntityComponent<TransformComponent>(PlayerID);
	BG[Transform->GetPosY()][Transform->GetPosX()] = '>';

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
