#ifndef _PLAYER_H
#define _PLAYER_H

#include "SDLGameObject.h"
class Player :
	public SDLGameObject
{
public:
	Player(const LoadParams* pParams);
	~Player();

	virtual void draw();
	virtual void update();
	virtual void clean();

private:
	void handleInput();
};


#endif

