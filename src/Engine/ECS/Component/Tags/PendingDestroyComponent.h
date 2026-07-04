// 各システムでこのコンポーネントが追加された場合一括で削除するためのフラグ


#pragma once
namespace Engine::ECS::Component::Tags
{
	struct PendingDestroyComponent
	{
		int delayFrames = 0;// 削除遅延フレーム数
		bool fadeOutEffect = false;// フェードアウト処理などの有無フラグ
	};
}

