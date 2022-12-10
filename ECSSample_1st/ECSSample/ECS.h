#pragma once

#include <iostream>
#include <array>
#include <bitset>
#include <iostream>

class Entity;
class Component;

using ComponentTypeID = std::size_t;

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

	static const ComponentTypeID typeID = getUniqueComponentID();

	return typeID;
}

constexpr std::size_t MAX_ENTITIES = 5000;
constexpr std::size_t MAX_COMPONENTS = 32;

using ComponentBitset = std::bitset<MAX_COMPONENTS>;
using ComponentList = std::array<Component*, MAX_COMPONENTS>;