#include "Player.h"
#include "InputHandler.h"


Player::Player(const LoadParams *pParams) : SDLGameObject(pParams)
{
}


Player::~Player()
{
}

void Player::draw()
{
	SDLGameObject::draw();
}

void Player::update()
{
	//m_velocity.setX(1);

// 	if (InputHandler::Instance()->getMousButtonState(InputHandler::LEFT)) {
// 		m_velocity.setX(m_velocity.getX() + 1);
// 	} else if (InputHandler::Instance()->getMousButtonState(InputHandler::RIGHT)) {
// 		m_velocity.setX(m_velocity.getX() - 1);
// 	}
// 
// 	Vector2D *vec = InputHandler::Instance()->getMousePosition();
// 	*vec -= Vector2D(125 / 2, 150 / 2);
// 	m_velocity = (*vec - m_position) / 1;

// 	if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT)) {
// 		m_velocity.setX(2);
// 	}
// 	if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT)) {
// 		m_velocity.setX(-2);
// 	}
// 	if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_DOWN)) {
// 		m_velocity.setY(2);
// 	}
// 	if (InputHandler::Instance()->isKeyDown(SDL_SCANCODE_UP)) {
// 		m_velocity.setY(-2);
// 	}

	handleInput();

 	m_currentFrame = (m_currentFrame + 1) % 5;
// 	m_currentRow = m_currentFrame / 4 + 1;
	SDLGameObject::update();
}

void Player::clean()
{
	SDLGameObject::clean();
}

void Player::handleInput()
{
	Vector2D* target = InputHandler::Instance()->getMousePosition();

	m_velocity = *target - m_position;
	m_velocity /= 10;
}
