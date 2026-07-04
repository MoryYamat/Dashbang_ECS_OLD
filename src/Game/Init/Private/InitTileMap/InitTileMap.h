// Initialize logical data from drawing data

#pragma once

#include "Engine/ECS/EntityManager.h"

// #include "Engine/Graphics/Private/Model/ModelData.h"
#include "Engine/Graphics/Public/GraphicsFwd.hpp"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Engine/ECS/Component/Common/TransformComponent.h"

#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

//
//
#include "Engine/Public/EngineFwd.hpp"


#include <string>



namespace Game::Init::Logic2D
{
	// BaseMeshデータ、論理データ、設定タイルサイズからTileMapComponentを初期化する関数
	Engine::ECS::Component::Logic2D::TileMapComponent InitTileMapFromBounds(
		const Engine::ECS::Component::Common::TransformComponent& transformComp
		, const Engine::Graphics::Model::ModelData& modelData
		, const Engine::ECS::Component::Logic2D::Logic2DTransformComponent& logic2DComp
		, float tileSize = 1.0f);

	// 設定済みのTileMapComponentの内容からtiles行列(配列)を初期化する関数
	void InitTileMapTiles(Engine::ECS::Component::Logic2D::TileMapComponent& tileMapComp);

	// モデルの回転を考慮したワールド座標における最大／最小の点の矩形座標を求める関数
	glm::vec2 GetModelWorldAABBSizeXZ(const Engine::ECS::Component::Common::TransformComponent& transformComp, const Engine::Graphics::Model::ModelData& modelData);

	// 最大／最小座標の8点 を返す関数
	glm::vec3 GetAABBCorner(glm::vec3 min, glm::vec3 max, int i);

	// 論理タイルマップの原点のワールド座標における位置を求める
	glm::vec2 ComputeTileMapOriginFromModel(const Engine::ECS::Component::Common::TransformComponent& transformComp, const Engine::Graphics::Model::ModelData& modelData);


	// 中心点がTerrainMeshのAABB内にあるかチェックする用の関数
	//bool isInsideTerrainDrawBounds(glm::vec2 pointXZ, const ModelData& modelData);

	// to init to walkable attribution by terrain mesh
	//void InitWalKableByTerrain(TileMapComponent& tileMapComp, const ModelData& modelData);
}

// For set tile attributes
namespace Game::Init::Logic2D
{
	// 障害物アクターとかぶるタイルマップの属性を`unWalkable`へ設定する
	void ApplyObstacleCollidersToTileMap(Engine::ECS::EntityMgr& ecs, Engine::ECS::Component::Logic2D::TileMapComponent& tileMapComp);

	// 中空にあるタイルマップの属性を`unWalkable`へ設定
	void MaskUncoveredTilesByTerrainOBB(Engine::ECS::EntityMgr& ecs, Engine::ECS::Component::Logic2D::TileMapComponent& tileMapComp);
}


// 
// 

namespace Game::Init::Logic2D
{
	// BaseMeshデータ、論理データ、設定タイルサイズからTileMapComponentを初期化する関数
	Engine::ECS::Component::Logic2D::TileMapComponent InitTileMapFromBounds(
		const Engine::Component::TransformComponent& transformComp
		, const Engine::Graphics::Model::ModelData& modelData
		, const Engine::Component::Logic2DTransformComponent& logic2DComp
		, float tileSize = 1.0f);

	// 設定済みのTileMapComponentの内容からtiles行列(配列)を初期化する関数
	void InitTileMapTiles(Engine::Component::TileMapComponent& tileMapComp);

	// モデルの回転を考慮したワールド座標における最大／最小の点の矩形座標を求める関数
	glm::vec2 GetModelWorldAABBSizeXZ(const Engine::Component::TransformComponent& transformComp, const Engine::Graphics::Model::ModelData& modelData);

	// 最大／最小座標の8点 を返す関数
	glm::vec3 GetAABBCorner(glm::vec3 min, glm::vec3 max, int i);

	// 論理タイルマップの原点のワールド座標における位置を求める
	glm::vec2 ComputeTileMapOriginFromModel(const Engine::Component::TransformComponent& transformComp, const Engine::Graphics::Model::ModelData& modelData);


	// 障害物アクターとかぶるタイルマップの属性を`unWalkable`へ設定する
	void ApplyObstacleCollidersToTileMap(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Component::TileMapComponent& tileMapComp);

	// 中空にあるタイルマップの属性を`unWalkable`へ設定
	void MaskUncoveredTilesByTerrainOBB(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Component::TileMapComponent& tileMapComp);

}