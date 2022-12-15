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

		// �ı��ϱ�
		if (Transform->GetPosX() < 1)
		{
			m_ECS->DestroyEntity(*iter, EntityType::Enemy);

			// listEnemyEntities ��Ͽ��� �����ϱ� 
			iter = listEnemyEntities.erase(iter);
			continue;
		}
		++iter;
	}
}
