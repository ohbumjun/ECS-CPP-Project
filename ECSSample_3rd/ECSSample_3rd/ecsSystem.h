#pragma once

#include "ecsComponent.h"

// 특정 Component 를 가지는 entity 를 추출하여 logic 을 수행한다.
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
	// component type id 들이 배열 => 해당 System 이 어떤 component 를 다루는가에 대한 정보
	std::vector<unsigned int> componentTypes;
	std::vector<int> componentFlags;
};

