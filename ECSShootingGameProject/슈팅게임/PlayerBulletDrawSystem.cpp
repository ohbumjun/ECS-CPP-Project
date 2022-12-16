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
}
