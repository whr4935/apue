#include "Game.h"
#include "TextureManager.h"
#include "InputHandler.h"

//#include <windows.h>

#include "Player.h"
#include "Enemy.h"
#include "MenuState.h"

#include <cstdlib>
#include <ctime>

Game::Game() : m_bRunning(false), m_pWindow(nullptr), m_pRender(nullptr), 
			m_pGameStateMachine(new GameStateMachine())
{
}


Game::~Game()
{
}

Game* Game::s_pInstance = nullptr;

bool Game::running()
{
	return m_bRunning;
}

Game* Game::Instance()
{
	if (s_pInstance == 0) {
		s_pInstance = new Game();
	}

	return s_pInstance;
}

int Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullScreen)
{
#ifdef _DEBUG
	AllocConsole();
	freopen("CON", "w", stdout);
#endif

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		return -1;
	}

	int flags = 0;
	if (fullScreen)
		flags = SDL_WINDOW_FULLSCREEN;

	m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
	if (m_pWindow == nullptr)
		return -1;

	m_pRender = SDL_CreateRenderer(m_pWindow, -1, 0);
	if (m_pRender == nullptr) {
		SDL_DestroyWindow(m_pWindow);
		return -1;
	}

	SDL_SetRenderDrawColor(m_pRender, 0, 0, 0, 255);
	SDL_RenderClear(m_pRender);

	m_bRunning = true;


	InputHandler::Instance()->initialiseJoysticks();

	m_pGameStateMachine->changeState(new MenuState());

	std::srand(std::time(nullptr));

	return 0;
}

void Game::handleEvents()
{
	InputHandler::Instance()->update();
}

int Game::update()
{
	m_pGameStateMachine->update();

	return 0;
}

int Game::render()
{
	SDL_RenderClear(m_pRender);

	m_pGameStateMachine->render();

	SDL_RenderPresent(m_pRender);

	return 0;
}

void Game::clean()
{
	delete m_pGameStateMachine;

	InputHandler::Instance()->clean();

	SDL_DestroyRenderer(m_pRender);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();

#ifdef _DEBUG
	FreeConsole();
#endif
}
