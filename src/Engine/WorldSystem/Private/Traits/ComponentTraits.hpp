#pragma once


// worldは型情報を削除(void)してポインタを保持しているため
// 外部から型についてのメタ情報を提供する役割を持つ
// 破棄時処理 / 追加時処理 / コピークローン可否 / ネットワーク同期可否 / 物理更新対象 など
namespace Engine::WorldSystem::Traits
{
	// デストロイメンバの有無を検出
	template<typename T>
	concept HasDestroyMethod = requires(T& x) { x.Destroy(); };

	template<typename T>
	struct ComponentTraits
	{
		// 既定: 追加/削除フックは no-op
		static void OnAdd(T&) noexcept {}

		static void OnRemove(T& x) noexcept 
		{
			if constexpr (HasDestroyMethod<T>)
			{
				x.Destroy();
			}
		}
	};
}