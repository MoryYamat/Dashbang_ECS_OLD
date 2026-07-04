#pragma once
#include "Game/VFX/Internal/DTO.hpp"
#include "Game/VFX/Public/GameVFXFwd.hpp"
#include "Engine/VFX/Public/VFXFwd.hpp"

#include <string>
#include <vector>

namespace Game::VFX
{
	struct BuildErrors
	{
		std::vector<std::string> msgs;
		bool ok() const { return msgs.empty(); }
		void error(std::string msg) { msgs.emplace_back(msg); }
	};

	class GameVFXCatalogBuilder
	{
	private:

		std::vector<VFXDTO> defs_;
		BuildErrors errs_;

	public:
		void Add(VFXDTO dto) { defs_.push_back(std::move(dto)); }

		void Build(::Engine::VFX::VFXCatalog& catalog, VFXIds& ids);

		bool Ok() { return errs_.ok(); }
	};
}