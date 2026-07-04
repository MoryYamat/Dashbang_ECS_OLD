#pragma once 

namespace Game::ECS::Tags
{
	// PlayerCharacter Tag
	struct PlayerCharacterTag {};//	Actorの属性を指定(1つにつけるだけ)

	// Mob Tag
	struct MobTag {};

	// Enemy Character Tag
	struct EnemyCharacterTag {};

	// Neutral NPC Tag
	struct NeutralNPCTag {};
}