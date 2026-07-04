#include "Engine/Audio/Internal/Backends/MiniAudioBackend.hpp"

#include "Engine/Audio/Internal/AudioLog.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include <utility>
#include <miniaudio.h>
#include <memory>
#include <filesystem>
#include <vector>

namespace Engine::Audio
{
	struct MiniAudioBackend::Impl
	{
		ma_engine engine{};
		bool initialized = false;

		struct Voice
		{
			ma_sound sound{};
			bool inited = false;

			// path文字列が内部で参照される可能性をつぶすため、念のため保持
			// 少なくともvoice の寿命中は確実に生きる
			std::string path;
		};

		std::vector<std::unique_ptr<Voice>> activeVoices;
		std::size_t maxVoices = 64;		// 最大同時発生音数 (仮)
	};

	MiniAudioBackend::MiniAudioBackend()
		:impl_(std::make_unique<Impl>())
	{

	}

	MiniAudioBackend::~MiniAudioBackend()
	{
		shutdown();
	}

	MiniAudioBackend::MiniAudioBackend(MiniAudioBackend&& other) noexcept
		:impl_(std::move(other.impl_))
	{

	}

	MiniAudioBackend& MiniAudioBackend::operator=(MiniAudioBackend&& other) noexcept
	{
		if (this != &other)
		{
			shutdown();
			impl_.reset();
			impl_ = std::move(other.impl_);
			other.impl_.reset();
		}
		return *this;
	}

	bool MiniAudioBackend::initialize()
	{
		if (!impl_ || impl_->initialized)
		{
			return true;
		}

		const ma_result r = ma_engine_init(nullptr, &impl_->engine);
		if (r != MA_SUCCESS)
		{
			Log::error(Log::kBackend, "MiniAudio::initialize (failed to init MiniAudioEngine)");
			impl_->initialized = false;
			return false;
		}

		impl_->initialized = true;
		return true;
	}

	void MiniAudioBackend::shutdown()noexcept
	{
		if (!impl_ || !impl_->initialized)
		{
			Log::error(Log::kBackend, "MiniAudio::shutdown (unexpected)");
			return;
		}

		ma_engine_uninit(&impl_->engine);
		impl_->initialized = false;
	}

	void MiniAudioBackend::set_max_voices(std::size_t n)
	{
		if (!impl_)return;
		impl_->maxVoices = std::max<std::size_t>(n, 1);
	}

	void MiniAudioBackend::play_one_shot(std::string_view path, float volume)
	{
		(void)volume; // 最小実装では無視。後でバス/ボイス管理で対応。
		if (!impl_ || !impl_->initialized)
		{
			Log::error(Log::kBackend, "MiniAudio::play_one_shot (unexpected)");
			return;
		}

		std::string pathZ(path); // null終端が必要
		const ma_result r = ma_engine_play_sound(&impl_->engine, pathZ.c_str(), nullptr);
		if (r != MA_SUCCESS)
		{
			std::cerr << "play_sound failed: " << pathZ << "r=" << r << "\n";
		}

		// if (!impl_ || !impl_->initialized)
		// {
		// 	return;
		// }
		// 
		// // volume の最低限の安全化 (上位でもやるなら不要)
		// volume = std::clamp(volume, 0.0f, 10.0f);
		// 
		// // ma_engine_play_sound は音量指定ができないので、
		// // 一度 ma_sound を作って音量を設定してから再生する
		// ma_sound sound{};
		// const ma_uint32 flags = 0;
		// 
		// // path は null 終端が必要なので一度 std::string にする
		// std::string pathZ(path);
		// 
		// if (ma_sound_init_from_file(&impl_->engine, pathZ.c_str(), flags, nullptr, nullptr, &sound) != MA_SUCCESS)
		// {
		// 	return;
		// }
		// 
		// ma_sound_set_volume(&sound, volume);
		// ma_sound_start(&sound);


		// ワンショットなので fire-and-forget にしたいが、
		// ここで uninit すると音が止まる。
		// → 最小実装では「自動破棄管理」をまだ作っていないので、
		//    ma_sound を detatch してエンジンに管理させるやり方を取る。
		//
		// miniaudio の ma_engine は内部でサウンドを追跡しないため、
		// 本格的には「再生中サウンドのリスト」を backend が持ち、
		// 再生終了を検出して uninit する必要がある。
		//
		// まずは “動作確認” として、簡易に「エンジンで即時再生」したい場合は
		// ma_engine_play_sound を使ってしまうのが最短（音量指定不可）。
		//
		// ここでは「音量指定したい」を優先して、暫定策として
		// しばらく生存させる管理を後で追加する前提にする。

		// 暫定：このままだとリークします（後で必ず管理を足す必要あり）
		// なので、まずは音量指定不要なら ma_engine_play_sound を使ってください。
		// -----
		// ma_engine_play_sound(&impl_->engine, pathZ.c_str(), nullptr);
		// ma_sound_uninit(&sound);
	}

