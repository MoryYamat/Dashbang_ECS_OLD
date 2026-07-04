#include "Engine/FSM/Public/Core/ConditionBank.hpp"



namespace Engine::FSM::Core
{
	void ConditionBank::compose(BitEnvSnapshot& out, std::uint32_t numConds) const
	{
		out.ensureSize(numConds);
		out.clearAll();

		const auto combine = [&](const BitEnvSnapshot& src)
			{
				const auto words = src.bits.size();
				if (words == 0) return;
				out.ensureSize(words << BitEnvSnapshot::kWordShift);
				for (size_t i = 0; i < words; ++i)
				{
					// OR合成
					out.bits[i] |= src.bits[i];
				}
			};

		// 優先度順(Derived → Base → Latched → DebugOverride)
		combine(ch[(size_t)CondChannel::Derived]);
		combine(ch[(size_t)CondChannel::Base]);
		combine(ch[(size_t)CondChannel::Latched]);
		combine(ch[(size_t)CondChannel::DebugOverride]);
	}


	void ConditionBank::latchForFrames(std::uint32_t condBit, std::uint16_t frames)
	{
		if (frames == 0) return;
		ch[(size_t)CondChannel::Latched].set(condBit, true);
		// 再ラッチ時の語義は「延長(上書き)」
		framesLeft_[condBit] = std::max(framesLeft_[condBit], frames);
	}

	void ConditionBank::latchForSeconds(std::uint32_t condBit, float seconds)
	{
		if (seconds <= 0.f)  return;
		ch[(size_t)CondChannel::Latched].set(condBit, true);
		// 再ラッチは「最大値に延長」(加算も考えられるが、まず延長)
		auto& left = secondsLeft_[condBit];
		left = std::max(left, seconds);
	}

	void ConditionBank::tickLatchedFrames()
	{
		if (framesLeft_.empty()) return;
		auto& latched = ch[(size_t)CondChannel::Latched];
		for (auto it = framesLeft_.begin(); it != framesLeft_.end();)
		{
			if (it->second == 0)
			{
				latched.set(it->first, false);
				it = framesLeft_.erase(it);
			}
			else
			{
				--(it->second);
				++it;
			}
		}
	}

	void ConditionBank::tickLatchedSeconds(float dt)
	{
		if (secondsLeft_.empty())  return;
		auto& latched = ch[(size_t)CondChannel::Latched];
		for (auto it = secondsLeft_.begin(); it != secondsLeft_.end();)
		{
			it->second -= dt;
			if (it->second <= 0.f)
			{
				latched.set(it->first, false);
				it = secondsLeft_.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}