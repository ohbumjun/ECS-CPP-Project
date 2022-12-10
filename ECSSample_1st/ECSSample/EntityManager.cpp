#include "EntityManager.h"
#include "Entity.h"

void EntityManager::draw()
{
	for (auto& entity : entities)
		entity->draw();
}

void EntityManager::update()
{
	for (auto& entity : entities)
		entity->update();
}

void EntityManager::refresh()
{
}

void EntityManager::addEntity(Entity* player)
{
	std::unique_ptr<Entity> uniquePtr{ player };
	entities.emplace_back(std::move(uniquePtr));
}

void EntityManager::eraseEntity(Entity* player)
{
}

Entity* EntityManager::cloneEntity(Entity* player)
{
	return nullptr;
}
