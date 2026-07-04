// tile grid XZ

#pragma once

#include "Engine/Config/CanonicalDefaults.h"

#include "Engine/Math/Private/Logic2D/LogicMathUtils.h"



#include <GLM/glm.hpp>

// 現在未使用　(3次元論理空間における二次元タイルを考える仕様のため)
namespace Engine::ECS::Component::Logic2D
{
	struct TileData
	{
		bool isWalkable = true;
		int tileType = 0;
	};

	// Y = 0にある論理平面のデータ構造
	struct TileGridXZComponent
	{
		int numCols;
		int numRows;
		float tileSize = 1.0f;

		// origin は(0, 0, 0)で Zがrow方向，Xがcol方向に広がる 
		glm::vec3 origin = CanonicalDefaults::kCanonicalOrigin;

		//tiles[row][col]
		std::vector<std::vector<Engine::ECS::Component::Logic2D::TileData>> tiles;

		//
		inline std::pair<glm::vec3, glm::vec3> GetTileAABB(int row, int col) 
		{
			glm::vec3 tileMin = origin + glm::vec3(col * tileSize, 0.0f, row * tileSize);
			glm::vec3 tileMax = tileMin + glm::vec3(tileSize, 0.0f, tileSize);
			return { tileMin, tileMax };
		}

		//
		inline glm::vec3 GetTileCenter(int row, int col)
		{
			return origin + glm::vec3((col + 0.5f) * tileSize, 0.0f, (row + 0.5f) * tileSize);
		}
	};
}