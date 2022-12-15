#pragma once

#include <vector>
#include <tuple>

struct BaseECSComponent;
typedef void* EntityHandle;
typedef unsigned int (*ECSComponentCreateFunction)(std::vector<char>& memory, EntityHandle entity, BaseECSComponent* comp);
typedef unsigned int (*ECSComponentFreeFunction)(BaseECSComponent* comp);

struct BaseECSComponent
{
	static unsigned int registerComponentType(ECSComponentCreateFunction createfn, ECSComponentFreeFunction freefn, 
		size_t size);
	EntityHandle entity = nullptr;

	inline static ECSComponentCreateFunction GetTypeCreateFunction(unsigned int id)
	{
		return std::get<0>((*componentTypes)[id]);
	}

	inline static ECSComponentFreeFunction GetTypeFreeFunction(unsigned int id)
	{
		return std::get<1>((*componentTypes)[id]);
	}

	inline static std::size_t GetTypeSize(unsigned int id)
	{
		return std::get<2>((*componentTypes)[id]);
	}

	inline static bool isTypeValid(unsigned int id)
	{
		return id >= componentTypes->size();
	}

private :
	// 모든 Component 정보를 들고 있는다고 생각하면 된다.
	// idx => component ID ?
	static std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, std::size_t>>* componentTypes;
};

template<typename T>
struct ECSComponent : public BaseECSComponent
{
	static const ECSComponentCreateFunction CREATE_FUNCTION;
	static const ECSComponentFreeFunction FREE_FUNCTION;
	static const unsigned int ID;
	static const size_t SIZE;
};

template<typename Component>
unsigned int ECSComponentCreate(std::vector<char>& memory, EntityHandle entity, BaseECSComponent* comp)
{
	unsigned int index = memory.size();
	memory.resize(index + Component::SIZE);
	Component* component = new (&memory[index]) Component(*(Component*)comp);
	component->entity = entity;
	return index;
}

template<typename Component>
void ECSComponentFree(BaseECSComponent* comp)
{
	Component* component = (Component*)comp;
	component->~Component();
}

template<typename T>
const unsigned int ECSComponent<T>::ID(BaseECSComponent::registerComponentType(ECSComponentCreate<T>,
	ECSComponentFree<T>, sizeof(T)));

template<typename T>
const unsigned int ECSComponent<T>::SIZE(sizeof(T));

template<typename T>
const ECSComponentCreateFunction ECSComponent<T>::CREATE_FUNCTION(ECSComponentCreate<T>);

template<typename T>
const ECSComponentFreeFunction ECSComponent<T>::FREE_FUNCTION(ECSComponentFree<T>);

struct TestComponent : public  ECSComponent<TestComponent>
{
	float x;
	float y;
};