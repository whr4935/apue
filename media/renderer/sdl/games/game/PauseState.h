#ifndef _PAUSESTATE_H
#define _PAUSESTATE_H

#include "GameState.h"
#include "GameObject.h"


#include <vector>
#include <string>

class PauseState :
	public GameState
{
public:
	PauseState();
	~PauseState();

	virtual void update();
	virtual void render();
	virtual bool onEnter();
	virtual bool onExit();

	std::string getStateID() const { return s_pauseID; }

private:
	static std::string s_pauseID;

	static void s_pauseToMain();
	static void s_resumePlay();
	std::vector<GameObject*> m_gameObjects;
};


#endif
