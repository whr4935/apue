#include <SDL2/SDL_main.h>
#include "Game.h"

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")

const int FPS = 10;
const int DELAY_TIME = 1000.0f / FPS;

int main(int argc, char **argv)
{
	unsigned int frameStart, frameTime;

	Game::Instance()->init("SDL Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, false);

	while (Game::Instance()->running()) {
		frameStart = SDL_GetTicks();

		Game::Instance()->handleEvents();
		Game::Instance()->update();
		Game::Instance()->render();

		frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < DELAY_TIME) {
			SDL_Delay(DELAY_TIME - frameTime);
		}
	}
	Game::Instance()->clean();

	return 0;
}
