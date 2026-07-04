#pragma once

#include "Engine/Window/Public/Window.h"
#include "Engine/WorldSystem/Private/Core/WorldCtx.hpp"
#include "Engine/WorldSystem/Private/Query/View.hpp"
#include "Engine/WorldSystem/Private/Query/Filter.hpp"

#include "Engine/ECS/Public/Types.hpp"

#include "Engine/Log/Public/LogApi.hpp"

#include <functional>
#include <string>


namespace Engine::ECS::Core
{
	namespace Init
	{
		template<typename Component>
		bool ApplyDefferedInit(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Window::Window& window)
		{
			InitPhase phase = GetInitPhase<Component>();

			if (phase == InitPhase::deffered)
			{
				// std::cerr << "[Init] Deferred Init for: " << typeid(Component).name() << std::endl;
				// Engine::Log::Write(Engine::Log::Level::Info, "[Init] Deferred Init for: ", typeid(Component).name());
				auto ents = Engine::WorldSystem::Query::ViewWhere(ctx.rw, Engine::WorldSystem::Query::All<Component>{});

				// Engine::Log::Write(Engine::Log::Level::Info, "- Found ", std::string(ents.size()) + " entities.");
				// std::cerr << " " << ents.size() << " " << std::endl;

				for (auto e : ents)
				{
					auto& comp = ctx.ww.Get<Component>(e);
					InitSystem<Component>::Init(comp, e, ctx, window);
				}
			}

			return true;
		}

		template<typename ...Components>
		bool ApplyAllDefferedInit(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Window::Window& window)
		{
			if (!(ApplyDefferedInit<Components>(ctx, window),...))
			{
				Engine::Log::Write(Engine::Log::Level::Fatal, "[Deffered Initialization] failed to deffered init", "");
				return false;
			}

			// std::cerr << "[Deffered Initialization] deffered initialization completed successfully\n";
			return true;
		}
	}
}