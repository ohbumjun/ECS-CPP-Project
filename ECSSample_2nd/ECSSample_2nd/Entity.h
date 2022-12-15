#pragma once

#include "Component.h"
#include "ECS.h"

class Entity
{
	friend class Scene;
private:
	EntityID id;
	ComponentMask mask;

public  :
	Entity(EntityID pId, ComponentMask pMask)
	{
		id = pId;
		mask = pMask;
	};


	template<typename T>
	inline T* addComponent(T* comp)
	{
		std::unique_ptr<Component> uptr{ comp };

		if (comp->init())
		{
			mask[getComponentTypeID<T>()] = true;
			comp->entity = this;
			return comp;
		}

		return static_cast<T*>(nullptr);
	}

	template<typename T>
	inline T* getComponent() const
	{
		int componentID = getComponentTypeID<T>();
		auto ptr(compList[componentID]);
		return static_cast<T*>(ptr);
	}

	template<typename T>
	inline bool hasComponent() const
	{
		// std::bitset -> bool operator[](size_t pos) const;
		return mask[getComponentTypeID<T>()];
	}
};