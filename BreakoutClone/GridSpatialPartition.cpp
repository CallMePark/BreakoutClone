#include "GridSpatialPartition.h"
#include "CollisionComponent.h"
#include "SDL.h"

GridSpatialPartition::GridSpatialPartition()
	: mGridSize(0)
{
	// Create correctly-sized grid with empty cells
	for (int row = 0; row < NUM_CELL_ROW; ++row)
	{
		for (int col = 0; col < NUM_CELL_COL; ++col)
		{
			mGrid[row][col] = Cell{ row, col, Vector2D(col * CELL_W, row * CELL_H)};
			++mGridSize;
		}
	}
}

// Add based on center since, Block Size = Cell Size
void GridSpatialPartition::AddCellMember(const CollisionComponent* inMember)
{
	const AABB& box = inMember->GetWorldAABB();
	Cell& cell = PositionToCell(box.GetCenter());

	cell.members.insert(inMember);
}

// Remove based on center since, Block Size = Cell Size
void GridSpatialPartition::RemoveCellMember(const CollisionComponent* inMember)
{
	const AABB& box = inMember->GetWorldAABB();
	Cell& cell = PositionToCell(box.GetCenter());

	auto iter = cell.members.find(inMember);
	if (iter != cell.members.end())
	{
		cell.members.erase(iter);
	}
}

void GridSpatialPartition::ClearAllCells()
{
	for (int row = 0; row < NUM_CELL_ROW; ++row)
	{
		for (int col = 0; col < NUM_CELL_COL; ++col)
		{
			mGrid[row][col].members.clear();
		}
	}
}

const std::unordered_set<const CollisionComponent*>& GridSpatialPartition::GetCellMembers(const CollisionComponent* inMember)
{
	const AABB& box = inMember->GetWorldAABB();
	Cell& cell = PositionToCell(box.GetCenter());

	return cell.members;
}

// For dynamic objects to check against cell members
bool GridSpatialPartition::IsInEmptyCell(const CollisionComponent* inMember)
{
	bool isInEmptyCell = true;

	const AABB& box = inMember->GetWorldAABB();
	std::vector<Cell> cells;
	AABBToCells(box, cells);

	for (const Cell& cell : cells)
	{
		if (!cell.members.empty())
		{
			isInEmptyCell = false;
		}
	}

	return isInEmptyCell;
}

Cell& GridSpatialPartition::PositionToCell(const Vector2D& inPosition)
{
	int row = static_cast<int>(inPosition.y * INV_CELL_H);
	int col = static_cast<int>(inPosition.x * INV_CELL_W);
	return mGrid[row][col];
}

// Return all cells occupied within the AABB
void GridSpatialPartition::AABBToCells(const AABB& inAABB, std::vector<Cell>& cells)
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
			auto iter = std::find(cells.begin(), cells.end(), mGrid[row][col]);
			if (iter == cells.end())
			{
				cells.push_back(mGrid[row][col]);
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

			if (!cell.members.empty())
			{
				occupiedCells.push_back(cell);
				continue;
			}

			SDL_Rect debugBox = {
				static_cast<int>(cell.cellPosition.x),
				static_cast<int>(cell.cellPosition.y),
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
				static_cast<int>(cell.cellPosition.x) - i,
				static_cast<int>(cell.cellPosition.y) - i,
				static_cast<int>(CELL_W) + (2 * i),
				static_cast<int>(CELL_H) + (2 * i)
			};
			SDL_RenderDrawRect(renderer, &thickDebugBox);
		}
	}
}