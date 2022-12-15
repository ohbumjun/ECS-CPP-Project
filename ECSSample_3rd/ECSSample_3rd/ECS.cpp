#include "ECS.h"

ECS::ECS()
{
}

ECS::~ECS()
{
    for (auto it = mapComponent.begin(); it != mapComponent.end(); ++it)
    {
        size_t typeSize = BaseECSComponent::GetTypeSize(it->first);
        ECSComponentFreeFunction freefn = BaseECSComponent::GetTypeFreeFunction(it->first);

        for (unsigned int i = 0; i < it->second.size(); i += typeSize)
        {
            freefn((BaseECSComponent*)&it->second[i]);
        }
    }

    for (unsigned int i = 0; i < entities.size(); ++i)
        delete entities[i];
}

EntityHandle ECS::makeEntity(BaseECSComponent* components, const unsigned int* componentIDs, size_t numComponents)
{
    std::pair<unsigned int, std::vector<std::pair<unsigned int, unsigned int>>>* newEntity = new std::pair<unsigned int, std::vector<std::pair<unsigned int, unsigned int>>>();
    
    EntityHandle handle = (EntityHandle)newEntity;

    for (unsigned int i = 0; i < numComponents; ++i)
    {
        // Check if componentID is actually valid
        if (BaseECSComponent::isTypeValid(componentIDs[i]) == false)
        {
            // delete new entity 
            delete newEntity;
            return nullptr;
        }

        addComponentInternal(handle, newEntity->second, componentIDs[i], &components[i]);
    }
    
    return EntityHandle();
}

// ex. ���� �߿� Bullet �� ������� �ϱ� 
void ECS::removeEntity(EntityHandle handle)
{
    std::vector<std::pair<unsigned int, unsigned int>>& entity = handleToEntity(handle);

    for (unsigned int i = 0; i < entity.size(); ++i)
    {
        deleteComponent(entities[i]->first, entity[i].second);

    }

    unsigned int destIdx = handleToEntityIndex(handle);
    unsigned int srcIdx   = entities.size() - 1;

    delete entities[destIdx];

    entities[destIdx] = entities[srcIdx];
    entities[destIdx]->first = destIdx;
    entities.pop_back();
}

void ECS::updateSystem(float delta)
{
    std::vector<BaseECSComponent*> componentParams;
    std::vector<std::vector<char>*> componentArrays;

    for (size_t i = 0; i < systems.size(); ++i)
    {
        // component ID ���� �迭
        const std::vector<unsigned int>& componentTypes = systems[i]->getComponentTypes();

        if (componentTypes.size() == 1)
        {
            // componentTypes[0] : component ID
            size_t typeSize = BaseECSComponent::GetTypeSize(componentTypes[0]);

            // component Pool
            std::vector<char>& vecComponents = mapComponent[componentTypes[0]];

            for (size_t i = 0; i < vecComponents.size(); i += typeSize)
            {
                BaseECSComponent* componentAddress = (BaseECSComponent*)&vecComponents[i];
                systems[i]->updateComponents(delta, componentAddress);
            }
        }
        else
        {
            updateSystemWithMultipleComponents(i, delta, componentTypes, componentParams, componentArrays);
        }
    }
}

void ECS::updateSystemWithMultipleComponents(int index, float delta, const std::vector<unsigned int>& componentTypes,
    std::vector<BaseECSComponent*>& componentParams, std::vector<std::vector<char>*>& componentArrays)
{
    const std::vector<int>& componentFlags = systems[index]->getComponentFlags();

    componentParams.resize(componentTypes.size());
    componentArrays.resize(componentTypes.size());

    // componentArrays : component Pool ���� �ּҰ����� �迭 ���·� ��Ƶ� ��
    for (size_t i = 0; i < componentTypes.size(); ++i)
        componentArrays[i] = &mapComponent[componentTypes[i]];

    // component �� �� �޸� �� ���� ���� �ִ� component �� id �� idx
    int minSizeIdx = findLeastCommonComponent(componentTypes);

    size_t typeSize = BaseECSComponent::GetTypeSize(componentTypes[minSizeIdx]);
    std::vector<char>& vecComponents = mapComponent[componentTypes[minSizeIdx]];

    // minSizeIdx �� �ش��ϴ� Component �� Component Pool �� ��ȸ�Ѵ�.
    for (size_t i = 0; i < vecComponents.size(); i += typeSize)
    {
        componentParams[minSizeIdx] = (BaseECSComponent*)&componentArrays[minSizeIdx][i];
        
        // �ش� component �� ����ϴ� entity -> �ش� entity�� ����ϴ� component ��� ���Ǳ� 
        std::vector<std::pair<unsigned int, unsigned int>>& entityComponents = handleToEntity(componentParams[minSizeIdx]->entity);

        bool IsValid = true;

        for (size_t s = 0; s < componentTypes.size(); ++s)
        {
            if (s == minSizeIdx)
                continue;

            // componentParams[s] : ���� �޸� �� �����ϴ� Component �� �޸� ����
            componentParams[s] = getComponentInternal(entityComponents, *componentArrays[s], componentTypes[s]);

            // ���� �޸𸮿��� ������ ���¶��
            if (componentParams[s] == nullptr && (componentFlags[s] & BaseECSSystem::FLAG_OPTIONAL == 0))
            {
                IsValid = false;
                break;
            }
        }

        // ??
        if (IsValid)
        {
            systems[i]->updateComponents(delta, componentParams[0]);
        }

    }
}

