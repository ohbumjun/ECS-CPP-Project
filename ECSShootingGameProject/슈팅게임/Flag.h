#pragma once

#pragma once

#include <iostream>
#include <bitset>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <conio.h>

class Component;

constexpr std::size_t MAX_ENTITIES = 5000;
constexpr std::size_t MAX_COMPONENTS = 32;

using ComponentMask = std::bitset<MAX_COMPONENTS>;
using ComponentTypeID = std::size_t;
using EntityID = unsigned long long;
using EntityIndex = unsigned int;
using EntityVersion = unsigned int;

enum class EntityType
{
	Bullet,
	Enemy,
	Max
};

template<typename T>
inline ComponentTypeID getUniqueComponentID()
{
	static ComponentTypeID lastID = 0;
	return lastID++;
}

// �� Type �� ���� getComponentTypeID() �Լ��� �Ѱ��� ���������.
// �Լ� ���� static ������ ��� �Լ� �ϳ��� ���� �ѹ����� �޸𸮿� �Ҵ�ȴ�. (�ش� �Լ� ó�� ȣ���� ���� �Ҵ�ȴ� !!)
// ���� ���� Type �� ���� getComponentTypeID() �� ȣ���ϸ� �̹� static const �� ���� ������ �״�� �����ϴ� ���̴�.
// ������ �޸𸮿� �Ҵ�� �༮�̰�, ��� �Լ����� �����ϴ� ���̱� �����̴�.
template<typename T>
inline ComponentTypeID getComponentTypeID() noexcept
{
	static_assert(std::is_base_of<Component, T>::value, "Type not base on component!");

	static const ComponentTypeID typeID = getUniqueComponentID<T>();

	return typeID;
}

// Entity ID
inline EntityID CreateEntityId(EntityIndex index, EntityVersion version)
{
	return ((EntityID)index << 32) | ((EntityID)version);
}

inline EntityIndex GetEntityIndex(EntityID id)
{
	return id >> 32;
}

inline EntityVersion GetEntityVersion(EntityID id)
{
	return (EntityVersion)id;
}

inline bool IsEntityValid(EntityID id)
{
	return (id >> 32) != (EntityIndex)(-1);
}

