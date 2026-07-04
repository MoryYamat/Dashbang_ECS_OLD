#include "Engine/FSM/Public/FSMApi.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"

#include <unordered_map>
#include <vector>
#include <cstdint>
#include <string>
#include <string_view>
#include <span>
#include <iostream>

namespace Engine::FSM::Core
{
	// 部分評価: requiredBitsに含まれるoutBitIndexだけ実行
	void ExecuteDerivedCondsFiltered
	(
		const EnvAssemblerPlan& plan,
		std::span<const std::uint32_t> requiredBits,
		const IFieldReader& reader,
		BitEnvSnapshot& outBits
	)
	{
		// フィルタを O(1) 参照にするためにビットセット化(最大 outBitIndex + 1 がわからないので map でOK)
		// 規模が小さいうちは線形で十分．
		for (const auto& op : plan.ops)
		{
			// 必要な outBitIndex だけ実行
			bool needed = false;
			for (auto b : requiredBits)
			{
				if (b == op.outBitIndex)
				{
					needed = true;
					break;
				}
			}
			if (!needed) continue;

			const float v = reader.getF32(op.field);
			bool pass = false;
			switch (op.kind)
			{
			case CondKind::CompareF:
				switch (op.op)
				{
				case CmpOp::GT: pass = (v > op.f32_0); break;
				case CmpOp::GE: pass = (v >= op.f32_0); break;
				case CmpOp::LT: pass = (v < op.f32_0); break;
				case CmpOp::LE: pass = (v <= op.f32_0); break;
				default: pass = false; break;
				}
				break;

			case CondKind::InRange:
				pass = (op.f32_0 <= v && v <= op.f32_1);
				break;

			case CondKind::Bit:

				pass = false;
				break;
			}


			outBits.set(op.outBitIndex, pass);
		}
	}
}