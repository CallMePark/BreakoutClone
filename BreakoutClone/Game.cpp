#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "ActorBackground.h"
#include "ActorPlayer.h"
#include "ActorBall.h"
#include "GridSpatialPartition.h"
#include "LevelLoader.h"
#include "SDL_image.h"

#include <algorithm>

Game::Game()
	: mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true), mUpdatingActors(false)
{
	mGSP = new GridSpatialPartition();
	mLevelLoader = new LevelLoader(this);
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("SDL library failed to initialize: %s", SDL_GetError());
		return false;
	}
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("SDL_Image failed to initialize: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"Breakout Clone",
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, 
		WINDOW_HEIGHT, 
		0
	);
	if (!mWindow)
	{
		SDL_Log("Failed to create SDL window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!mRenderer)
	{
		SDL_Log("Failed to create SDL renderer: %s", SDL_GetError());
		return false;
	}

	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	mTicksCount = SDL_GetTicks();

	LoadData();

	return true;
}

void Game::LoadData()
{
	Actor* a = new ActorBackground(this);
	a = new ActorPlayer(this);
	a = new ActorBall(this);

	std::string levelFileName = "Assets/BlockGrid_Level1.csv";
	if (!mLevelLoader->LoadLevel(levelFileName, a))
	{
		mIsRunning = false;
	}
}

void Game::RunGameLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		Update();
		Render();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: // End program at user-requested quit
			mIsRunning = false;
			break;
		case SDL_KEYDOWN: // Disable mouse motion on key press
			SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
			break;
		case SDL_MOUSEBUTTONDOWN: // Enable mouse motiohn on mouse button press
			SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
		}
	}

	// Build input state
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	int mouseX, mouseY;

	const Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

	const Uint8 mouseMotionState = SDL_EventState(SDL_MOUSEMOTION, SDL_QUERY);
	bool isMouseEnabled = (mouseMotionState == SDL_ENABLE) ? true : false;

	InputState inputState{ keyState, mouseState, mouseX, mouseY, isMouseEnabled };

	// End game if ESC key or middle mouse button is pressed
	if (keyState[SDL_SCANCODE_ESCAPE] || mouseState == SDL_BUTTON_MIDDLE)
	{
		mIsRunning = false;
	}

	// Pass the InputState to all actors
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(inputState);
	}
	mUpdatingActors = false;
}

void Game::Update()
{
	float deltaTime = CalcDeltaTime();
	/*SDL_Log("Current FPS: %.0f", (1 / deltaTime));*/

	// 1. Update all actors in mActors
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// 2. Move all new actor pointers during mUpdatingActors(true) to mActors
	for (auto pendingActor : mPendingActors)
	{
		mActors.emplace_back(pendingActor);
	}

	// 1. Move all dead actor pointers from mActors to deadActors
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.push_back(actor);
		}
	}

	// 2. Delete the dead actor objects
	for (auto deadActor : deadActors)
	{
		delete deadActor;
	}
}

float Game::CalcDeltaTime()
{
	// Limit frame rate to approx. 62.5 FPS (60 would require float type)
	Uint32 capTicksCount = mTicksCount + 16;

	// If FPS is higher than 62.5FPS, pause execution until it gets lower
	if (SDL_GetTicks() < capTicksCount)
	{
		SDL_Delay(capTicksCount - SDL_GetTicks());
	}
	// In case FPS < 20, set deltaTime to 0.05 (~20FPS) to prevent large single update
	float deltaTime = std::min(((SDL_GetTicks() - mTicksCount) / 1000.0f), 0.05f);

	// Update for next deltaTime calculation
	mTicksCount = SDL_GetTicks();

	return deltaTime;
}

// Double buffering. Initial front buffer is the default SDL screen
void Game::Render()
{
	// 1. Clears the back buffer with the color specified in SetRednerDrawColor
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_RenderClear(mRenderer);

	// 2. Draw to back buffer using functions such as SDL_RenderCopyEx
	for (auto spriteComponent : mSpriteComponents)
	{
		spriteComponent->Draw(mRenderer);
	}

	// DEBUG RENDERS
	mGSP->RenderGridDebug(mRenderer);
	for (auto actor : mActors)
	{
		actor->RenderActorDebug(mRenderer);
	}
	// DEBUG RENDERS

	// 3. Swap the front and back buffer to render new content
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	// Actor::~Actor removes(pops) it from mActors
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy all textures
	for (auto texture : mTextures)
	{
		SDL_DestroyTexture(texture.second);
	}
	mTextures.clear();

	// Destroy GridSpatialPartition object
	delete mGSP;

	// Must be destroyed in reverse of their initialization order
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	IMG_Quit();
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// Memory of removed pending actors are managed in mActors (see Game::Update)
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Memory of removed actors are managed in deadActors (see Game::Update)
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSpriteComponent(SpriteComponent* spriteComponent)
{
	// Insert SpriteComponent based on drawOrder (lower order renders first = back)
	int newUpdateOrder = spriteComponent->GetDrawOrder();
	auto iter = mSpriteComponents.begin();
	for (; iter != mSpriteComponents.end(); ++iter)
	{
		if (newUpdateOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}
	mSpriteComponents.insert(iter, spriteComponent);
}

void Game::RemoveSpriteComponent(SpriteComponent* spriteComponent)
{
	// Memory is managed by owner (Actor), so just erase from vector
	auto iter = std::find(mSpriteComponents.begin(), mSpriteComponents.end(), spriteComponent);
	if (iter != mSpriteComponents.end())
	{
		mSpriteComponents.erase(iter);
	}
}

// Implemented in Game class to prevent loading duplicate image files
SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* texture = nullptr;

	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		texture = iter->second;
	}
	else
	{
		// Intermediary to load image to CPU before converting it to SDL_Texture(GPU)
		SDL_Surface* surface = IMG_Load(fileName.c_str());
		if (!surface)
		{
			SDL_Log("Failed to load image from path: %s", fileName.c_str());
			return nullptr;
		}

		// Convert image surface to SDL_Texture
		texture = SDL_CreateTextureFromSurface(mRenderer, surface);
		// Free SDL_Surface dynamically allocated by IMG_Load
		SDL_FreeSurface(surface); 
		if (!texture)
		{
			SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), texture);
	}

	return texture;
}