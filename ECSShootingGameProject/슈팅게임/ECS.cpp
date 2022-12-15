#include "ECS.h"
#include "TransformComponent.h"
#include "InputSystem.h"
#include "EnemyMovementSystem.h"
#include "BulletMovementSystem.h"
#include "EnemyDrawSystem.h"
#include "PlayerBulletDrawSystem.h"
#include "ColliderSystem.h"

bool ECS::Init()
{
	// BG �۾�
	m_BG.resize(25);

	for (int r = 0; r < 25; ++r)
		m_BG[r].resize(80);

	// 1. System �۾�
	/*
	const int InputSystemIdx = 0;
	const int EnemyMovementSystemIdx = 1;
	const int BulletMovementSystemIdx = 2;
	const int EnemyDrawSystemIdx = 3;
	const int PlayerBulletDrawSystemIdx = 4;
	*/
	InputSystem* InputSys					= AddSystem<InputSystem>();
	EnemyMovementSystem* EnemyMovementSys	= AddSystem<EnemyMovementSystem>();
	BulletMovementSystem* BulletMovementSys = AddSystem<BulletMovementSystem>();
	EnemyDrawSystem* EnemyDrawSys			= AddSystem<EnemyDrawSystem>();
	PlayerBulletDrawSystem* PlayerBulletSys = AddSystem<PlayerBulletDrawSystem>();
	ColliderSystem* ColliderSys				= AddSystem<ColliderSystem>();

	// 2. Entity �۾�
	// Player Entity �߰�
	EntityID PlayerEntity = CreateEntity();
	AddComponent<TransformComponent, int>(PlayerEntity, 0, 12); // x,y
	InputSys->SetPlayerHandle(PlayerEntity);

	// Enemy Entity 1�� �߰��ϱ�
	EntityID EnemyEntity = CreateEntity();
	AddComponent<TransformComponent, int>(EnemyEntity, 77, 12); // x,y
	listEnemyEntities.push_back(EnemyEntity);

	return true;
}

void ECS::Draw()
{
	for (int i = 0; i < 25; ++i)
	{
		m_BG[i][79] = '\0';

		for (int j = 0; j < 79; ++j)
		{
			std::cout << m_BG[i][j];
		}
		// std::cout << m_BG[i] << std::endl;
		// printf("%s", bg[i]);
	}
}

void ECS::Update(float Delta)
{
	/*
	const int InputSystemIdx = 0;
	const int EnemyMovementSystemIdx = 1;
	const int BulletMovementSystemIdx = 2;
	const int EnemyDrawSystemIdx = 3;
	const int PlayerBulletDrawSystemIdx = 4;
	*/

	// Key Control
	vecSystems[InputSystemIdx]->updateComponents(Delta);

	// ���� �ð� ���� Enemy Entity �� ������.

	// Movement Update
	// 1. Enemy
	// 2. Bullet => ��Ƽ ������� �ϱ� 
	m_SyncNum = 0;
	vecSystems[EnemyMovementSystemIdx]->updateComponents(Delta);
	vecSystems[BulletMovementSystemIdx]->updateComponents(Delta);
	
	// ����� ���ڸ� ����
	
	// Render
	// 1. Enemy
	// 2. Player,Bullet
	m_SyncNum = 0;
	vecSystems[EnemyDrawSystemIdx]->updateComponents(Delta);
	vecSystems[PlayerBulletDrawSystemIdx]->updateComponents(Delta);

	// Collider
	vecSystems[ColliderSystemIdx]->updateComponents(Delta);
}

void ECS::Refresh()
{
	system("cls");

	for (int y = 0; y < 25; y++)
	{
		for (int x = 0; x < 80; x++)
		{
			m_BG[y][x] = ' ';
		}
	}
}

EntityID ECS::CreateEntity()
{
	Entity newEntity(CreateEntityId(EntityIndex(vecEntities.size()), 0), ComponentMask());
	vecEntities.push_back(std::make_unique<Entity>(newEntity));
	return vecEntities.back()->id;
}

EntityID ECS::CreateBulletEntity()
{
	if (!listBulletFreeEntities.empty())
	{
		EntityIndex newIdx = listBulletFreeEntities.back();
		listBulletFreeEntities.pop_back();
		EntityID newID = CreateEntityId(newIdx, GetEntityVersion(vecEntities[newIdx]->id));
		vecEntities[newIdx]->id = newID;
		listBulletEntities.push_back(newID);
		return vecEntities[newIdx]->id;
	}

	EntityID newID = CreateEntityId(EntityIndex(vecEntities.size()), 0);
	Entity newEntity(newID, ComponentMask());
	AddComponent<TransformComponent>(newID);
	vecEntities.push_back(std::make_unique<Entity>(newEntity));
	return vecEntities.back()->id;
}

EntityID ECS::CreateEnemyEntity()
{
	if (!listEnemyFreeEntities.empty())
	{
		EntityIndex newIdx = listEnemyFreeEntities.back();
		listEnemyFreeEntities.pop_back();
		EntityID newID = CreateEntityId(newIdx, GetEntityVersion(vecEntities[newIdx]->id));
		vecEntities[newIdx]->id = newID;
		listEnemyEntities.push_back(newID);
		return vecEntities[newIdx]->id;
	}

	EntityID newID = CreateEntityId(EntityIndex(vecEntities.size()), 0);
	Entity newEntity(newID, ComponentMask());
	AddComponent<TransformComponent>(newID);
	vecEntities.push_back(std::make_unique<Entity>(newEntity));
	return vecEntities.back()->id;
}

void ECS::DestroyEntity(EntityID id, EntityType Type)
{
	// �ش� slot �� invalid �ϰ� ����� + version ������Ű��
	EntityID newID = CreateEntityId(EntityIndex(-1), GetEntityVersion(id) + 1);
	EntityIndex Idx = GetEntityIndex(id);

	vecEntities[Idx]->id = newID;
	vecEntities[Idx]->mask.reset();

	if (Type == EntityType::Bullet)
		listBulletFreeEntities.push_back(Idx);
	else
		listEnemyFreeEntities.push_back(Idx);
}
