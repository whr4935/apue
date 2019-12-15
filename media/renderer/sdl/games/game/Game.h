#pragma once
#ifndef _GAME_H
#define _GMAE_H

#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>

#include "GameStateMachine.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Game
{
public:
	Game();
	~Game();

	int init(const char* title, int xpos, int ypos, int width, int height, bool fullScreen);
	
	int render();
	int update();
	void handleEvents();
	void clean();

	bool running();
	void quit() { m_bRunning = false; }

	static Game* Instance();
	SDL_Renderer* getRenderer() const { return m_pRender; }
	GameStateMachine* getStateMachine() { return m_pGameStateMachine; }

private:
	SDL_Window *m_pWindow;
	SDL_Renderer *m_pRender;
	bool m_bRunning;

	static Game* s_pInstance;

	GameStateMachine* m_pGameStateMachine;
};


#endif



