#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#include <string>

class GameState
{
public:
	GameState();
	virtual ~GameState();

	virtual void update() = 0;
	virtual void render() = 0;
	virtual bool onEnter() = 0;
	virtual bool onExit() = 0;

	virtual std::string getStateID() const = 0;
};



#endif

