#include "TileMapActor.h"

#include "Engine/ECS/Entity.h"

#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"
#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"

#include "Engine/ECS/Component/Tags/TileMapTag.h"



#include "Game/Init/Private/InitModel/InitLogicTransformFromModel.h"
#include "Game/Init/Private/InitTileMap/InitTileMap.h"



#include "Engine/ECS/Ops/CoreOps.hpp"

#include <cstdint>

//Engine::ECS::Entity Game::Actor::Map::TileMapActor::Create(
//	Engine::ECS::EntityMgr& ecs,
//	const Engine::ECS::Component::Common::TransformComponent& transform,
//	const Engine::Graphics::Model::ModelData& modelData,
//	float tileSize
//)
//{
//	//namespace Ops = Engine::ECS::Ops;
//	//namespace Comp = Engine::ECS::Component;
//	//
//
//	//Engine::ECS::Entity e = ecs.createEntity();
//
//	//// コンポーネント初期化
//	//// TerrainMeshの情報をコピーする
//	//Comp::Logic2D::Logic2DTransformComponent tempLogic2D = 
//	//	Game::Init::Logic2D::InitLogic2DTransformFromModel(transform, modelData);
//	//// ecs.addComponent(entity, logic2DComp);
//
//	//// タイルマップ情報初期化
//	//Comp::Logic2D::TileMapComponent tileMapComp;
//	//tileMapComp.tileSize = tileSize;
//	//tileMapComp = Game::Init::Logic2D::InitTileMapFromBounds(transform, modelData, tempLogic2D, tileSize);
//	//Game::Init::Logic2D::InitTileMapTiles(tileMapComp);
//	//Ops::Add<Comp::Logic2D::TileMapComponent>(ecs, e, tileMapComp);
//
//	//// 論理transformの再初期化(タイルマップ自体は回転を考えない)
//	//Comp::Logic2D::Logic2DTransformComponent fixedLogic2D;
//	//fixedLogic2D.positionXZ = tempLogic2D.positionXZ;
//	//fixedLogic2D.rotation = 0.0f;
//	//Ops::Add<Comp::Logic2D::Logic2DTransformComponent>(ecs, e, fixedLogic2D);
//
//
//	//Ops::Add<Comp::Tags::MainTileMapTag>(ecs, e, Comp::Tags::MainTileMapTag{});
//
//
//	//Engine::Debug::Logging::debugLog("Tile Map Actor Created Successfully!", "TileMapActor.cpp(Create)");
//	//
//	//return e;
//}