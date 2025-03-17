#include "LevelLoader.h"
#include "GridSpatialPartition.h"
#include "Game.h"
#include "ActorBlock.h"
#include "SDL.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

LevelLoader::LevelLoader(Game* game)
	: mGame(game)
{}

// Game manages actor
bool LevelLoader::LoadLevel(const std::string& fileName, Actor* actor)
{
	if (!LoadLevelFile(fileName))
	{
		SDL_Log("Failed to load level file: %s", fileName.c_str());
		return false;
	}

	LoadActors(actor);

	return true;
}

bool LevelLoader::LoadLevelFile(const std::string& fileName)
{
	std::ifstream inFile{ fileName };
	if (!inFile)
	{
		SDL_Log("Failed to open block grid file: %s", fileName.c_str());
		return false;
	}

	std::string line;
	while (std::getline(inFile, line))
	{
		std::vector<int> row;
		std::stringstream ss{ line };
		std::string value;

		// Add row data (columns)
		while (std::getline(ss, value, ','))
		{
			try
			{
				row.push_back(std::stoi(value));
			}
			catch (const std::invalid_argument& ex) // Thrown by stoi when string is not an integer
			{
				SDL_Log("%s in csv file: %s", ex.what(), fileName.c_str());
				return false;
			}
		}

		// Add row to csvData
		if (!row.empty())
		{
			mCSVData.push_back(row);
		}

		// Ensure CSV column size matchs the GSP grid column size
		if (row.size() > NUM_CELL_COL)
		{
			SDL_Log("Column size %zu exceeds the required size %d in %s", row.size(), NUM_CELL_COL, fileName.c_str());
			return false;
		}
	}
	// Ensure CSV row size matchs the GSP grid row size
	if (mCSVData.size() > NUM_CELL_ROW)
	{
		SDL_Log("Row size %zu exceeds the required size %d in %s", mCSVData.size(), NUM_CELL_ROW, fileName.c_str());
		return false;
	}

	return true;
}

bool LevelLoader::LoadActors(Actor* actor)
{
	for (int row = 0; row < mCSVData.size(); ++row)
	{
		for (int col = 0; col < mCSVData[0].size(); ++col)
		{
			if (mCSVData[row][col] == 1)
			{
				actor = new ActorBlock(mGame);
				float posGridCol = CELL_W * 0.5f + (CELL_W * col);
				float posGridRow = CELL_H * 0.5f + (CELL_H * row);
				actor->SetPosition(Vector2D(posGridCol, posGridRow)); // Center of cell
			}
		}
	}
	return true;
}

