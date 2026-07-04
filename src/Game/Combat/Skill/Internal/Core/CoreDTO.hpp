// 作者が定義

#pragma once

#include <string>
#include <vector>
#include <array>

namespace Game::Combat::Skill
{
	struct SkillPhaseDTO
	{
		std::string name;			// "Casting", "Active",... // FSM定義と同じにする必要あり
		float duration;				// フェーズの基準時間
		// 将来: cancelWindowStart, cancelWindowEndなど
	};


	struct SkillEffectDefDTO
	{
		std::string phase;			// どのフェーズで
		float timeOffset = 0.f;			// フェーズ開始から何秒後に
		float lifetime = 0.f;				// 寿命
		std::string kind;			// "SpawnHitbox", "PlayAnim",...
		//std::string param;			// ヒットボックスID や アニメ名など
		float value = 0.f;				// Movement 用: 例) 0.5f → 50% など
	};

	struct SkillDefDTO
	{
		std::string name;			// skill名"Skill_211"
		std::vector<SkillPhaseDTO> phases;
		std::vector<SkillEffectDefDTO> effects;

		// FSM/Profile名への紐づけなど
		std::string fsm;			// (基本は"Basic")  fsm のどの遷移構造(幾何的グラフ)を使用するか
		std::string profile;		// (基本は"Default")skill fsm の どのプロファイルを使用するか (どのSkillFSMのfsm定義を使用するかという自由度もあるが今は1つしかFSM定義がないと仮定する)
	};

	// 人が定義・読む
	struct SkillSlotLoadoutDTO
	{
		std::array<std::string, 4> slotSkillnames;// {"Fireball", "Dash", ...}

	};
}