#pragma once

#include "Math.h"

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
	std::unordered_set<const class CollisionComponent*> members;
	
	int row, col;
	Vector2D cellPosition; // Top-left coordinate of the cell

	Cell() 
		: row(-1), col(-1), cellPosition(0.0f, 0.0f)
	{}
	Cell(int inRow, int inCol, const Vector2D& inCellPosition) 
		: row(inRow), col(inCol), cellPosition(inCellPosition)
	{}

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

	void AddCellMember(const class CollisionComponent* inMember);
	void RemoveCellMember(const class CollisionComponent* inMember);
	void ClearAllCells();
	
	bool IsInEmptyCell(const class CollisionComponent* inMember);
	const std::unordered_set<const class CollisionComponent*>& GetCellMembers(const class CollisionComponent* inMember);

	void RenderGridDebug(struct SDL_Renderer* renderer);

private:
	Cell& PositionToCell(const Vector2D& inPosition);
	void AABBToCells(const struct AABB& inAABB, std::vector<Cell>& cells);

	Cell mGrid[NUM_CELL_ROW][NUM_CELL_COL];
	int mGridSize;
};