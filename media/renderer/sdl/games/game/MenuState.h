#ifndef _MENUSTATE_H
#define _MENUSTATE_H

#include "GameState.h"
#include "MenuButton.h"

#include <vector>

class MenuState :
	public GameState
{
public:
	MenuState();
	~MenuState();

	virtual void update();
	virtual void render();
	virtual bool onEnter();
	virtual bool onExit();

	virtual std::string getStateID() const { return s_menuID; }

private:
	static void s_menuToPlay();
	static void s_exitFromMenu();

private:
	static const std::string s_menuID;
	std::vector<GameObject*> m_gameObjects;
};

#endif
