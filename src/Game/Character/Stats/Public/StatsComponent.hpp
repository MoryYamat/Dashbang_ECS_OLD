// キャラクターの固有値のデータ群


#pragma once

namespace Game::Character::Stats
{
	struct CharacterStatsComponent
	{
		float maxHp = 100.0f;
		float currentHP = 100.0f;


		float moveSpeed = 5.0f;// base move speed
		float acceleration = 0.0f;
	};
}

