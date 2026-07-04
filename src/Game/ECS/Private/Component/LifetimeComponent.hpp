// lifetimeComp

#pragma once

namespace Game::ECS::Component
{
	struct LifetimeComponent
	{
		float totalLifetime = 0.0f; // コンポーネントのライフタイム
		float elapsedTime = 0.0f; // 経過時間
		bool isExpired() const
		{
			return elapsedTime >= totalLifetime; // ライフタイムを超えたかどうか
		}
	};
}