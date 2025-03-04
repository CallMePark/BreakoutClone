#include "GridSpatialPartition.h"
#include "Actor.h"
#include "SDL_log.h"

GridSpatialPartition::GridSpatialPartition()
{
	// Create correctly-sized grid with empty cells
	for (int row = 0; row < NUM_CELL_ROW; ++row)
	{
		for (int col = 0; col < NUM_CELL_COL; ++col)
		{
			mGrid[row][col] = Cell{ row, col };
		}
	}
}

bool GridSpatialPartition::IsInEmptyCell(const Actor* inMember)
{
	Cell& cell = PositionToCell(inMember->GetPosition());

	return cell.members.empty();
}

void GridSpatialPartition::AddCellMember(const Actor* inMember)
{
	Cell& cell = PositionToCell(inMember->GetPosition());

	auto result = cell.members.insert(inMember);
	if (!result.second)
	{
		SDL_Log("AddCellMember failed. Duplicate found.");
	}
}

// Update cell placement every frame as actors move around the scene
void GridSpatialPartition::UpdateCellMember(const Actor* inMember, const Vector2D& newPosition)
{
	Cell& currCell = PositionToCell(inMember->GetPosition());
	Cell& newCell = PositionToCell(newPosition);

	if (currCell != newCell)
	{
		currCell.members.erase(inMember);
		newCell.members.insert(inMember);
	}
}

void GridSpatialPartition::RemoveCellMember(const Actor* inMember)
{
	Cell& cell = PositionToCell(inMember->GetPosition());

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

Cell& GridSpatialPartition::PositionToCell(const Vector2D& inPosition)
{
	int row = static_cast<int>(inPosition.y * INV_CELL_H);
	int col = static_cast<int>(inPosition.x * INV_CELL_W);
	return mGrid[row][col];
}