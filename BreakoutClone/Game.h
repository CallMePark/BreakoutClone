#pragma once

#include "SDL.h"

#include <vector>
#include <string>
#include <unordered_map>

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;

struct InputState
{
	const Uint8* keyboardState;
	const Uint32 mouseState;
	const int mouseX, mouseY;
	const bool isMouseEnabled;

	InputState(const Uint8* inKeyboardState, const Uint32 inMouseState, const int inMouseX, 
		const int inMouseY, const bool inIsMouseEnabled)
		: keyboardState(inKeyboardState), mouseState(inMouseState), mouseX(inMouseX), 
		mouseY(inMouseY), isMouseEnabled(inIsMouseEnabled) 
	{}
};

class Game
{
public:
	Game();

	bool Initialize();
	void RunGameLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSpriteComponent(class SpriteComponent* spriteComponent);
	void RemoveSpriteComponent(class SpriteComponent* spriteComponent);

	SDL_Texture* GetTexture(const std::string& fileName);

private:
	// [START] Initialize
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	// [END] Initialize

	// [START] GAME LOOP
	void ProcessInput();
	void Update();
	void Render();
	bool mIsRunning;
	// [END] GAME LOOP

	void LoadData();
	float CalcDeltaTime();

	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	bool mUpdatingActors;

	std::vector<class SpriteComponent*> mSpriteComponents;
	std::unordered_map<std::string, SDL_Texture*> mTextures;
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

// Actor
// - virtual void OnCollision(Actor* actor, Vector) {}
// Component
// - virtual void OnPositionUpdate() {}
// CollisionComponent
// - bool CheckCollision
// - void OnPositionUpdate() override
//		if (CheckCollision())
//			CollisionInfo: OtherActor, ImpactPoint, ImpactNormal
//			mOwner->OnCollision(CollisionInfo);