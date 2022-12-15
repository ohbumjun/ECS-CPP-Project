#pragma once

#include "Component.h"
#include "Flag.h"

class Entity
{
	friend class ECS;
private:
	EntityID id;
	ComponentMask mask;

public:
	Entity(EntityID pId, ComponentMask pMask)
	{
		id = pId;
		mask = pMask;
	};

	template<typename T>
	inline void addComponent()
	{
		mask[getComponentTypeID<T>()] = true;
	}
};