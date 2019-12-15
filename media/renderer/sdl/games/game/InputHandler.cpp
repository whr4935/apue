#include "InputHandler.h"
#include "Game.h"

#include <iostream>
#include <string>



InputHandler::InputHandler() : m_bJoysticksInstalled(false), m_keystate(nullptr), m_mousePosition(new Vector2D())
{
	for (int i = 0; i < BUTTON_TOTAL; ++i) {
		m_mouseButtonStates.push_back(false);
	}
}


InputHandler::~InputHandler()
{
	delete m_mousePosition;
}

InputHandler* InputHandler::s_pInstance = nullptr;

void InputHandler::initialiseJoysticks()
{
	if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0) {
		SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	}

	if (SDL_NumJoysticks() > 0) {
		for (int i = 0; i < SDL_NumJoysticks(); ++i) {
			SDL_Joystick* joy = SDL_JoystickOpen(i);

			if (SDL_JoystickGetAttached(joy)) {
				m_joysticks.push_back(joy);
			} else {
				std::cout << SDL_GetError() << std::endl;
			}
		}
		SDL_JoystickEventState(SDL_ENABLE);
		m_bJoysticksInstalled = true;
	} else {
		m_bJoysticksInstalled = false;
	}
}

bool InputHandler::isKeyDown(SDL_Scancode key)
{
	if (m_keystate != nullptr) {
		if (m_keystate[key] == 1)
			return true;
		else
			return false;
	}
	return false;
}

void InputHandler::onKeyDown()
{
	m_keystate = SDL_GetKeyboardState(0);
}

void InputHandler::onKeyUp()
{
	m_keystate = SDL_GetKeyboardState(0);
}

void InputHandler::onMouseMove(SDL_Event& event)
{
	m_mousePosition->setX(event.motion.x);
	m_mousePosition->setY(event.motion.y);
}

void InputHandler::onMouseButtonDown(SDL_Event& event)
{
	if (event.button.button == SDL_BUTTON_LEFT) {
		m_mouseButtonStates[LEFT] = true;
	} else if (event.button.button == SDL_BUTTON_MIDDLE) {
		m_mouseButtonStates[MIDDLE] = true;
	} else if (event.button.button == SDL_BUTTON_RIGHT) {
		m_mouseButtonStates[RIGHT] = true;
	}
}

void InputHandler::onMouseButtonUp(SDL_Event& event)
{
	if (event.button.button == SDL_BUTTON_LEFT) {
		m_mouseButtonStates[LEFT] = false;
	} else if (event.button.button == SDL_BUTTON_MIDDLE) {
		m_mouseButtonStates[MIDDLE] = false;
	} else if (event.button.button == SDL_BUTTON_RIGHT) {
		m_mouseButtonStates[RIGHT] = false;
	}
}

void InputHandler::update()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			Game::Instance()->quit();
			break;

		case SDL_MOUSEBUTTONDOWN:
			onMouseButtonDown(event);
			break;

		case SDL_MOUSEBUTTONUP:
			onMouseButtonUp(event);
			break;

		case SDL_MOUSEMOTION:
			onMouseMove(event);
			break;

		case SDL_KEYDOWN:
			onKeyDown();
			break;

		case SDL_KEYUP:
			onKeyUp();
			break;

		default:
			break;
		}
	}
}

void InputHandler::clean()
{
	if (m_bJoysticksInstalled) {
		for (int i = 0; i < SDL_NumJoysticks(); ++i) {
			SDL_JoystickClose(m_joysticks[i]);
		}
		m_bJoysticksInstalled = false;
	}

	m_joysticks.clear();
}

void InputHandler::reset()
{

}
