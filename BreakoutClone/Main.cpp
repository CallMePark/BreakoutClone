#include "Game.h"

int main(int argc, char* argv[])
{
	Game game;
	bool isInitSuccess = game.Initialize();
	if (isInitSuccess)
	{
		game.RunGameLoop();
	}
	game.Shutdown();
	return 0;
}