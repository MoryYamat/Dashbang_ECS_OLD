// TODO: ゲーム側にあるべきコードを分ける (AudioIDs / AudioBus / など ) 
// コールバック化による初期化処理などの対応が必要
// 将来的に、Assetの登録・管理・運用できる一環したツールを作成することで対応する
// 

#pragma once
#include "Engine/Audio/Public/AudioFwd.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/IO/Public/FileSystemFwd.hpp"

#include <optional>
#include <filesystem>
#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <memory>

namespace Engine::Audio
{
	struct SoundDef
	{
		std::string path;
		AudioBus bus = AudioBus::SFX;
		float defaultVolume = 1.0f;
		bool stream = false;			// true -> streaming 
	};

	// public
	class AudioCatalog
	{
	public:
		AudioCatalog();
		~AudioCatalog();
		 
		AudioCatalog(AudioCatalog&&) noexcept;
		AudioCatalog& operator=(AudioCatalog&&)noexcept;

		// コピー禁止
		AudioCatalog(const AudioCatalog&) = delete;
		AudioCatalog& operator=(const AudioCatalog&) = delete;


		SoundID register_sound(std::string name, SoundDef def);
		const SoundDef* try_get(SoundID id) const noexcept;
		SoundID find(std::string_view name) const noexcept;

		std::optional<std::filesystem::path>
			get_or_resolve_abs_path(SoundID id, const Engine::IO::IPathResolver& resolver) const;


	private:
		struct Impl;
		std::unique_ptr<Impl> impl_;
	};

	struct AudioCatalogResource
	{
		AudioCatalog catalog;
	};

	struct AudioCmd
	{
		enum class Kind : std::uint8_t
		{
			SetBusVolume,
			PlayOneShot,
		};

		Kind kind = Kind::PlayOneShot;
		SoundID sound = SoundID::Invalid();
		float volume_scale = 1.0f;			// イベント1回限りの倍率

		AudioBus bus = AudioBus::Master;
		float bus_volume = 1.0f;		// 例: Master / SFX / BGM
	};

	struct AudioCmdBuffer
	{
		std::vector<AudioCmd> cmds;

		void clear() noexcept { cmds.clear(); }

		void play_one_shot(SoundID id, float volume_scale = 1.0f)
		{
			AudioCmd c;
			c.kind = AudioCmd::Kind::PlayOneShot;
			c.sound = id;
			c.volume_scale = volume_scale;
			cmds.push_back(c);
		}

		void set_bus_volume(AudioBus bus, float volume)
		{
			AudioCmd c;
			c.kind = AudioCmd::Kind::SetBusVolume;
			c.bus = bus;
			c.bus_volume = volume;
			cmds.push_back(c);
		}
	};

	struct AudioCmdBufferResource
	{
		AudioCmdBuffer cmd;

		bool empty() const noexcept
		{
			return cmd.cmds.empty();
		}
	};

	class AudioSystem
	{
	public:
		AudioSystem();
		~AudioSystem();

		AudioSystem(const AudioSystem&) = delete;
		AudioSystem& operator=(const AudioSystem&) = delete;

		// コピー禁止
		AudioSystem(AudioSystem&&) noexcept;
		AudioSystem& operator=(AudioSystem&&) noexcept;

		// WorldSystem lifecycle
		bool initialize();
		void shutdown() noexcept;
		// void update(float dt);
		void update(const AudioCatalogResource& resource, AudioCmdBufferResource& cmds, Engine::IO::IPathResolver& resolver, float dt);

		void set_max_voices(std::size_t n);

		float get_bus_volume(AudioBus bus) const noexcept;

		// World側から Catalog を差し込む (resource を握らせる)
		void bind_catalog(const void* catalog_ptr) noexcept;

	private:
		float compute_final_volume(const SoundDef& def, float volume_scale) const noexcept;
		static std::size_t bus_index(AudioBus bus) noexcept;

		// バス音量
		void set_bus_volume(AudioBus bus, float volume);

		// Master/SFX/BGM
		std::array<float, 3> bus_volume_{};

		struct Impl;
		std::unique_ptr<Impl> impl_;

	};

	void InitAllAudioResourceSystem(::Engine::WorldSystem::Core::WorldCtx& ctx);


}

// 使用方法の案

// GameApp{ private unique_ptr<AudioSystem> audiosys_;
// 
// 
// 
// 
// 
// 