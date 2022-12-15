#pragma once

#include "ecsComponent.h"

// Ư�� Component �� ������ entity �� �����Ͽ� logic �� �����Ѵ�.
class BaseECSSystem
{
public :
	enum
	{
		FLAG_OPTIONAL = 1
	};

	virtual void updateComponents(float delta, BaseECSComponent* components) {};

	const std::vector<unsigned int>& getComponentTypes()
	{
		return componentTypes;
	}
	const std::vector<int>& getComponentFlags()
	{
		return componentFlags;
	}
protected: 
	void addComponentType(unsigned int componentType, int componentFlag)
	{
		componentTypes.push_back(componentType);
		componentFlags.push_back(componentFlag);
	}
private :
	// component type id ���� �迭 => �ش� System �� � component �� �ٷ�°��� ���� ����
	std::vector<unsigned int> componentTypes;
	std::vector<int> componentFlags;
};

