#pragma once
#include "Engine/FSM/Public/Core/Types.hpp"

#include <cstdint>
#include <array>
#include <cstddef>

namespace Engine::FSM::Core
{
	enum class CondChannel : std::uint8_t
	{
		DebugOverride,
		Latched,			// ゲーム側/FSMエンジン(1～Nフレーム維持): 攻撃ヒット時のフラグや一時的CCフラグなど
		Base,				// ゲーム側システム用(毎フレーム更新): 基本の外部状態 
		Derived,			// FSMエンジン内部(毎フレーム更新): CompareF/InRangeなどのデータ駆動条件
		COUNT, 
	};

	struct ConditionBank
	{
		std::array<BitEnvSnapshot, (size_t)CondChannel::COUNT> ch;

		void beginFrame()
		{
			ch[(size_t)CondChannel::Base].clearAll();
			ch[(size_t)CondChannel::Derived].clearAll();
		}

		void set(CondChannel c, std::uint32_t bit, bool v)
		{
			ch[(size_t)c].set(bit, v);
		}

		void latch(std::uint32_t bit)
		{
			set(CondChannel::Latched, bit, true);
		}

		void unlatch(std::uint32_t bit)
		{
			set(CondChannel::Latched, bit, false);
		}

		void overrideOn(std::uint32_t bit)
		{
			set(CondChannel::DebugOverride, bit, true);
		}

		void overrideOff(std::uint32_t bit)
		{
			set(CondChannel::DebugOverride, bit, false);
		}

		// 遷移評価前にビットをOR合成
		void compose(BitEnvSnapshot& out, std::uint32_t numConds) const;

		// ラッチ寿命管理
		void latchForFrames(std::uint32_t condBit, std::uint16_t frames);
		void latchForSeconds(std::uint32_t condBit, float seconds);
		void tickLatchedFrames();
		void tickLatchedSeconds(float dt);

	private:
		// アクティブなラッチだけを保持(スパース)
		std::unordered_map<std::uint32_t, std::uint16_t> framesLeft_;
		std::unordered_map<std::uint32_t, float> secondsLeft_;
	};
}