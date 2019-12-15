#include "Enemy.h"
#include "Game.h"


Enemy::Enemy(const LoadParams* pParams) : SDLGameObject(pParams)
{
	m_velocity.setY(2);
	m_velocity.setX(4);
}


Enemy::~Enemy()
{
}

void Enemy::draw()
{
	SDLGameObject::draw();
}

void Enemy::update()
{
	m_currentFrame = (m_currentFrame + 1) % 5;

	if (m_position.getY() < 0) {
		m_velocity.setY(2);
	} else if (m_position.getY() > SCREEN_HEIGHT-m_height) {
		m_velocity.setY(-2);
	}

	if (m_position.getX() < 0 || m_position.getX() > SCREEN_WIDTH-m_width) {
		m_velocity.setX(-m_velocity.getX());
	}

	SDLGameObject::update();
}

void Enemy::clean()
{

}
