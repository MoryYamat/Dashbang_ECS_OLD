#include "Game/Character/Animation/Private/InternalAPI.hpp"

#include "Game/Character/Animation/Public/SkillAnimComponent.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

namespace Game::Character::Animation::Skill
{
	using namespace ::Engine::WorldSystem::Query;

	void SkillAnimRequestLifetimeSystem::Update(const float dt)
	{
		auto ents = ViewWhere(ctx.rw, All<SkillAnimRequestComponent>{});

		for (const auto& e : ents)
		{
			auto& req = ctx.ww.Get<SkillAnimRequestComponent>(e);

			if (!req.active) continue;
			req.ttl -= dt;
			//std::cout <<"dt: " << dt << ", ttl: " << req.ttl << "\n";
			if (req.ttl <= 0) req.active = false;
		}
	}

}