bool ECS::removeSystem(BaseECSSystem& system)
{
    for (size_t i = 0; i < systems.size(); ++i)
    {
        if (&system == systems[i])
        {
            systems.erase(systems.begin() + i);
            return true;
        }
    }

    return false;
}

void ECS::deleteComponent(unsigned int componentID, unsigned int index)
{
    std::vector<char>& memoryArray = mapComponent[componentID];
    ECSComponentFreeFunction freefn = BaseECSComponent::GetTypeFreeFunction(componentID);
    size_t typeSize = BaseECSComponent::GetTypeSize(componentID);
    unsigned int srcIndex = memoryArray.size() - typeSize;

    BaseECSComponent* destComponent = (BaseECSComponent*)&memoryArray[index];
    BaseECSComponent* srcComponent = (BaseECSComponent*)&memoryArray[srcIndex];

    freefn(destComponent);

    if (index == srcIndex)
    {
        memoryArray.resize(srcIndex);
        return;
    }

    memcpy(destComponent, srcComponent, typeSize);
    
    std::vector<std::pair<unsigned int, unsigned int>>& srcComponents = handleToEntity(srcComponent->entity);

    for (unsigned int i = 0; i < srcComponents.size(); ++i)
    {
        if (componentID == srcComponents[i].first && srcIndex == srcComponents[i].second)
        {
            srcComponents[i].second = index;
            break;
        }
    }
}

bool ECS::removeComponentInternal(EntityHandle handle, unsigned int componentID)
{
    // �ش� entity �� ������ �ִ� Component ��ϵ�
    // componentID, index (in memory pool)
    std::vector<std::pair<unsigned int, unsigned int>>& entityComponents = handleToEntity(handle);

    for (unsigned int i = 0; i < entityComponents.size(); ++i)
    {
        //entityComponents[i].first : �ش� Component �� ���� ID
        if (componentID == entityComponents[i].first)
        {
            deleteComponent(componentID, entityComponents[i].second);
            unsigned int srcIndex   = entityComponents.size() - 1;
            unsigned int destIndex = i;
            entityComponents[destIndex] = entityComponents[srcIndex];
            entityComponents.pop_back();
            return true;
        }
    }

    return false;
}

void ECS::addComponentInternal(EntityHandle handle, std::vector<std::pair<unsigned int, unsigned int>>& entity, 
    unsigned int componentID, BaseECSComponent* component)
{
    // BaseECSComponent* component : �߰��ϰ��� �ϴ� component ����

    ECSComponentCreateFunction createfn = BaseECSComponent::GetTypeCreateFunction(componentID);
    std::pair<unsigned int, unsigned int> newPair;
    newPair.first = componentID;

    // newPair.second : componentPool �� �ش� component �� ����� idx ����
    newPair.second = createfn(mapComponent[componentID], handle, component);

    entity.push_back(newPair);
}

BaseECSComponent* ECS::getComponentInternal(std::vector<std::pair<unsigned int, unsigned int>>& entityComponents,
    std::vector<char>& vecArray, unsigned int componentID)
{
    for (unsigned int i = 0; i < entityComponents.size(); ++i)
    {
        if (componentID == entityComponents[i].first)
        {
            // mapComponent[componentID] ���� ��ü�� Tree �� ��ȸ�ϸ鼭 ã�ƴٴϴ� ���̴�. ĳ�� ���߷��� ���̱� ����
            // �Ʒ��� ���� ������ ���̴�. 
            // return (BaseECSComponent*)&mapComponent[componentID][entityComponents[i].second];

            // entityComponents[i].second : �ش� Component �� Component Pool �󿡼� ����� idx ����
            return (BaseECSComponent*)&vecArray[entityComponents[i].second];
        }
    }

    return nullptr;
}

// �޸� �� ���� ���� �ִ� Component �� ã�� 
int ECS::findLeastCommonComponent(const std::vector<unsigned int>& componentTypes)
{
    int minSize = mapComponent[0].size() / BaseECSComponent::GetTypeSize(componentTypes[0]);
    int minIdx = 0;

    for (size_t i = 1; i < componentTypes.size(); ++i)
    {
        size_t typeSize = BaseECSComponent::GetTypeSize(componentTypes[i]);
        int size = mapComponent[componentTypes[i]].size() / typeSize;

        if (size < minSize)
        {
            minSize = size;
            minIdx = i;
        }
    }

    return 0;
}
