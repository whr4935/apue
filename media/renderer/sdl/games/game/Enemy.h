#ifndef _ENEMY_H
#define _ENEMY_H

#include "SDLGameObject.h"
class Enemy :
	public SDLGameObject
{
public:
	Enemy(const LoadParams* pParams);
	~Enemy();

	virtual void draw();
	virtual void update();
	virtual void clean();
};

#endif
