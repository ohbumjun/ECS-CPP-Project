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
	m_BG = new char*[25];

	for (int r = 0; r < 25; ++r)
		m_BG[r] = new char[79];

	// 1. System 작업
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

	// 2. Entity 작업
	// Player Entity 추가
	EntityID PlayerEntity = CreateEntity();
	AddComponent<TransformComponent, int>(PlayerEntity, 0, 12); // x,y
	InputSys->SetPlayerHandle(PlayerEntity);

	// Enemy Entity 1개 추가하기
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

		// for (int j = 0; j < 79; ++j)
		// {
		// 	std::cout << m_BG[i][j];
		// }

		std::cout << m_BG[i] << std::endl;
		// printf("%s", bg[i]);
	}
}

void ECS::Update(int Tick)
{
	/*
	const int InputSystemIdx = 0;
	const int EnemyMovementSystemIdx = 1;
	const int BulletMovementSystemIdx = 2;
	const int EnemyDrawSystemIdx = 3;
	const int PlayerBulletDrawSystemIdx = 4;
	*/

	float Delta = 0.f;

	// Key Control => 임의로 0.f
	vecSystems[InputSystemIdx]->updateComponents(Delta);

	// 일정 시간 마다 Enemy Entity 를 만들어낸다.
	m_ThresHold += Tick;

	if (m_ThresHold >= 10)
	{
		m_ThresHold = 0.f;
		EntityID EnemyEntityID = CreateEnemyEntity();
		TransformComponent* BulletTransform = AddComponent<TransformComponent, int>(EnemyEntityID, 77, rand() % 24); // x,y
	}

	// Movement Update
	// 1. Enemy
	// 2. Bullet => 멀티 쓰레드로 하기 
	m_SyncNum = 0;
	vecSystems[EnemyMovementSystemIdx]->updateComponents(Delta);
	vecSystems[BulletMovementSystemIdx]->updateComponents(Delta);
	
	// 아토믹 숫자를 두자
	
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
		vecEntities[newIdx]->addComponent<TransformComponent>();
		listBulletEntities.push_back(newID);
		return vecEntities[newIdx]->id;
	}

	EntityID newID = CreateEntityId(EntityIndex(vecEntities.size()), 0);
	Entity newEntity(newID, ComponentMask());
	vecEntities.push_back(std::make_unique<Entity>(newEntity));
	listBulletEntities.push_back(newID);
	AddComponent<TransformComponent>(newID);
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
		vecEntities[newIdx]->addComponent<TransformComponent>();
		listEnemyEntities.push_back(newID);
		return vecEntities[newIdx]->id;
	}

	EntityID newID = CreateEntityId(EntityIndex(vecEntities.size()), 0);
	Entity newEntity(newID, ComponentMask());
	vecEntities.push_back(std::make_unique<Entity>(newEntity));
	listEnemyEntities.push_back(newID);
	// AddComponent<TransformComponent>(newID); => 별도로 추가해줄 것이다.
	return vecEntities.back()->id;
}

void ECS::DestroyEntity(EntityID id, EntityType Type)
{
	// 해당 slot 을 invalid 하게 만들기 + version 증가시키기
	EntityID newID = CreateEntityId(EntityIndex(-1), GetEntityVersion(id) + 1);
	EntityIndex Idx = GetEntityIndex(id);

	vecEntities[Idx]->id = newID;
	vecEntities[Idx]->mask.reset();

	if (Type == EntityType::Bullet)
 		listBulletFreeEntities.push_back(Idx);
	else
		listEnemyFreeEntities.push_back(Idx);
}
