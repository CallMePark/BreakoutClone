#pragma once

#include "Math.h"

#include <unordered_set>

// BLOCKSIZE(.png) = 64 x 32
constexpr int NUM_CELL_ROW = 24; // WINDOW_HEIGHT(768) / BLOCKSIZE_H(32)
constexpr int NUM_CELL_COL = 16; // WINDOW_WIDTH(1024) / BLOCKSIZE_W(64)
constexpr float CELL_H = 32.0f; // BLOCKSIZE_H
constexpr float CELL_W = 64.0f; // BLOCKSIZE_W
constexpr float INV_CELL_H = 1.0f / CELL_H; // Expensive division at compile time
constexpr float INV_CELL_W = 1.0f / CELL_W;

// The Actor* in Cell will be managed in CollisionComponent constructor/destructor
struct Cell
{
	std::unordered_set<const class Actor*> members;
	int row, col;

	Cell() : row(-1), col(-1) {}
	Cell(int inRow, int inCol) : row(inRow), col(inCol) {}

	bool operator!=(const Cell& other) const
	{
		return (row != other.row) || (col != other.col);
	}
};

class GridSpatialPartition
{
public:
	GridSpatialPartition();

	bool IsInEmptyCell(const class Actor* inMember);
	void AddCellMember(const class Actor* inMember);
	void UpdateCellMember(const class Actor* inMember, const Vector2D& newPosition);
	void RemoveCellMember(const class Actor* inMember);
	void ClearAllCells();

private:
	Cell& PositionToCell(const Vector2D& inPosition);

	Cell mGrid[NUM_CELL_ROW][NUM_CELL_COL];
};