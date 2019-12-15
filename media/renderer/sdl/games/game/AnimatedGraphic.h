#ifndef _ANIMATEDGRAPHIC_H
#define _ANIMATEDGRAPHIC_H


#include "SDLGameObject.h"
class AnimatedGraphic :
	public SDLGameObject
{
public:
	AnimatedGraphic(const LoadParams* pParams, int animSpeed);
	~AnimatedGraphic();

	virtual void update();
	virtual void draw();
	virtual void clean();

private:
	int m_animaSpeed;
};


#endif
