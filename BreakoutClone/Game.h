#pragma once

#include "SDL.h"

#include <vector>
#include <string>
#include <unordered_map>

struct InputState
{
	const Uint8* keyboardState;
	const Uint32 mouseState;
	const int mouseX, mouseY;
	const bool isMouseEnabled;

	InputState(const Uint8* inKeyboardState, const Uint32 inMouseState, const int inMouseX, 
		const int inMouseY, const bool inIsMouseEnabled)
		: keyboardState(inKeyboardState), mouseState(inMouseState), mouseX(inMouseX), 
		mouseY(inMouseY), isMouseEnabled(inIsMouseEnabled) { }
};

static constexpr int WINDOW_WIDTH = 1024;
static constexpr int WINDOW_HEIGHT = 768;

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

	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	bool mUpdatingActors;

	std::vector<class SpriteComponent*> mSpriteComponents;
	std::unordered_map<std::string, SDL_Texture*> mTextures;
};