	void MiniAudioBackend::play_one_shot(const std::filesystem::path& path, float volume)
	{
		//(void)volume; // 最小実装では無視。後でバス/ボイス管理で対応。
		//if (!impl_ || !impl_->initialized)
		//{
		//	Log::error(Log::kBackend, "MiniAudio::play_one_shot (unexpected)");
		//	return;
		//}

		//const std::string pathZ = path.string(); // null終端が必要
		//const ma_result r = ma_engine_play_sound(&impl_->engine, pathZ.c_str(), nullptr);
		//if (r != MA_SUCCESS)
		//{
		//	std::cerr << "play_sound failed: " << pathZ << "r=" << r << "\n";
		//}

		// 

		if (!impl_ || !impl_->initialized)
		{
			Log::error(Log::kBackend, "MiniAudio::play_one_shot (engine not initialized)");
			return;
		}

		pump();

		if (impl_->activeVoices.size() >= impl_->maxVoices)
		{
			// 
			Log::warn(Log::kBackend, "SFX dropped (maxVoices reached)");
			return;
		}

		// 寿命中にアドレスが変わることを想定していない
		auto v = std::make_unique<Impl::Voice>();
		v->path = path.string();		// UTF-8周りは後で整備(Windowsならwstring版も検討)

		//
		constexpr ma_uint32 flags = MA_SOUND_FLAG_DECODE;
			
		
		const ma_result rInit = ma_sound_init_from_file(
			&impl_->engine,
			v->path.c_str(),
			flags,
			nullptr,			// group (後でSFXパスに差し替え)
			nullptr,			// doneFence (必要なら非同期完了待ちに使う)
			&v->sound
		);
		if (rInit != MA_SUCCESS)
		{
			Log::warn(Log::kBackend, "ma_sound_init_from_file failed.");
			return;
		}
		v->inited = true;

		ma_sound_set_volume(&v->sound, volume);

		const ma_result rStart = ma_sound_start(&v->sound);
		if (rStart != MA_SUCCESS)
		{
			Log::warn(Log::kBackend, "ma_sound_start failed");
			ma_sound_uninit(&v->sound);
			return;
		}

		impl_->activeVoices.emplace_back(std::move(v));

	}

	// 明示的ガベージコレクション
	void MiniAudioBackend::pump()
	{
		if (!impl_ || !impl_->initialized)
		{
			Log::warn(Log::kBackend, "MiniAudio::pump (engine not initialized)");
			return;
		}

		auto& a = impl_->activeVoices;

		// 終わった音を回収 (アドレスを動かさない)
		// vector(unique_ptr) なので erase しても Voice本体は moveされない
		for (std::size_t i = 0; i < a.size();)
		{
			Impl::Voice& v = *a[i];

			// at_end: データソース終端判定．終わってたら止めて破棄
			if (v.inited && ma_sound_at_end(&v.sound))
			{
				ma_sound_stop(&v.sound);
				ma_sound_uninit(& v.sound);

				// swap-pop(順序非依存だから)
				a[i] = std::move(a.back());
				a.pop_back();
				continue;
			}

			++i;
		}
	}
}

// memo
// MiniAudioが仕様上同一再生サウンドについてメモリ内で位置を移動することを想定していないことと、
// 音声再生リソースの寿命をフレームスタックからヒープに移すために、
// std::vector<std::unique_ptr<Voices>>を導入し、
// ヒープにリソースを作成するようにして、
// pump()でガベージコレクションを毎フレーム行うように実装した