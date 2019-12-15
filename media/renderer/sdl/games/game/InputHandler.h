#ifndef _INPUTHANDLER_H
#define _INPUTHANDLER_H

#include <SDL2/SDL.h>
#include <vector>
#include "Vector2D.h"

class InputHandler
{
public:
	enum MouseButtons{
		LEFT,
		MIDDLE,
		RIGHT,
		BUTTON_TOTAL,
	};

	~InputHandler();

	static InputHandler* Instance() {
		if (s_pInstance == nullptr) {
			s_pInstance = new InputHandler();
		}
		return s_pInstance;
	}

	void update();
	void clean();
	void initialiseJoysticks();
	bool joysticksInitialised() {
		return m_bJoysticksInstalled;
	}
	bool getMousButtonState(int buttonNumber) {
		return m_mouseButtonStates[buttonNumber];
	}
	Vector2D* getMousePosition() {
		return m_mousePosition;
	}
	bool isKeyDown(SDL_Scancode key);
	void reset();

private:
	//handle keyboard events
	void onKeyDown();
	void onKeyUp();

	//handle mouse events
	void onMouseMove(SDL_Event& event);
	void onMouseButtonDown(SDL_Event& event);
	void onMouseButtonUp(SDL_Event& event);


private:
	InputHandler();
	static InputHandler* s_pInstance;

	std::vector<SDL_Joystick*> m_joysticks;
	bool m_bJoysticksInstalled;

	std::vector<bool> m_mouseButtonStates;
	Vector2D *m_mousePosition;
	const Uint8* m_keystate;
};





#endif
