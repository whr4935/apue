#include "MenuState.h"
#include "TextureManager.h"
#include "Game.h"
#include "PlayState.h"

MenuState::MenuState()
{
}


MenuState::~MenuState()
{
}

const std::string MenuState::s_menuID = "MENU";

void MenuState::update()
{
	for (auto &obj : m_gameObjects) {
		obj->update();
	}
}

void MenuState::render()
{
	for (auto &obj : m_gameObjects) {
		obj->draw();
	}
}

bool MenuState::onEnter()
{
	if (!TextureManager::Instance()->load("assets/button.png", "playbutton", 
		Game::Instance()->getRenderer())) {
		return false;
	}

	if (!TextureManager::Instance()->load("assets/exit.png", "exitbutton",
										 Game::Instance()->getRenderer())) {
		return false;
	}

	GameObject* button1 = new MenuButton(new LoadParams(100, 100, 400, 100, "playbutton"), s_menuToPlay);
	GameObject* button2 = new MenuButton(new LoadParams(100, 300, 400, 100, "exitbutton"), s_exitFromMenu);

	m_gameObjects.push_back(button1);
	m_gameObjects.push_back(button2);

	return true;
}

bool MenuState::onExit()
{
	for (auto &obj : m_gameObjects) {
		obj->clean();
	//	delete obj;
	}

//	m_gameObjects.clear();

	TextureManager::Instance()->clearFromTextureMap("playbutton");
	TextureManager::Instance()->clearFromTextureMap("exitbutton");

	return true;
}

void MenuState::s_menuToPlay()
{
	//std::cout << "Play button clicked" << std::endl;
	Game::Instance()->getStateMachine()->changeState(new PlayState());
}

void MenuState::s_exitFromMenu()
{
	//std::cout << "Exit button clicked" << std::endl;
	Game::Instance()->quit();
}
