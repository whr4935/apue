#ifndef _MENUBUTTON_H
#define _MENUBUTTON_H

#include "SDLGameObject.h"
class MenuButton :
	public SDLGameObject
{
public:
	enum ButtonState {
		MOUSE_OUT,
		MOUSE_OVER,
		MOUSE_CLICKED,
	};

	MenuButton(const LoadParams* pParams, void (*callback)());
	~MenuButton();

	virtual void draw();
	virtual void update();
	virtual void clean();

private:
	void(*m_callback)();
	bool m_bClicked;
};

#endif

