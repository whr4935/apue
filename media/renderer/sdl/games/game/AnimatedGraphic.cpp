#include "AnimatedGraphic.h"



AnimatedGraphic::AnimatedGraphic(const LoadParams* pParams, int animSpeed) : SDLGameObject(pParams),
								m_animaSpeed(animSpeed)
{
}


AnimatedGraphic::~AnimatedGraphic()
{
}

void AnimatedGraphic::update()
{
	m_currentFrame = (m_currentFrame + 1) % 2;
}

void AnimatedGraphic::draw()
{
	SDLGameObject::draw();
}

void AnimatedGraphic::clean()
{
	SDLGameObject::clean();
}