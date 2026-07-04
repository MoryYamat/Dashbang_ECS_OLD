#include "Engine/Audio/Public/AudioAPI.hpp"

#include "Engine/Audio/Public/AudioFwd.hpp"
#include "Engine/Audio/Internal/Backends/MiniAudioBackend.hpp"

#include "Engine/Audio/Internal/AudioLog.hpp"

#include "Engine/IO/Public/FileSystemAPI.hpp"

#include <cstddef>
#include <memory>
#include <utility>
#include <algorithm>
#include <iostream>


namespace Engine::Audio
{
	struct AudioSystem::Impl
	{
		MiniAudioBackend backend;

		// World Resource ÇéwÇ∑ (èäóLÇπÇ∏)
		const AudioCatalog* catalog = nullptr;

		bool initialized = false;
	};

	std::size_t AudioSystem::bus_index(AudioBus bus) noexcept
	{
		//  ç°ÇÕ Master/SFX/BGMÇÃ3Ç¬Ç…å¿íË
		return static_cast<std::size_t>(bus);
	}

	AudioSystem::AudioSystem()
		: bus_volume_{ 1.0f, 1.0f, 1.0f }
		, impl_(std::make_unique<Impl>())
	{
	}

	AudioSystem::~AudioSystem()
	{
		shutdown();
	}

	AudioSystem::AudioSystem(AudioSystem&& other) noexcept
		: bus_volume_(other.bus_volume_)
		, impl_(std::move(other.impl_))
	{
	}

	AudioSystem& AudioSystem::operator=(AudioSystem&& other) noexcept
	{
		if (this != &other)
		{
			shutdown();

			bus_volume_ = other.bus_volume_;
			impl_ = std::move(other.impl_);
		}
		return *this;
	}

	void AudioSystem::bind_catalog(const void* catalog_ptr) noexcept
	{
		if (!impl_)
		{
			Log::error(Log::kSystem, "bind_catlog failed (unexpected)");
			return;
		}
		impl_->catalog = static_cast<const AudioCatalog*>(catalog_ptr);
	}

	bool AudioSystem::initialize()
	{
		if (!impl_)
		{
			Log::error(Log::kSystem, "initialize failed (unexpected)");
			return false;
		}
		if (impl_->initialized)
		{
			Log::info(Log::kSystem, "initialize failed (AudioSystem has already been initialized)");
			return true;
		}
		
		if (!impl_->backend.initialize())
		{
			impl_->initialized = false;
			Log::error(Log::kSystem, "initialize failed (AudioBackend initialization failed)");
			return false;
		}

		impl_->initialized = true;
		return true;
	}

	void AudioSystem::shutdown() noexcept
	{
		if (!impl_ || !impl_->initialized)
		{
			Log::error(Log::kSystem, "AudioSystem failed to shut down gracefully");
			return;
		}
			
		impl_->backend.shutdown();
		impl_->initialized = false;
	}

	void AudioSystem::set_max_voices(std::size_t n)
	{
		if (!impl_)
			return;
		impl_->backend.set_max_voices(n);
	}

	void AudioSystem::update(const AudioCatalogResource& resource, AudioCmdBufferResource& cmds, Engine::IO::IPathResolver& resolver, float dt)
	{
		const auto& catalog = resource.catalog;

		if (cmds.empty())
		{
			cmds.cmd.clear();
			impl_->backend.pump();
			return;
		}
			
		// std::cerr << "sfx request existed\n"; // for debug


		for (const auto& c : cmds.cmd.cmds)
		{
			switch (c.kind)
			{
			case ::Engine::Audio::AudioCmd::Kind::PlayOneShot:
			{
				const Engine::Audio::SoundDef* def = catalog.try_get(c.sound);
				if (!def) break;

				const auto absOpt = catalog.get_or_resolve_abs_path(c.sound, resolver);
				if (!absOpt)
				{
					Log::warn(Log::kSystem, "Failed to resolve absolute path");
					continue;
				}

				//impl_->backend.play_one_shot(std::string_view(def->path));
				float vol = compute_final_volume(*def, 1.0f);
				impl_->backend.play_one_shot(*absOpt, vol);
				break;
			}
			case ::Engine::Audio::AudioCmd::Kind::SetBusVolume:
			{
				set_bus_volume(c.bus, c.bus_volume);
				break;
			}
			default:
				break;
			}
		}

		cmds.cmd.clear();

		impl_->backend.pump();
	}

	void AudioSystem::set_bus_volume(AudioBus bus, float volume)
	{
		const std::size_t i = bus_index(bus);
		if (i >= bus_volume_.size())
		{
			Log::error(Log::kSystem, "set_bus_volume (Invalid SoundID)");
			return;
		}
		
		bus_volume_[i] = std::clamp(volume, 0.0f, 10.0f);
	}

	float AudioSystem::get_bus_volume(AudioBus bus) const noexcept
	{
		const std::size_t i = bus_index(bus);
		if (i >= bus_volume_.size())
		{
			Log::error(Log::kSystem, "get_bus_volume (Invalid bus_index)");
			return 1.0f;
		}
		
		return bus_volume_[i];
	}

	float AudioSystem::compute_final_volume(const SoundDef& def, float volume_scale) const noexcept
	{
		const float master = bus_volume_[bus_index(AudioBus::Master)];
		const float bus = bus_volume_[bus_index(def.bus)];
		const float base = def.defaultVolume;

		return std::clamp(master * bus * base * volume_scale, 0.0f, 10.0f);
	}
}