#include "PlayState.h"
#include "PauseState.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "GameOverState.h"

#include <cstdlib>

PlayState::PlayState()
{
}


PlayState::~PlayState()
{
}

std::string const PlayState::s_playID = "PLAY";

void PlayState::render()
{
	for (auto& obj : m_gameObjects) {
		obj->draw();
	}
}

void PlayState::update()
{
	if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE)) {
		Game::Instance()->getStateMachine()->pushState(new PauseState());
	}

	for (auto& obj : m_gameObjects) {
		obj->update();
	}

	if (checkCollision(dynamic_cast<SDLGameObject*>(m_gameObjects[0]), dynamic_cast<SDLGameObject*>(m_gameObjects[1]))) {
		Game::Instance()->getStateMachine()->pushState(new GameOverState());
	}
}

bool PlayState::onEnter()
{
	if (!TextureManager::Instance()->load("assets/helicopter.png", "helicopter",
		Game::Instance()->getRenderer())) {
		return false;
	}

	if (!TextureManager::Instance()->load("assets/helicopter2.png", "helicopter2",
		Game::Instance()->getRenderer())) {
		return false;
	}

	int width = 128;
	int height = 55;
	int x = std::rand() % (SCREEN_WIDTH - width);
	int y = std::rand() % (SCREEN_HEIGHT - height);

	GameObject* player = new Player(new LoadParams(500, 100, 128, 55, "helicopter"));
	GameObject* enemy = new Enemy(new LoadParams(x, y, width, height, "helicopter2"));
	m_gameObjects.push_back(player);
	m_gameObjects.push_back(enemy);

	return true;
}

bool PlayState::onExit()
{
	for (auto &obj : m_gameObjects) {
		obj->clean();
	}
//	m_gameObjects.clear();

	TextureManager::Instance()->clearFromTextureMap("helicopter");
	TextureManager::Instance()->clearFromTextureMap("helicopter2");

	return true;
}

bool PlayState::checkCollision(SDLGameObject * p1, SDLGameObject * p2)
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = p1->getPosition().getX();
	rightA = p1->getPosition().getX() + p1->getWidth();
	topA = p1->getPosition().getY();
	bottomA = p1->getPosition().getY() + p1->getHeight();

	leftB = p2->getPosition().getX();
	rightB = p2->getPosition().getX() + p2->getWidth();
	topB = p2->getPosition().getY();
	bottomB = p2->getPosition().getY() + p2->getHeight();

	if (bottomA <= topB) return false;
	if (topA >= bottomB) return false;
	if (rightA <= leftB) return false;
	if (leftA >= rightB) return false;

	return true;
}

