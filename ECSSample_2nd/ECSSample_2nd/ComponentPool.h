#pragma once

#include "ECS.h"

// managed by scene
class ComponentPool
{
public :
	ComponentPool(size_t elementsize)
	{
		elementSize = elementsize;
		pData = new char[elementSize * MAX_ENTITIES];
	}

	~ComponentPool()
	{
		delete[] pData;
	}

	inline void* get(size_t index)
	{
		// index : Entity id
		// looking up the component at the desired index
		return pData + index * elementSize;
	}

private :
	char* pData{ nullptr };
	size_t elementSize{ 0 };
};