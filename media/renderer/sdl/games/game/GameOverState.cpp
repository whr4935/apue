#include "GameOverState.h"
#include "Game.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "MenuState.h"
#include "PlayState.h"
#include "AnimatedGraphic.h"


GameOverState::GameOverState()
{
}


GameOverState::~GameOverState()
{
}

std::string GameOverState::s_gameOverStateID = "GAMEOVER";

void GameOverState::render()
{
	for (auto &obj : m_gameObject) {
		obj->draw();
	}
}

void GameOverState::update()
{
	for (auto &obj : m_gameObject) {
		obj->update();
	}
}

bool GameOverState::onEnter()
{
	if (!TextureManager::Instance()->load("assets/gameover.png", "gameovertext",
										  Game::Instance()->getRenderer())) {
		return false;
	}

	if (!TextureManager::Instance()->load("assets/restart.png", "restartbutton",
										  Game::Instance()->getRenderer())) {
		return false;
	}

	if (!TextureManager::Instance()->load("assets/main.png", "mainbuton",
										  Game::Instance()->getRenderer())) {
		return false;
	}

	GameObject* gameOverText = new AnimatedGraphic(new LoadParams(200, 100, 190, 30, "gameovertext"), 2);
	GameObject* button1 = new MenuButton(new LoadParams(200, 200, 200, 80, "mainbuton"), s_gameOverToMain);
	GameObject* button2 = new MenuButton(new LoadParams(200, 300, 200, 80, "restartbutton"), s_restartPlay);
	m_gameObject.push_back(gameOverText);
	m_gameObject.push_back(button1);
	m_gameObject.push_back(button2);

	return true;
}

bool GameOverState::onExit()
{
	for (auto &obj : m_gameObject) {
		obj->clean();
	}

	TextureManager::Instance()->clearFromTextureMap("gameovertext");
	TextureManager::Instance()->clearFromTextureMap("mainbuton");
	TextureManager::Instance()->clearFromTextureMap("restartbutton");

	return true;
}

void GameOverState::s_gameOverToMain()
{
	Game::Instance()->getStateMachine()->changeState(new MenuState());
}

void GameOverState::s_restartPlay()
{
	Game::Instance()->getStateMachine()->changeState(new PlayState());
}
