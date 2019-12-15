#ifndef _PLAYSTATE_H
#define _PLAYSTATE_H

#include "GameState.h"
#include "GameObject.h"
#include "SDLGameObject.h"
#include <vector>

class PlayState :
	public GameState
{
public:
	PlayState();
	~PlayState();

	void render();
	void update();
	bool onEnter();
	bool onExit();

	std::string getStateID() const { return s_playID; }

private:
	static const std::string s_playID;
	std::vector<GameObject*> m_gameObjects;

	bool checkCollision(SDLGameObject* p1, SDLGameObject* p2);
};

#endif

