#ifndef _GAMESTATEMACHINE_H
#define _GAMESTATEMACHINE_H

#include "GameState.h"
#include <vector>

class GameStateMachine
{
public:
	GameStateMachine();
	~GameStateMachine();

	void pushState(GameState *pState);
	void changeState(GameState *pState);
	void popState();

	void update();
	void render();

private:
	std::vector<GameState*> m_gameStates;
};


#endif
