#pragma once

#include <string>
#include <vector>

class LevelLoader
{
public:
	LevelLoader(class Game* game);

	bool LoadLevel(const std::string& fileName, class Actor* actor);

private:
	bool LoadLevelFile(const std::string& fileName);
	bool LoadActors(class Actor* actor);

	class Game* mGame;

	std::vector<std::vector<int>> mCSVData;
};