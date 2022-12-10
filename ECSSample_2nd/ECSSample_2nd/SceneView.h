#pragma once

#include "ECS.h"
#include "Scene.h"

// 특정 Component 들을 가진 entity 들을 iterate 해주는 class
template<typename ...ComponentTypes>
class SceneView
{
public:
	SceneView(Scene& Scene) : pScene(&Scene)  
	{
		if (sizeof...(ComponentTypes) == 0)
		{
			// iterate all entities ==> SceneView<>(scene)
			all = true;
		}
		else
		{
			int componentIds[] = { 0, getComponentTypeID<ComponentTypes>()... };

			for (int i = 1; i < sizeof...(ComponentTypes) + 1; ++i)
			{
				componentMask.set(componentIds[i]);
			}
		}
	}

private :

	struct Iterator
	{
		Iterator(Scene* Scene, EntityIndex idx, ComponentMask mask, bool all) :
			pScene(Scene), index(idx), mask(mask), all(all) {};

		EntityID operator * () const
		{
			// give back entityID we're currently at
			return pScene->GetEntities()[index].id;
		}

		bool operator == (const Iterator& other) const
		{
			return index == other.index || index == pScene->entities.size();
		}

		bool operator != (const Iterator& other) const
		{
			return index != other.index && index != pScene->entities.size();
		}

		Iterator& operator ++()
		{
			// Move iterator forwards
			// Cautious : Interator must skip over entities in the free list
			do
			{
				index++;
			} while (index < pScene->entities.size() && !ValidIndex());

			return *this;
		}

		bool ValidIndex()
		{
			// valid entity id (not in freelist) + correct component mask
			return IsEntityValid(pScene->entities[index].id) && (all || mask == (pScene->entities[index].mask));
		}

		EntityIndex index;
		Scene* pScene;
		ComponentMask mask;
		bool all{ false };
	};

	const Iterator begin() const
	{
		int firstIndex = 0;

		while (firstIndex < pScene->entities.size() &&
			(componentMask != (componentMask & pScene->entities[firstIndex].mask) ||
				!IsEntityValid(pScene->entities[firstIndex].id))
		{
			firstIndex++;
		}

		return Iterator(pScene, firstIndex, componentMask, all);
	}

	const Iterator end() const
	{
		return Iterator(pScene, EntityIndex(pScene->entities.size()), componentMask, all);
	}

	

	Scene* pScene = nullptr;
	ComponentMask  componentMask;
	bool all{ false };
};
