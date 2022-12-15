#include "ecsComponent.h"

std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, std::size_t>>* BaseECSComponent::componentTypes;

static unsigned int componentID = 0;

unsigned int BaseECSComponent::registerComponentType(ECSComponentCreateFunction createfn, ECSComponentFreeFunction freefn,
	std::size_t size)
{
	if (!componentTypes)
		componentTypes = new std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, std::size_t>>();

	unsigned int componentID = componentTypes->size();

	std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, std::size_t> NewComponentType = {createfn, freefn, size };

	componentTypes->push_back(NewComponentType);

	return componentID;
}