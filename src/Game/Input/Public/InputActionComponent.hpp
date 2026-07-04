// Entityのアクション状態を保持	
// Maintains the action state of the entity

#pragma once


#include <cstdint>
#include <unordered_map>

namespace Game::Input
{
	// TODO: 公開面を隠ぺいする構造にするよう検討
	enum class InputAction
	{
		None = 0,

		// 共通的な操作
		Confirm,
		Cancel,
		Pause,
		Quit,


		// 移動
		MoveForward,
		MoveBackward,
		MoveLeft,
		MoveRight,

		// 視点操作，ジャンプなど
		LookAround,
		Jump,

		// スキル・攻撃
		CastSkill1,
		CastSkill2,
		CastSkill3,
		CastSkill4,

		// UI用
		SelectUp,
		SelectDown,
		SelectLeft,
		SelectRight,
	};

	struct InputActionComponent
	{
		// 現在のフレームで有効なアクション
		std::unordered_map<InputAction, bool> current;

		// 1フレーム前の状態(isJustPressedなｄの検出に使う)
		std::unordered_map<InputAction, bool> previous;

		// 今押されてるか
		bool isPressed(InputAction action) const
		{
			auto it = current.find(action);
			return it != current.end() && it->second;
		}

		// 今フレームで押された瞬間か?
		bool isJustPressed(InputAction action) const
		{
			return isPressed(action) && (!previous.count(action) || !previous.at(action));
		}

		// 今フレームで離された瞬間か
		bool isJustReleased(InputAction action) const
		{
			return !isPressed(action) && previous.count(action) && previous.at(action);
		}
	};
}

