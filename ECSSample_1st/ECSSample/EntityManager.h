#pragma once

#include <vector>
#include <memory>

class Entity;

class EntityManager
{
public :
	EntityManager() = default;
	~EntityManager() = default;

	void draw();
	void update();
	void refresh();

	void addEntity(Entity* player);
	void eraseEntity(Entity* player);
	
	Entity* cloneEntity(Entity* player);

protected :

private :
	std::vector<std::unique_ptr<Entity>> entities;
};