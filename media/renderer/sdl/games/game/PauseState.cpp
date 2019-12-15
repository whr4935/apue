#include "PauseState.h"
#include "Game.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "MenuState.h"
#include "PlayState.h"


PauseState::PauseState()
{
}


PauseState::~PauseState()
{
}

std::string PauseState::s_pauseID = "PAUSE";

void PauseState::update()
{
	for (auto& obj : m_gameObjects) {
		obj->update();
	}
}

void PauseState::render()
{
	for (auto & obj : m_gameObjects) {
		obj->draw();
	}
}

bool PauseState::onEnter()
{
	if (!TextureManager::Instance()->load("assets/resume.png", "resumebutton", 
										  Game::Instance()->getRenderer())) {
		return false;
	}

	if (!TextureManager::Instance()->load("assets/main.png", "mainbutton",
										  Game::Instance()->getRenderer())) {
		return false;
	}

	GameObject* button1 = new MenuButton(new LoadParams(200, 100, 200, 80, "mainbutton"), s_pauseToMain);
	GameObject* button2 = new MenuButton(new LoadParams(200, 300, 200, 80, "resumebutton"), s_resumePlay);

	m_gameObjects.push_back(button1);
	m_gameObjects.push_back(button2);

	return true;
}

bool PauseState::onExit()
{
	for (auto &obj : m_gameObjects) {
		obj->clean();
	}
//	m_gameObjects.clear();
	TextureManager::Instance()->clearFromTextureMap("mainbutton");
	TextureManager::Instance()->clearFromTextureMap("resumebutton");

	InputHandler::Instance()->reset();

	return true;
}

void PauseState::s_pauseToMain()
{
	Game::Instance()->getStateMachine()->changeState(new MenuState());
}

void PauseState::s_resumePlay()
{
	Game::Instance()->getStateMachine()->popState();
}
