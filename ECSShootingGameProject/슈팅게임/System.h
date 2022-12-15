#pragma once

#include "Flag.h"

class ECS;

class System
{
public :
	// virtual void updateComponents(float delta, BaseECSComponent* components) {};
	virtual void updateComponents(float delta) {};
	virtual bool Init() { return true; }
	void SetECS(ECS* ECS) { m_ECS = ECS; }
protected :
	ComponentMask  componentMask;
	ECS* m_ECS;

	template<typename T>
	inline void addComponent()
	{
		componentMask[getComponentTypeID<T>()] = true;
	}
};

