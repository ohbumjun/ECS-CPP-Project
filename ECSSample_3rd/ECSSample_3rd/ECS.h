#pragma once

#include "ecsComponent.h"
#include "ecsSystem.h"
#include <vector>
#include <map>

class ECS
{
public :
	ECS();
	~ECS();

	// Entity Methods
	EntityHandle makeEntity(BaseECSComponent* components, const unsigned int* componentIDs, size_t numComponents);
	void removeEntity(EntityHandle handle);

	// Component Methods
	template<typename Component>
	void addComponent(EntityHandle entity, Component* component);

	template<typename Component>
	void removeComponent(EntityHandle entity);
	
	template<typename Component>
	void getComponent(EntityHandle entity);

	// System Methods
	inline void addSystem(BaseECSSystem& system)
	{
		systems.push_back(&system);
	}

	void updateSystem(float delta);

	void updateSystemWithMultipleComponents(int index, float delta, const std::vector<unsigned int>& componentTypes,
		std::vector<BaseECSComponent*>& componentParams, std::vector<std::vector<char>*>& componentArrays);

	bool removeSystem(BaseECSSystem& system);

private :
	std::vector<BaseECSSystem*> systems;

	// component pools ?
	std::map<unsigned, std::vector<char>> mapComponent;

	std::vector<std::pair<unsigned, std::vector<std::pair<unsigned int, unsigned int>>>*> entities;

	inline std::pair <unsigned, std::vector<std::pair<unsigned int, unsigned int>>>* handleToRawType(EntityHandle handle)
	{
		return (std::pair <unsigned, std::vector<std::pair<unsigned int, unsigned int>>>*) handle;
	}

	inline unsigned handleToEntityIndex(EntityHandle handle)
	{
		return handleToRawType(handle)->first;
	}

	inline std::vector<std::pair<unsigned, unsigned>>& handleToEntity(EntityHandle handle)
	{
		return handleToRawType(handle)->second;
	}

	void deleteComponent(unsigned int componentID, unsigned int index);
	bool removeComponentInternal(EntityHandle handle, unsigned int componentID);
	void addComponentInternal(EntityHandle handle, std::vector<std::pair<unsigned int, unsigned int>>& entity, unsigned int componentID,
		BaseECSComponent* component);
	BaseECSComponent* getComponentInternal(std::vector<std::pair<unsigned int, unsigned int>>& entityComponents, 
		std::vector<char>& vecArray,unsigned int componentID);
	int findLeastCommonComponent(const std::vector<unsigned int>& componentTypes);
};

template<typename Component>
inline void ECS::addComponent(EntityHandle entity, Component* component)
{
	addComponentInternal(handleToEntity(entity), Component::ID, component);
}

template<typename Component>
inline void ECS::removeComponent(EntityHandle entity)
{
	return removeComponentInternal(entity, Component::ID);
}

template<typename Component>
inline void ECS::getComponent(EntityHandle entity)
{
	getComponentInternal(handleToEntity(entity), mapComponent[Component::ID], Component::ID);
}
