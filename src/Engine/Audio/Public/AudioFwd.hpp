#pragma once

#include "Engine/Core/Public/StrongID.hpp"
#include <cstdint>

namespace Engine::Audio
{
	// ミキサーのフェーダー(音量つまみ)の単位
	enum class AudioBus : std::uint8_t
	{
		Master = 0,			// 全体
		SFX,				// 効果音
		BGM,				// BGM
		Count				// 配列サイズ
	};


	struct SoundIDTag {};
	using SoundID = Engine::Core::StrongID<SoundIDTag, std::uint32_t>;




	class AudioCatalog;
	struct SoundDef;	
	class AudioSystem;

}