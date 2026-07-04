#pragma once
#include "Engine/VFX/Public/VFXFwd.hpp"
#include "Engine/IO/Public/FileSystemFwd.hpp"
#include "Engine/Math/Public/MathAPI.hpp"

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

#include "Engine/Graphics/Public/GraphicsFwd.hpp"

#include <string>
#include <string_view>
#include <memory>
#include <optional>
#include <filesystem>
#include <cstdint>

namespace Engine::VFX
{
	enum class VFXKind : std::uint8_t
	{
		Sprite,		// 1枚テクスチャ
		Flipbook,	// 連番画像(コマ送り)
		Count,
	};

	enum class VFXBlend : std::uint8_t
	{
		Add,
		Alpha,
	};

	struct VFXDef
	{
		std::string path;					// res://VFX/xxx01.vfx.json のような入口ファイルのパス
		VFXKind kind = VFXKind::Sprite;
		VFXBlend blend = VFXBlend::Add;
		float duration = 0.0f;				// default 寿命
		float defaultScale = 1.0f;
	};

	// VFXDef(定義) - ID ペア (CRUD) (Update/Deleteは後回しor不要)
	// 
	class VFXCatalog
	{
	public:

		VFXCatalog();
		~VFXCatalog();

		// move
		VFXCatalog(VFXCatalog&&) noexcept;
		VFXCatalog& operator=(VFXCatalog&&)noexcept;

		// コピー禁止
		VFXCatalog(const VFXCatalog&) = delete;
		VFXCatalog& operator=(const VFXCatalog&) = delete;

		VFXID register_vfx(std::string, VFXDef def);
		const VFXDef* try_get(VFXID id) const noexcept;
		VFXID find(std::string_view name) const noexcept;

		// 入口ファイルのパス解決
		std::optional<std::filesystem::path>
			get_or_resolve_abs_path(VFXID id, const Engine::IO::IPathResolver& resolver) const;


	private:
		struct Impl;
		std::unique_ptr<Impl> impl_;
	};

	struct VFXCatalogResource
	{
		VFXCatalog catalog;
	};

	// 再生するVFXの種類(ID) + 制御情報
	struct VFXCmd
	{
		VFXID id = VFXID::Invalid();
		::Engine::Math::TRS trs;
		float ttlOverride = -1.0f;	// -1 なら Def の durationを使う
	};

	struct VFXCmdBuffer
	{
		std::vector<VFXCmd> cmds;

		void clear()
		{
			cmds.clear();
		}

		void add(VFXCmd c)
		{
			cmds.push_back(std::move(c));
		}

		bool empty() const
		{
			return cmds.empty();
		}
	};

	struct VFXCmdBufferResource
	{
		VFXCmdBuffer cmd;
	};

	struct VFXActiveInstanceResource
	{
		struct VFXInstance
		{
			VFXID id;
			::Engine::Math::TRS trs;
			float t = 0.0f;
			float ttl = 0.0f;
		};

		std::vector<VFXInstance> instances;
	};

	struct VFXSystem
	{
	public:

		void Update(const VFXCatalogResource& cat,
			VFXCmdBufferResource& res_cmd,
			VFXActiveInstanceResource& active,
			::Engine::IO::IPathResolver& resolver,
			float dt);

	private:
	};

	struct VFXEntrySprite
	{
		std::string textureVPath;
		std::string blend;
		float defaultScale = 1.0f;
		float duration = 1.0f;
	};



	struct VFXRuntimeData
	{
		VFXKind kind;
		VFXBlend blend;
		float defaultScale;
		float duration;

		::Engine::Graphics::TextureHandle tex;
	};

	class VFXRuntimeCache
	{
	public:
		const VFXRuntimeData* get_or_load(
			VFXID id,
			const VFXCatalog& catalog,
			const ::Engine::IO::IPathResolver& resolver,
			::Engine::Graphics::ITextureProvider& texProvider
		);

	private:
		std::vector<std::optional<VFXRuntimeData>> cache_;
	};

	void InitAllVFXResourceSystem(::Engine::WorldSystem::Core::WorldCtx& ctx);

	void UpdateAllVFXSystem(::Engine::WorldSystem::Core::WorldCtx& ctx);
}