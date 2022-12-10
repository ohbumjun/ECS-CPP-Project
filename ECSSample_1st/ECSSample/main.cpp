
#include <iostream>
#include"Transform.h"
#include "Entity.h"
#include "EntityManager.h"

int main()
{
	EntityManager* manager = new EntityManager;
	Entity* entity = new Entity();
	manager->addEntity(entity);

	while (true)
	{
		manager->update();
		manager->draw();
	}

	std::cout << entity->getComponent<Transform>()->position.x << std::endl;
	std::cout << entity->hasComponent<Transform>() << std::endl;

	return 0;
}
