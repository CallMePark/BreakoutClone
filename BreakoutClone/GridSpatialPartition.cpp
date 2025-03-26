#include "GridSpatialPartition.h"
#include "Actor.h"
#include "SDL.h"

#include <algorithm>

GridSpatialPartition::GridSpatialPartition()
	: mGridSize(0)
{
	// Create correctly-sized grid with empty cells
	for (int row = 0; row < NUM_CELL_ROW; ++row)
	{
		for (int col = 0; col < NUM_CELL_COL; ++col)
		{
			mGrid[row][col] = Cell{ row, col, nullptr };
			++mGridSize;
		}
	}
}

// Add based on center since, Block Size = Cell Size
void GridSpatialPartition::AssignCellMember(Actor* inMember)
{
	Cell& cell = PositionToCell(inMember->GetPosition());

	cell.member = inMember;
}

// Remove based on center since, Block Size = Cell Size
void GridSpatialPartition::RemoveCellMember(Actor* inMember)
{
	Cell& cell = PositionToCell(inMember->GetPosition());

	cell.member = nullptr;
}

void GridSpatialPartition::ClearAllCells()
{
	for (int row = 0; row < NUM_CELL_ROW; ++row)
	{
		for (int col = 0; col < NUM_CELL_COL; ++col)
		{
			mGrid[row][col].member = nullptr;
		}
	}
}

// For dynamic objects to check against cell members
bool GridSpatialPartition::IsInEmptyCell(Actor* inMember, std::vector<Cell>& outOccupiedCells)
{
	bool isInEmptyCell = true;

	std::vector<Cell> cells = GetOverlappingCells(inMember);
	for (const Cell& cell : cells)
	{
		if (cell.member)
		{
			outOccupiedCells.push_back(cell);
			isInEmptyCell = false;
		}
	}

	return isInEmptyCell;
}

std::vector<Cell> GridSpatialPartition::GetOverlappingCells(const Actor* inMember)
{
	const AABB& box = inMember->GetCollisionComponent()->GetWorldAABB();
	std::vector<Cell> cells;
	AABBToCells(box, cells);

	return cells;
}

Cell& GridSpatialPartition::PositionToCell(const Vector2D& inPosition)
{
	int row = static_cast<int>(inPosition.y * INV_CELL_H);
	int col = static_cast<int>(inPosition.x * INV_CELL_W);

	row = std::clamp(row, 0, NUM_CELL_ROW - 1);
	col = std::clamp(col, 0, NUM_CELL_COL - 1);

	Cell& cell = mGrid[row][col];
	return cell;
}

// Return all cells occupied within the AABB
void GridSpatialPartition::AABBToCells(const AABB& inAABB, std::vector<Cell>& outCells)
{
	int minRow = static_cast<int>(inAABB.min.y * INV_CELL_H);
	int maxRow = static_cast<int>(inAABB.max.y * INV_CELL_H);
	int minCol = static_cast<int>(inAABB.min.x * INV_CELL_W);
	int maxCol = static_cast<int>(inAABB.max.x * INV_CELL_W);

	// Check bounds
	minRow = std::max(minRow, 0);
	maxRow = std::min(maxRow, NUM_CELL_ROW - 1);
	minCol = std::max(minCol, 0);
	maxCol = std::min(maxCol, NUM_CELL_COL - 1);

	for (int row = minRow; row <= maxRow; ++row)
	{
		for (int col = minCol; col <= maxCol; ++col)
		{
			auto iter = std::find(outCells.begin(), outCells.end(), mGrid[row][col]);
			if (iter == outCells.end())
			{
				outCells.push_back(mGrid[row][col]);
			}
		}
	}
}

void GridSpatialPartition::RenderGridDebug(SDL_Renderer* renderer)
{
	std::vector<Cell> occupiedCells;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (int row = 0; row < NUM_CELL_ROW; ++row)
	{
		for (int col = 0; col < NUM_CELL_COL; ++col)
		{
			Cell& cell = mGrid[row][col];

			if (cell.member)
			{
				occupiedCells.push_back(cell);
				continue;
			}

			SDL_Rect debugBox = {
				static_cast<int>(cell.box.min.x),
				static_cast<int>(cell.box.min.y),
				static_cast<int>(CELL_W),
				static_cast<int>(CELL_H)
			};
			SDL_RenderDrawRect(renderer, &debugBox);
		}
	}

	// Highlight occupied cells RED
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	for (const Cell& cell : occupiedCells)
	{
		int thickness = 3;
		for (int i = 0; i < thickness; ++i)
		{
			SDL_Rect thickDebugBox = {
				static_cast<int>(cell.box.min.x) - i,
				static_cast<int>(cell.box.min.y) - i,
				static_cast<int>(CELL_W) + (2 * i),
				static_cast<int>(CELL_H) + (2 * i)
			};
			SDL_RenderDrawRect(renderer, &thickDebugBox);
		}
	}
}