#pragma once
class LevelLoader
{
};

// [START] Level Loader
//#include "GridSpatialPartition.h"
//
//#include <fstream>
//#include <sstream>
//#include <iostream>
//#include <string>
//
//class LevelLoader
//{
//public:
//	LevelLoader() { }
//
//	bool Initialize()
//	{
//		// [START] Read CSV
//		std::string fileName = "Assets/BlockGrid_Level1.csv";
//		std::ifstream inFile{ fileName };
//		if (!inFile)
//		{
//			SDL_Log("Failed to open block grid file: %s", fileName.c_str());
//			return false;
//		}
//
//		std::vector<std::vector<int>> csvData;
//		std::string line;
//		while (std::getline(inFile, line))
//		{
//			std::vector<int> row;
//			std::stringstream ss{ line };
//			std::string value;
//
//			while (std::getline(ss, value, ','))
//			{
//				try
//				{
//					 row.push_back(std::stoi(value));
//				}
//				catch (const std::invalid_argument& ex)
//				{
//					SDL_Log("%s in csv file: %s", ex.what(), fileName.c_str());
//					return false;
//				}
//			}
//
//			if (!row.empty())
//			{
//				csvData.push_back(row);
//			}
//
//			if (row.size() > NUM_CELL_COL)
//			{
//				SDL_Log("Column size %zu exceeds the required size %d in %s", row.size(), NUM_CELL_COL, fileName.c_str());
//				return false;
//			}
//		}
//		if (csvData.size() > NUM_CELL_ROW)
//		{
//			SDL_Log("Row size %zu exceeds the required size %d in %s", csvData.size(), NUM_CELL_ROW, fileName.c_str());
//			return false;
//		}
//
//		for (const auto& row : csvData)
//		{
//			for (const auto& cell : row)
//			{
//				std::cout << cell << " ";
//			}
//			std::cout << std::endl;
//		}
//		// [END] Read CSV
//
//		for (int row = 0; row < csvData.size(); row++)
//		{
//			for (int col = 0; col < csvData[row].size(); col++)
//			{
//
//			}
//		}
//
//		return true;
//	}
//};
// [END] Level Loader
