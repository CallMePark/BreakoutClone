#pragma once

#include "Math.h"
#include "CollisionComponent.h"

#include <unordered_set>
#include <vector>

// BLOCKSIZE(.png) = 64 x 32
constexpr int NUM_CELL_ROW = 24; // WINDOW_HEIGHT / BLOCKSIZE_H
constexpr int NUM_CELL_COL = 16; // WINDOW_WIDTH / BLOCKSIZE_W
constexpr float CELL_H = 32.0f; // BLOCKSIZE_H
constexpr float CELL_W = 64.0f; // BLOCKSIZE_W
constexpr float INV_CELL_H = 1.0f / CELL_H; // Expensive division at compile time
constexpr float INV_CELL_W = 1.0f / CELL_W;

struct Cell
{
	class Actor* member;
	int row, col;
	AABB box;

	Cell() 
		: row(-1), col(-1), box(Vector2D::Zero, Vector2D::Zero), member(nullptr)
	{}

	Cell(int inRow, int inCol, class Actor* inMember) 
		: row(inRow), col(inCol), member(inMember)
	{
		Vector2D min{ inCol * CELL_W, inRow * CELL_H };
		Vector2D max{ (inCol + 1) * CELL_W, (inRow + 1) * CELL_H };
		box = AABB(min, max);
	}

	bool operator==(const Cell& other) const
	{
		return (row == other.row) && (col == other.col);
	}

	bool operator!=(const Cell& other) const
	{
		return (row != other.row) || (col != other.col);
	}
};

class GridSpatialPartition
{
public:
	GridSpatialPartition();

	void AssignCellMember(class Actor* inMember);
	void RemoveCellMember(class Actor* inMember);
	void ClearAllCells();
	
	bool IsInEmptyCell(class Actor* inMember, std::vector<Cell>& outOccupiedCells);

	void RenderGridDebug(struct SDL_Renderer* renderer);

private:
	std::vector<Cell> GetOverlappingCells(const Actor* inMember);
	Cell& PositionToCell(const Vector2D& inPosition);
	void AABBToCells(const AABB& inAABB, std::vector<Cell>& cells);

	Cell mGrid[NUM_CELL_ROW][NUM_CELL_COL];
	int mGridSize;
};