// TileMap base/unit data

#pragma once

#include <glm/glm.hpp>

#include <vector>
namespace Engine::ECS::Component::Logic2D
{
	struct Tile
	{
		bool isWalkable = true;
		int tileType = 0;
	};


	// Data for the entire TileMap
	struct TileMapComponent
	{
		int numCols;// 横の枚数
		int numRows;// 縦の枚数
		float tileSize = 1.0f;// タイルの1辺のサイズ

		glm::vec2 origin = { 0, 0 };// タイルの左下の`ワールド座標`

		std::vector<std::vector<Tile>> tiles;


		inline std::pair<glm::vec2, glm::vec2> GetTileAABB(int row, int col) const
		{
			glm::vec2 tileMin = origin + glm::vec2(col, row) * tileSize;
			glm::vec2 tileMax = tileMin + glm::vec2(tileSize);
			return { tileMin, tileMax };
		}


		glm::vec2 GetTileCenter(int row, int col)
		{
			return origin + glm::vec2(col + 0.5f, row + 0.5f) * tileSize;
		}
	};
}
