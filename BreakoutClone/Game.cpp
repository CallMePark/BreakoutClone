#include "Game.h"
#include "SDL_image.h"

Game::Game()
	: mWindow(nullptr), 
	mRenderer(nullptr), 
	mTicksCount(0), 
	mIsRunning(true), 
	mUpdatingActors(false) {}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
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
		SDL_WINDOW_RESIZABLE);
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

	mTicksCount = SDL_GetTicks();

	return true;
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
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}
}

void Game::Update()
{
	SDL_Log("%d", SDL_GetTicks());
}

void Game::Render()
{
}

void Game::Shutdown()
{
}

void Game::AddActor(Actor* actor)
{
}

void Game::RemoveActor(Actor* actor)
{
}

