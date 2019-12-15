#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <res_path.h>
#include <cleanup.h>
#include <Util.h>


#pragma comment(lib, "sdl2main.lib")
#pragma comment(lib, "sdl2.lib")
#pragma comment(lib, "sdl2_image.lib")
#pragma comment(lib, "sdl2_ttf.lib")


int main(int argc, char *argv[])
{
	if (getenv("PATH") != NULL)
		std::cout << "debug" << std::endl;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
	}

	if ((IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG) != IMG_INIT_PNG) {
		logSDLError(std::cout, "IMG_Init");
		return -1;
	}
	if (TTF_Init() != 0) {
		logSDLError(std::cout, "TTF_Init");
		return -1;
	}

	atexit(SDL_Quit);
	atexit(IMG_Quit);
	atexit(TTF_Quit);

	SDL_Window *window = SDL_CreateWindow("SDL", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		logSDLError(std::cout, "CreateWindow");
		return -1;
	}

	SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (render == nullptr) {
		logSDLError(std::cout, "CreateRenderer");
		cleanup(window);
		return -1;
	}

	const std::string resPath = getResourcePath();

	SDL_Color color{ 255, 255, 255, 255 };
	SDL_Texture * image = renderText("TTF fonts are cool!", resPath + "sample.ttf", color, 64, render);
	if (image == nullptr) {
		cleanup(render, window);
		return -1;
	}

	int iW, iH;
	SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;

	bool quit = false;
	SDL_Event e;
	while (!quit) {
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
		}
		SDL_Delay(40);

		SDL_RenderClear(render);
		renderTexture(image, render, x, y);
		SDL_RenderPresent(render);
	}
	cleanup(image, render, window);


// 	SDL_Texture *image = loadTexture(resPath + "image_3.png", render);
// 	if (image == nullptr) {
// 		cleanup(render, window);
// 		return -1;
// 	}
// 
// 	int iW, iH;
// 	iW	=
// 	iH	= 100;
// 	int x = SCREEN_WIDTH / 2 - iW / 2;
// 	int y = SCREEN_HEIGHT / 2 - iH / 2;
// 
// 	SDL_Rect clips[4];
// 	for (int i = 0; i < 4; ++i) {
// 		clips[i].x = i / 2 * iW;
// 		clips[i].y = i % 2 * iH;
// 		clips[i].w = iW;
// 		clips[i].h = iH;
// 	}
// 	int useClip = 0;
// 
// 	bool quit = false;
// 	SDL_Event e;
// 	while (!quit) {
// 		SDL_PollEvent(&e);
// 		if (e.type == SDL_QUIT)
// 			quit = true;
// 
// 		if (e.type == SDL_KEYDOWN) {
// 			switch (e.key.keysym.sym) {
// 			case SDLK_1:
// 				useClip = 0;
// 				break;
// 
// 			case SDLK_2:
// 				useClip = 1;
// 				break;
// 
// 			case SDLK_3:
// 				useClip = 2;
// 				break;
// 
// 			case SDLK_4:
// 				useClip = 3;
// 				break;
// 
// 			default:
//				break;
// 			}
// 		}
// 
// 		SDL_RenderClear(render);
// 		renderTexture(image, render, x, y, clips + useClip);
// 		SDL_RenderPresent(render);
// 
// 		SDL_Delay(40);
// 	}
// 
// 	cleanup(image, render, window);


// 	SDL_Texture *background = loadTexture(resPath + "background.png", render);
// 	SDL_Texture *image = loadTexture(resPath + "image.png", render);
// 	if (background == nullptr || image == nullptr) {
// 		cleanup(background, image, render, window);
// 		return -1;
// 	}
// 
// 	SDL_RenderClear(render);
// 
// 	int tiles_per_row = SCREEN_WIDTH / TILE_SIZE;
// 	int tiles_per_col = SCREEN_HEIGHT / TILE_SIZE;
// 	for (int i = 0; i < tiles_per_row * tiles_per_col; ++i) {
// 		renderTexture(background, render, i%tiles_per_row * TILE_SIZE, i / tiles_per_row * TILE_SIZE);
// 	}
// 
// 	int iW, iH;
// 	SDL_QueryTexture(image, nullptr, nullptr, &iW, &iH);
// 	renderTexture(image, render, SCREEN_WIDTH / 2 - iW / 2, SCREEN_HEIGHT / 2 - iH / 2);
// 
// 	SDL_RenderPresent(render);
// 	
// 	SDL_Event e;
// 	bool quit = false;
// 	while (!quit) {
// 		while (SDL_PollEvent(&e)) {
// 			if (e.type == SDL_QUIT) {
// 				quit = true;
// 			}
// 			if (e.type == SDL_KEYDOWN) {
// 				quit = true;
// 			}
// 			if (e.type == SDL_MOUSEBUTTONDOWN) {
// 				quit = true;
// 			}
// 		}
// 		SDL_Delay(50);
// 	}
// 	std::cout << "\nExit..." << std::endl;
// 
// 	cleanup(background, image, render, window);


// 	SDL_Texture* background = loadTexture(resPath + "background.bmp", render);
// 	SDL_Texture *image = loadTexture(resPath + "image.bmp", render);
// 	if (background == nullptr || image == nullptr) {
// 		cleanup(background, image, render, window);
// 		return -1;
// 	}
// 
// 	SDL_RenderClear(render);
// 	int bW, bH;
// 	SDL_QueryTexture(background, nullptr, nullptr, &bW, &bH);
// 	renderTexture(background, render, 0, 0);
// 	renderTexture(background, render, bW, 0);
// 	renderTexture(background, render, 0, bH);
// 	renderTexture(background, render, bW, bH);
// 
// 	int iW, iH;
// 	SDL_QueryTexture(image, nullptr, nullptr, &iW, &iH);
// 	int x = SCREEN_WIDTH / 2 - iW / 2;
// 	int y = SCREEN_HEIGHT / 2 - iH / 2;
// 	renderTexture(image, render, x, y);
// 
// 	SDL_RenderPresent(render);
// 	SDL_Delay(2000);
// 
// 	cleanup(image, background, render, window);

	return 0;
}