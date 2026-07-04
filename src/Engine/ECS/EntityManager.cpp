#include "EntityManager.h"


#include "Engine/ECS/Component/Graphics/MeshComponent.h"



template<typename T>
void destroyIfExists(std::type_index currentType, std::shared_ptr<void>& compPtr)
{
	if (currentType == std::type_index(typeid(T)))
	{
		auto typed = std::static_pointer_cast<T>(compPtr);
		typed->Destroy();
	}
}

// Clear all components at once
void Engine::ECS::EntityMgr::Clear()
{
	for (auto& [type, entityMap] : mComponentPools)
	{
		auto ti = mTypeInfo.find(type);
		for (auto& [eid, sp] : entityMap)
		{
			if (sp && ti != mTypeInfo.end())
				ti->second.onDestroy(sp.get());
		}
	}

	mComponentPools.clear();
	mLivingEntities.clear();
	mResources.clear();
	mTypeInfo.clear();
}