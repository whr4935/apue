#ifndef _GAMEOVERSTATE_H
#define _GAMEOVERSTATE_H

#include "GameState.h"
#include "GameObject.h"
#include <vector>

class GameOverState :
	public GameState
{
public:
	GameOverState();
	~GameOverState();

	virtual void render();
	virtual void update();
	virtual bool onEnter();
	virtual bool onExit();

	std::string getStateID() const { return s_gameOverStateID; }

private:
	static std::string s_gameOverStateID;

	static void s_gameOverToMain();
	static void s_restartPlay();
	std::vector<GameObject*> m_gameObject;
};


#endif
