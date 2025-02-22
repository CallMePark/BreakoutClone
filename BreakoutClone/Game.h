#pragma once

#include "SDL.h"

#include <vector>

class Game
{
public:
	static constexpr int WINDOW_WIDTH = 1024;
	static constexpr int WINDOW_HEIGHT = 768;

	Game();

	bool Initialize();
	void RunGameLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

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

	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	bool mUpdatingActors;
};