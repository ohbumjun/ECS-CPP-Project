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

		// �ı��ϱ�
		if (Transform->GetPosX() > 70)
		{
  			m_ECS->DestroyEntity(*iter, EntityType::Bullet);

			// listBulletEntities ��Ͽ��� �����ϱ� 
			iter = listBulletEntities.erase(iter);
			continue;
		}
		++iter;
	}
}
